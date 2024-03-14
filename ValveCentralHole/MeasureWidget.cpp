#include "MeasureWidget.h"
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

	file_dialog_ = std::make_unique<QFileDialog>(this, "Select Valve Image");
	file_dialog_->setStyleSheet("background: white;");

	InitializeUIElements();
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

int MeasureWidget::GetThresholdValue() const
{
	return threshold_value_;
}

void MeasureWidget::SetPreviewMat(Mat preview_mat)
{
	binarized_image_preview_mat_ = preview_mat;
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
		// TODO Implement functionality for measuring the valve here
		if (current_image_mat_.empty() || binarized_image_preview_mat_.empty())
		{
			MessageBoxHelper::ShowErrorDialog("Select an input image first");
			return;
		}

		auto thread_handle = std::async(std::launch::async, [this]()
			{
				std::pair<unsigned long long, unsigned long long> num_on_off_pixels = BinarizeImageHelper::GetNumberOfOnAndOffPixels(binarized_image_preview_mat_);
				emit this->onAreaCalculationComplete(BinarizeImageHelper::CalculateValveArea(num_on_off_pixels.first, CalibrateWidget::GetCalibrationFactor()));
			});
		});

	connect(this, &MeasureWidget::onAreaCalculationComplete, this, [this](double valve_area)
		{
			QString unit_suffix = GetUnitSuffix(CalibrateWidget::current_unit_selection_);
			calculated_area_label_->setText("Calculated Valve Area: " + QString::number(valve_area) + " " + unit_suffix);
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