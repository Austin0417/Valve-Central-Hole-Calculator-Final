#include "MeasureWidget.h"
#include "MeasureDataDAO.h"
#include "messageboxhelper.h"


static void CreateBinaryImagePreview(MeasureWidget& measure_widget, const Mat& mat, bool is_update)
{
	Mat temp = mat.clone();
	BinarizeImageHelper::BinarizeImage(temp, measure_widget.GetThresholdValue(), ThresholdMode::INVERTED);
	measure_widget.SetPreviewMat(temp);

	if (is_update)
	{
		emit measure_widget.UpdatePreviewMat();
	}
}


MeasureWidget::MeasureWidget(QWidget* parent)
	: QWidget(parent)
	, ui(new Ui::MeasureWidget)
{
	ui->setupUi(this);

	calibration_factor_label_.reset(ui->current_calibration_factor);
	calculated_area_label_.reset(ui->valve_area_label);
	select_valve_image_.reset(ui->select_valve_image);
	preview_btn_.reset(ui->preview_btn);
	measure_btn_.reset(ui->measure_btn);
	original_image_.reset(ui->original_valve);
	binarized_image_.reset(ui->binarized_valve);
	threshold_value_slider_.reset(ui->threshold_value_slider);
	threshold_value_display_label_.reset(ui->threshold_value_display);

	threshold_value_spin_box_ = std::make_unique<ThresholdValueSpinBox>(this);
	save_binary_image_btn_ = std::make_unique<SaveBinaryImageButton>(1110, 140, 111, 24, this);
	save_binary_image_btn_->BindBinaryMat(&binarized_image_preview_mat_);

	import_binary_image_btn_ = std::make_unique<ImportBinaryImageButton>(954, 140, 121, 24, this);
	import_binary_image_btn_->AttachToParentLabel(binarized_image_.get());
	import_binary_image_btn_->AttachParentWidgetCallback([this](const QString& selected_file)
		{
			binarized_image_preview_mat_ = imread(selected_file.toStdString(), IMREAD_GRAYSCALE);
		});

	file_dialog_ = std::make_unique<QFileDialog>(this, "Select Valve Image");
	file_dialog_->setStyleSheet("background: white;");

	InitializeUIElements();
	StartDatabaseWriteThread();
	ConnectEventListeners();
}


void MeasureWidget::DisplayPreviewMat()
{
	if (current_image_mat_.empty() || binarized_image_preview_mat_.empty())
	{
		MessageBoxHelper::ShowErrorDialog("Select an input image first");
		return;
	}

	QImage image(binarized_image_preview_mat_.data, binarized_image_preview_mat_.cols, binarized_image_preview_mat_.rows, binarized_image_preview_mat_.step, QImage::Format_Grayscale8);
	QPixmap pixmap = QPixmap::fromImage(image, Qt::ImageConversionFlag::MonoOnly);
	binarized_image_->setPixmap(pixmap.scaled(binarized_image_->width(), binarized_image_->height()));
}


void MeasureWidget::InitializeUIElements() {
	calibration_factor_label_->setText(calibration_factor_label_->text() + QString::number(CalibrateWidget::GetCalibrationFactor()));

	threshold_value_slider_->setMinimum(0);
	threshold_value_slider_->setMaximum(255);
	threshold_value_slider_->setSingleStep(1);
	threshold_value_slider_->setValue(127);
}


void MeasureWidget::StartDatabaseWriteThread()
{
	// Worker thread which will process MeasureData objects from the latest_measure_data_ queue to insert into the database when new data is available
	std::thread([this]()
		{
			MeasureDataDAO dao;
			while (true)
			{
				{
					std::unique_lock<std::mutex> lock(database_thread_wait_mutex_);
					database_thread_cv_.wait(lock, [this]()
						{
							return !latest_measure_data_.empty();
						});
				}
				const MeasureData& data = latest_measure_data_.front();
				if (dao.InsertMeasureData(data))
				{
					qDebug() << "Successfully inserted new measure data";
				}

				{
					std::unique_lock<std::mutex> lock(measure_data_queue_mutex_);
					latest_measure_data_.pop();
				}
			}
		}).detach();
}


void MeasureWidget::PerformValveAreaMeasurement()
{
	if (current_image_mat_.empty() && binarized_image_preview_mat_.empty())
	{
		MessageBoxHelper::ShowErrorDialog("Select an input image first");
		return;
	}

	auto thread_handle = std::async(std::launch::async, [this]()
		{
			std::pair<unsigned long long, unsigned long long> num_on_off_pixels = BinarizeImageHelper::GetNumberOfOnAndOffPixels(binarized_image_preview_mat_);
			emit this->onAreaCalculationComplete(BinarizeImageHelper::CalculateValveArea(num_on_off_pixels.first, CalibrateWidget::GetCalibrationFactor()));
		});
}


int MeasureWidget::GetThresholdValue() const
{
	return threshold_value_;
}

void MeasureWidget::SetPreviewMat(Mat preview_mat)
{
	binarized_image_preview_mat_ = preview_mat;
}

void MeasureWidget::SetMeasureDataCallback(const std::function<void(const MeasureData&)>& callback)
{
	measure_data_callback_ = callback;
}

void MeasureWidget::SetCurrentImageFilename(const QString& filename)
{
	current_image_filename_ = filename;
	if (save_binary_image_btn_ != nullptr)
	{
		save_binary_image_btn_->UpdateCurrentImageFilename(current_image_filename_);
	}
}


void MeasureWidget::ConnectEventListeners() {
	connect(select_valve_image_.get(), &QPushButton::clicked, this, [this]() {
		file_dialog_->exec();
		});

	connect(preview_btn_.get(), &QPushButton::clicked, this, [this]() {
		DisplayPreviewMat();
		isCurrentlyShowingPreview = true;
		});

	connect(measure_btn_.get(), &QPushButton::clicked, this, [this]() {
		PerformValveAreaMeasurement();
		});

	connect(threshold_value_spin_box_.get(), &ThresholdValueSpinBox::OnReturnPressed, this, [this](int threshold_value)
		{
			PerformValveAreaMeasurement();
		});

	connect(this, &MeasureWidget::onAreaCalculationComplete, this, [this](double valve_area)
		{
			QString unit_suffix = GetUnitSuffix(CalibrateWidget::current_unit_selection_);
			calculated_area_label_->setText("Calculated Valve Area: " + QString::number(valve_area) + " " + unit_suffix);

			QList<QString> split_filename = current_image_filename_.split("/");
			QString target_filename = split_filename[split_filename.size() - 1];

			{
				std::unique_lock<std::mutex> lock(measure_data_queue_mutex_);
				latest_measure_data_.emplace(target_filename.toStdString(), QDateTime::currentDateTimeUtc().toString().toStdString(), CalibrateWidget::current_unit_selection_, valve_area);
			}
			database_thread_cv_.notify_one();
		});

	connect(file_dialog_.get(), &QFileDialog::fileSelected, this, [this](const QString& filename) {
		qDebug() << "Valve Image filepath: " << filename;
		if (!filename.contains(".jpg", Qt::CaseInsensitive) && !filename.contains(".png", Qt::CaseInsensitive)) {
			MessageBoxHelper::ShowErrorDialog("Selected file is not a valid image format (.jpg or .png)");
			return;
		}
		current_image_mat_ = imread(filename.toStdString(), IMREAD_GRAYSCALE);
		if (current_image_mat_.empty())
		{
			MessageBoxHelper::ShowErrorDialog("An error occurred while trying to open the image file");
			return;
		}

		QPixmap image = QPixmap(filename).scaled(QSize(IMAGE_WIDTH, IMAGE_HEIGHT));
		original_image_->setPixmap(image);
		SetCurrentImageFilename(filename);

		auto thread_handle = std::async(std::launch::async, &CreateBinaryImagePreview, std::ref(*this), std::ref(current_image_mat_), false);
		});

	connect(threshold_value_slider_.get(), &QAbstractSlider::valueChanged, this, [this](int value)
		{
			threshold_value_ = value;
			threshold_value_display_label_->setText(QString::number(threshold_value_));

			if (!current_image_mat_.empty() && !binarized_image_preview_mat_.empty() && isCurrentlyShowingPreview)
			{
				// Update the preview image with the new threshold value
				std::future<void> thread_handle = std::async(std::launch::async, &CreateBinaryImagePreview, std::ref(*this), std::ref(current_image_mat_), true);
			}
		});

	connect(this, &MeasureWidget::UpdatePreviewMat, this, [this]()
		{
			DisplayPreviewMat();
		});

	connect(this, &MeasureWidget::OnPreviewMatResizeComplete, this, [this](const Mat& resized)
		{

		});

}

void MeasureWidget::RefreshCalibrationFactor()
{
	QString unit_suffix = GetUnitSuffix(CalibrateWidget::current_unit_selection_);
	calibration_factor_label_->setText("Calibration Factor: " + QString::number(CalibrateWidget::GetCalibrationFactor()) + " " + unit_suffix);
}

MeasureWidget::~MeasureWidget()
{
	delete ui;
}