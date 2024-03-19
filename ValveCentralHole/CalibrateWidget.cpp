#include "CalibrateWidget.h"
#include "MessageBoxHelper.h"
#include "qmediadevices.h"


double CalibrateWidget::gauge_diameter_ = 0;
double CalibrateWidget::calibration_factor_ = 0;

UnitSelection CalibrateWidget::current_unit_selection_ = UnitSelection::MILLIMETERS;


// Performs image binarization on a copy of the input image, and sets the preview Mat to this copy
/**
 * \brief
 * \param calibrate_widget [in]: reference to calling CalibrateWidget instance
 * \param original_mat [in] : Original, input matrix from the image file selected by the user
 * \param is_update [in] : set to true only if the function is used to update preview image as a result of the threshold slider being moved
 */
static void CreateBinaryImagePreview(CalibrateWidget& calibrate_widget, const Mat& original_mat, int threshold_value, int threshold_mode_selection_index, bool is_update, std::mutex& mutex) {
	Mat temp = original_mat.clone();

	ThresholdMode selected_threshold_mode = static_cast<ThresholdMode>(threshold_mode_selection_index);

	BinarizeImageHelper::BinarizeImage(temp, threshold_value, ThresholdMode::INVERTED);

	if (threshold_mode_selection_index == 0)
	{
		BinarizeImageHelper::InvertBinaryImage(temp);
	}

	// Acquire the mutex lock to avoid race condition where two threads set the binary preview mat at the same time
	{
		std::unique_lock<std::mutex> lock(mutex);
		calibrate_widget.SetPreviewMat(temp);
	}


	// Check if the image mat is extremely large (5000+ pixels in both directions)
	// If it is, we should only update when the sliders stops moving, or it'll lag too much
	if (temp.rows >= 5000 && temp.cols >= 5000)
	{
		// If the thread pool's queue is empty, this means that this working thread performed the last preview mat update so it should be updated in the GUI
		if (calibrate_widget.GetWidgetThreadPool().getTaskQueue().empty())
		{
			QMetaObject::invokeMethod(&calibrate_widget, [&calibrate_widget, temp, is_update]() mutable
				{
					if (is_update)
					{
						emit calibrate_widget.UpdatePreviewMat();
					}
				});
		}
	}
	else
	{
		QMetaObject::invokeMethod(&calibrate_widget, [&calibrate_widget, temp, is_update]() mutable
			{
				if (is_update)
				{
					emit calibrate_widget.UpdatePreviewMat();
				}
			});
	}

}

QString GetUnitSuffix(UnitSelection unit_selection_)
{
	QString result;
	switch (unit_selection_)
	{
	case  UnitSelection::INCHES:
	{
		result = "in^2";
		break;
	}
	case UnitSelection::MILLIMETERS:
	{
		result = "mm^2";
		break;
	}
	}
	return result;
}


CalibrateWidget::CalibrateWidget(const std::unique_ptr<bool>& gauge_helper_flag, QWidget* parent) : gauge_helper_flag_(gauge_helper_flag), QWidget(parent), ui(new Ui::CalibrateWidget) {

	ui->setupUi(this);

	// Initialize thread pool with 3 threads, which will be used for updating the binary preview image
	tp_.setNumThreads(3);
	InitializeUIElements();
	CheckForLastCalibrationParameters();
	ConnectEventListeners();

	const QList<QCameraDevice> cameras = QMediaDevices::videoInputs();
	for (const QCameraDevice& camera : cameras)
	{
		qDebug() << "Detected Camera: " << camera.description();
	}
}


void CalibrateWidget::SetPreviewMat(Mat mat)
{
	binarized_preview_image_mat_ = mat;
}

const ThreadPool& CalibrateWidget::GetWidgetThreadPool() const
{
	return tp_;
}

void CalibrateWidget::ApplyLastSavedParameters()
{
	if (!gauge_parameters_.IsValid())
	{
		return;
	}

	diameter_input_->setValue(gauge_parameters_.GetGaugeDiameter());
	threshold_input_slider_->setValue(gauge_parameters_.GetThresholdValue());
	diameter_unit_selection_->setCurrentIndex(static_cast<int>(gauge_parameters_.GetUnitSelection()));
	threshold_mode_combo_box_->setCurrentIndex(static_cast<int>(gauge_parameters_.GetThresholdMode()));
	threshold_value_label_->setText(QString::number(gauge_parameters_.GetThresholdValue()));
	DisplaySelectedImage(gauge_parameters_.GetImageFileName(), true);
}

void CalibrateWidget::ReceiveAndDisplayCameraImage(Mat mat_from_camera)
{
	// Grayscale the captured image
	cvtColor(mat_from_camera, mat_from_camera, COLOR_BGR2GRAY);

	// Resize the mat to be the correct dimensions so that it can be displayed
	cv::resize(mat_from_camera, mat_from_camera, Size(IMAGE_WIDTH, IMAGE_HEIGHT), 0, 0, INTER_CUBIC);

	current_image_mat_ = mat_from_camera;
	imshow("Result Image", current_image_mat_);
	DisplaySelectedImage(current_image_mat_, QImage::Format::Format_Grayscale8, true);
}

void CalibrateWidget::ReceiveAndDisplayCameraImage(const QString& image_name)
{
	DisplaySelectedImage(image_name, true);
}

void CalibrateWidget::SetMirrorCallback(const std::function<void(bool)>& callback)
{
	mirror_action_callback_ = callback;
}

void CalibrateWidget::SetCalibrateDataCallback(const std::function<void(const CalibrateData&)>& callback)
{
	save_calibrate_data_to_file_callback_ = callback;
}


double CalibrateWidget::GetGaugeDiameter() {
	return gauge_diameter_;
}

int CalibrateWidget::GetThresholdValue() const {
	return threshold_value_;
}

double CalibrateWidget::GetCalibrationFactor() {
	return calibration_factor_;
}

void CalibrateWidget::DisplaySelectedImage(const QString& filename, bool should_show_binary_immediately)
{
	current_image_mat_ = imread(filename.toStdString(), IMREAD_GRAYSCALE);
	if (current_image_mat_.empty())
	{
		return;
	}

	num_total_pixels_ = current_image_mat_.rows * current_image_mat_.cols;

	QPixmap image(filename);
	QPixmap scaled = image.scaled(QSize(IMAGE_WIDTH, IMAGE_HEIGHT));
	original_image_->setPixmap(scaled);
	original_image_->SetNewOriginalPixmap(scaled);

	gauge_parameters_.SetImageFileName(filename);
	SaveCurrentParametersToDatabase();

	if (should_show_binary_immediately)
	{
		SetIsCurrentlyShowingPreview(true);
	}

	// Binarize the selected image on a separate thread to have it ready for preview
	auto thread_handle = std::async(std::launch::async, &CreateBinaryImagePreview,
		std::ref(*this), std::ref(current_image_mat_), threshold_value_, threshold_mode_combo_box_->currentIndex(), should_show_binary_immediately, std::ref(mutex_));
}

void CalibrateWidget::DisplaySelectedImage(const Mat& selected_mat, const QImage::Format& image_format, bool should_show_binary_immediately)
{
	if (current_image_mat_.empty())
	{
		MessageBoxHelper::ShowErrorDialog("An error occurred while attempting to display the image file");
		return;
	}
	current_image_mat_ = selected_mat.clone();

	num_total_pixels_ = current_image_mat_.rows * current_image_mat_.cols;

	QImage image(current_image_mat_.data, current_image_mat_.cols, current_image_mat_.rows, current_image_mat_.step, image_format);
	QPixmap pixmap = QPixmap::fromImage(image).scaled(IMAGE_WIDTH, IMAGE_HEIGHT);
	original_image_->setPixmap(pixmap);
	original_image_->SetNewOriginalPixmap(pixmap);

	// Now we can grayscale the input mat if it isn't already in grayscale
	switch (image_format)
	{
	case QImage::Format::Format_BGR888:
	{
		cv::cvtColor(current_image_mat_, current_image_mat_, COLOR_BGR2GRAY);
		break;
	}
	}

	if (should_show_binary_immediately)
	{
		SetIsCurrentlyShowingPreview(true);
	}

	auto thread_handle = std::async(std::launch::async, &CreateBinaryImagePreview,
		std::ref(*this), std::ref(current_image_mat_), threshold_value_, threshold_mode_combo_box_->currentIndex(), should_show_binary_immediately, std::ref(mutex_));
}


void CalibrateWidget::DisplayPreviewMat()
{
	if (binarized_preview_image_mat_.empty())
	{
		MessageBoxHelper::ShowErrorDialog("Select an input image first");
		return;
	}

	QImage preview_image(binarized_preview_image_mat_.data, binarized_preview_image_mat_.cols, binarized_preview_image_mat_.rows,
		binarized_preview_image_mat_.step, QImage::Format_Grayscale8);
	if (preview_image.isNull())
	{
		qDebug() << "Error converting binary image into QImage object";
		return;
	}

	QPixmap preview_pixmap = QPixmap::fromImage(preview_image);
	if (preview_pixmap.isNull())
	{
		qDebug() << "Error converting QImage into QPixmap";
		return;
	}

	binarized_image_->setPixmap(preview_pixmap.scaled(QSize(IMAGE_WIDTH, IMAGE_HEIGHT)));
}


void CalibrateWidget::InitializeUIElements()
{
	diameter_input_.reset(ui->diameter_input);
	diameter_unit_selection_.reset(ui->diameter_unit_selection);
	threshold_input_slider_.reset(ui->threshold_value_slider);
	threshold_mode_combo_box_.reset(ui->threshold_mode_selection);
	is_saline_checkbox_.reset(ui->is_saline_selection);
	threshold_value_label_.reset(ui->threshold_value_display);
	select_file_button_.reset(ui->select_image_file);
	preview_btn_.reset(ui->preview_btn);
	calibrate_btn_.reset(ui->calibrate_btn);
	calibration_factor_label_.reset(ui->calibration_factor_label);
	threshold_mode_tooltip_label_.reset(ui->threshold_mode_tooltip_);
	saline_tooltip_label_.reset(ui->saline_tooltip);
	clear_lines_btn_.reset(ui->clear_lines);
	crop_image_btn_.reset(ui->crop_image_btn);
	clear_image_btn_.reset(ui->clear_image_btn);

	binary_details_label_ = std::make_unique<BinaryDetailsLabel>(&num_total_pixels_, &num_white_pixels_, this);
	binary_details_label_->setFixedWidth(21);
	binary_details_label_->setFixedHeight(21);
	binary_details_label_->move(1220, 180);
	binary_details_label_->setPixmap(QPixmap("tooltip_question_mark.png").scaled(binary_details_label_->width(), binary_details_label_->height()));

	save_binary_btn_.reset(ui->save_binary_btn);
	save_binary_btn_->setEnabled(false);
	save_binary_dialog_ = std::make_unique<QFileDialog>(this);
	save_binary_dialog_->setFileMode(QFileDialog::FileMode::Directory);

	original_image_ = std::make_unique<CalibrationGaugeLabel>(gauge_helper_flag_, this);
	binarized_image_ = std::make_unique<BinaryGaugeLabel>(gauge_helper_flag_, this);
	original_image_->setFixedWidth(IMAGE_WIDTH);
	original_image_->setFixedHeight(IMAGE_HEIGHT);
	binarized_image_->setFixedWidth(IMAGE_WIDTH);
	binarized_image_->setFixedHeight(IMAGE_HEIGHT);
	original_image_->move(10, 220);
	binarized_image_->move(640, 220);
	original_image_->setStyleSheet("background: brown;");
	binarized_image_->setStyleSheet("background: brown;");
	binarized_image_->SetCanDraw(false);
	binarized_image_->SetShouldClearOnRepaint(false);

	// This function is called by the CalibrationGaugeLabel that holds the original image, when the user draws a line in the original image
	// In this function, we will take the end points of the line that was drawn, and send them to the CalibrationGaugeLabel that holds the binary preview image
	original_image_->SetOnMouseReleaseCallback([this](const QPoint& start, const QPoint& end)
		{
			start_ = start;
			end_ = end;
			binarized_image_->SetLineStartPoint(start_);
			binarized_image_->SetLineEndPoint(end_);
		});

	clear_lines_btn_->setVisible(false);

	file_select_ = std::make_unique<QFileDialog>(this, "Select Gauge Image");

	diameter_unit_selection_->addItem("mm");
	diameter_unit_selection_->addItem("in");

	threshold_input_slider_->setMinimum(0);
	threshold_input_slider_->setMaximum(255);
	threshold_input_slider_->setSingleStep(1);
	threshold_input_slider_->setValue(127);

	threshold_mode_combo_box_->addItem("Standard");
	threshold_mode_combo_box_->addItem("Inverted");

	diameter_input_->setMinimum(0);
	diameter_input_->setMaximum(1000000);
	diameter_input_->setDecimals(5);

	file_select_->setStyleSheet("background: white;");

	threshold_mode_tooltip_label_->setPixmap(QPixmap("tooltip_question_mark.png")
		.scaled(threshold_mode_tooltip_label_->width(), threshold_mode_tooltip_label_->height()));
	saline_tooltip_label_->setPixmap(QPixmap("tooltip_question_mark.png")
		.scaled(saline_tooltip_label_->width(), saline_tooltip_label_->height()));

	threshold_mode_tooltip_label_->setToolTip("Choose the thresholding mode. \nIn Standard Thresholding mode, pixels with intensity values over the threshold value will become white, and intensity values below will be black."
		"\nIn Inverted, the inverse is applied");
	saline_tooltip_label_->setToolTip("Check the box to indicate that the calculations should take into account the index of refraction for saline."
		"\nIn particular, it will account for the magnification cause by refraction");

	threshold_mode_tooltip_label_->setToolTipDuration(20000);
	saline_tooltip_label_->setToolTipDuration(20000);
}

void CalibrateWidget::CheckForLastCalibrationParameters()
{
	CalibrationGaugeParametersDAO dao;
	if (dao.IsTableEmpty())
	{
		qDebug() << "Table is empty, creating first row...";
		dao.AddGaugeParameters(gauge_parameters_, [this](bool query_status)
			{
				if (!query_status)
				{
					qDebug() << "Failed to add the first row to the calibration_gauge_parameters table";
				}
			});
	}
	else
	{
		gauge_parameters_ = dao.RetrieveGaugeParameters();
		qDebug() << "Retrieved parameters " << gauge_parameters_.toString();

		gauge_diameter_ = gauge_parameters_.GetGaugeDiameter();
		threshold_value_ = gauge_parameters_.GetThresholdValue();
		selected_image_filename_ = gauge_parameters_.GetImageFileName();

		// Update the widgets to reflect this data here
		RefreshWidgetsWithDatabase(gauge_parameters_);
	}
}


void CalibrateWidget::DisplayCalibrationFactor()
{
	QString unit_display = GetUnitSuffix(current_unit_selection_);
	calibration_factor_label_->setText("Calibration Factor: " + QString::number(calibration_factor_) + " " + unit_display);
}

// Add event listeners
void CalibrateWidget::ConnectEventListeners()
{
	connect(diameter_input_.get(), &QDoubleSpinBox::valueChanged, this, [this](double new_value) {
		qDebug() << "Inputted Diameter: " << new_value;
		gauge_diameter_ = new_value;
		});

	connect(diameter_input_.get(), &QAbstractSpinBox::editingFinished, this, [this]()
		{
			gauge_parameters_.SetGaugeDiameter(diameter_input_->value());
			SaveCurrentParametersToDatabase();
		});

	connect(diameter_unit_selection_.get(), &QComboBox::currentIndexChanged, this, [this](int index)
		{
			switch (static_cast<UnitSelection>(index))
			{
				// Millimeters selection, convert from inches to millimeters
			case UnitSelection::MILLIMETERS:
			{
				gauge_diameter_ *= 25.4;
				current_unit_selection_ = UnitSelection::MILLIMETERS;
				break;
			}
			// Inches selection, convert from millimeters to inches
			case UnitSelection::INCHES:
			{
				gauge_diameter_ /= 25.4;
				current_unit_selection_ = UnitSelection::INCHES;
				break;
			}
			}
			diameter_input_->setValue(gauge_diameter_);
			gauge_parameters_.SetUnitSelection(current_unit_selection_);
			gauge_parameters_.SetGaugeDiameter(gauge_diameter_);

			SaveCurrentParametersToDatabase();
		});

	connect(threshold_input_slider_.get(), &QAbstractSlider::sliderMoved, this, [this](int value)
		{
			threshold_value_ = value;
			qDebug() << "Slider value changed: " << value;
			threshold_value_label_->setText(QString::number(threshold_value_));

			// When the slider is moved, we want to update the preview image if it is currently shown
			if (!current_image_mat_.empty() && !binarized_preview_image_mat_.empty() && isCurrentlyShowingPreview)
			{
				tp_.enqueue([this, value]()
					{
						CreateBinaryImagePreview(*this, current_image_mat_, value, threshold_mode_combo_box_->currentIndex(), true, mutex_);
					});
			}
		});

	connect(threshold_input_slider_.get(), &QSlider::sliderReleased, this, [this]()
		{
			gauge_parameters_.SetThresholdValue(threshold_value_);
			SaveCurrentParametersToDatabase();
		});

	connect(threshold_mode_combo_box_.get(), &QComboBox::currentIndexChanged, this, [this](int index)
		{
			if (!binarized_preview_image_mat_.empty())
			{
				BinarizeImageHelper::InvertBinaryImage(binarized_preview_image_mat_);
			}
			if (isCurrentlyShowingPreview)
			{
				DisplayPreviewMat();
			}

			gauge_parameters_.SetThresholdMode(static_cast<ThresholdMode>(index));
			SaveCurrentParametersToDatabase();
		});

	connect(this, &CalibrateWidget::UpdatePreviewMat, this, [this]()
		{
			DisplayPreviewMat();
		});

	connect(select_file_button_.get(), &QPushButton::clicked, this, [this]() {
		file_select_->exec();
		});

	connect(file_select_.get(), &QFileDialog::fileSelected, this, [this](const QString& file) {
		qDebug() << "Selected file name: " << file;
		// Check if the selected file is a valid image format
		if (!file.contains(".jpg", Qt::CaseInsensitive) && !file.contains(".png", Qt::CaseInsensitive)) {
			MessageBoxHelper::ShowErrorDialog("Selected file was not in a valid image format (.jpg or .png)");
			return;
		}
		selected_image_filename_ = file;
		DisplaySelectedImage(selected_image_filename_);
		});

	// When the preview button is pressed, we only want to binarize the input image for previewing (don't perform any calibration work yet)
	connect(preview_btn_.get(), &QPushButton::clicked, this, [this]() {
		SetIsCurrentlyShowingPreview(true);
		DisplayPreviewMat();
		});

	connect(calibrate_btn_.get(), &QPushButton::clicked, this, [this]() {


		if (current_image_mat_.empty() || binarized_preview_image_mat_.empty())
		{
			MessageBoxHelper::ShowErrorDialog("Select an input image first");
			return;
		}
		if (gauge_diameter_ <= 0)
		{
			MessageBoxHelper::ShowErrorDialog("Please input a valid gauge diameter value");
			return;
		}

		std::pair<unsigned long long, unsigned long long> num_on_off_pixels = BinarizeImageHelper::GetNumberOfOnAndOffPixels(binarized_preview_image_mat_, tp_);
		num_white_pixels_ = num_on_off_pixels.first;
		double calibration_factor;
		switch (threshold_mode_combo_box_->currentIndex())
		{
			// Standard thresholding mode, we should use the number of off pixels to get the calibration factor
		case 0:
		{
			calibration_factor = BinarizeImageHelper::GetCalibrationGaugeFactor(num_on_off_pixels.second, gauge_diameter_);
			break;
		}
		case 1:
		{
			calibration_factor = BinarizeImageHelper::GetCalibrationGaugeFactor(num_on_off_pixels.first, gauge_diameter_);
			break;
		}
		}

		// Apply any necessary saline transformations to the resulting calibration factor
		if (is_saline_checkbox_->isChecked())
		{
			calibration_factor = BinarizeImageHelper::ApplySalineTransformation(calibration_factor);
		}

		emit this->OnCalibrationComplete(calibration_factor);

		});

	connect(this, &CalibrateWidget::OnCalibrationComplete, this, [this](double calibrationFactor) {
		calibration_factor_ = calibrationFactor;
		DisplayCalibrationFactor();
		if (save_calibrate_data_to_file_callback_)
		{
			QList<QString> parsed_filename = selected_image_filename_.split("/");
			QString target_filename = parsed_filename[parsed_filename.size() - 1];

			save_calibrate_data_to_file_callback_(CalibrateData(target_filename.toStdString(),
				QDateTime::currentDateTimeUtc().toLocalTime().toString().toStdString(), current_unit_selection_, calibrationFactor));
		}
		});

	connect(crop_image_btn_.get(), &QPushButton::clicked, this, [this]()
		{
			if (current_image_mat_.empty())
			{
				MessageBoxHelper::ShowErrorDialog("An input image must be selected before cropping");
				return;
			}
			ImageCropDialog* crop_dialog = new ImageCropDialog(selected_image_filename_, this);
			crop_dialog->SetConfirmCallback([this](const Mat& cropped)
				{
					DisplaySelectedImage(cropped, QImage::Format::Format_BGR888);
					//current_image_mat_ = current_image_mat_(Rect(cropped_region.x(), cropped_region.y(), cropped_region.width(), cropped_region.height()));
				});
			crop_dialog->exec();
		});

	connect(clear_image_btn_.get(), &QPushButton::clicked, this, [this]()
		{
			// Clear the current mat and binarized mat variables, and clear the pixmaps for the labels as well
			ClearGaugeLines();
			current_image_mat_ = Mat();
			binarized_preview_image_mat_ = Mat();
			original_image_->setPixmap(QPixmap());
			binarized_image_->setPixmap(QPixmap());
			num_total_pixels_ = 0;
			num_white_pixels_ = 0;
			SetIsCurrentlyShowingPreview(false);
		});

	connect(this, &CalibrateWidget::ShouldClearHelperGaugeLines, this, [this]()
		{
			ClearGaugeLines();
		});

	connect(this, &CalibrateWidget::MirrorDrawnLinesToPreview, this, [this]()
		{
			if (!start_.isNull() && !end_.isNull())
			{
				binarized_image_->SetLineStartPoint(start_);
				binarized_image_->SetLineEndPoint(end_);
			}
			binarized_image_->PaintEventUpdate();
		});

	connect(save_binary_btn_.get(), &QPushButton::clicked, this, [this]()
		{
			save_binary_dialog_->exec();
		});

	connect(save_binary_dialog_.get(), &QFileDialog::fileSelected, this, [this](const QString& directory)
		{
			// Create a temp QString which holds the same value as selected_image_filename without the file extenstion
			QStringList temp = selected_image_filename_.split('.');
			QStringList split_file_name = temp[0].split('/');
			QString file_name = split_file_name[split_file_name.size() - 1];
			QString file_extension = temp[1];
			qDebug() << "Filename: " << file_name << "\tFile Extension: " << file_extension;
			if (binarized_preview_image_mat_.empty() || !imwrite((directory + "/" + file_name + " - Binary" + "." + file_extension).toStdString(), binarized_preview_image_mat_))
			{
				MessageBoxHelper::ShowErrorDialog("An error occurred while attempting to save the binary image (binary image mat is empty)");
			}
		});
}

void CalibrateWidget::SaveCurrentParametersToDatabase()
{
	tp_.enqueue([this]()
		{
			CalibrationGaugeParametersDAO dao;
			dao.UpdateGaugeParameters(gauge_parameters_, [](bool query_status)
				{
					if (!query_status)
					{
						qDebug() << "Failed to update latest set calibration gauge parameters";
					}
				});
		});
}

void CalibrateWidget::SetIsCurrentlyShowingPreview(bool status)
{
	isCurrentlyShowingPreview = status;
	save_binary_btn_->setEnabled(status);
	if (mirror_action_callback_)
	{
		mirror_action_callback_(isCurrentlyShowingPreview);
	}
}

void CalibrateWidget::ClearGaugeLines()
{
	original_image_->ClearDrawnLines();
	binarized_image_->ClearDrawnLines();
	start_ = QPoint();
	end_ = QPoint();
	binarized_image_->SetLineStartPoint(QPoint());
	binarized_image_->SetLineEndPoint(QPoint());
}

void CalibrateWidget::RefreshWidgetsWithDatabase(const CalibrationGaugeParameters& parameters)
{
	diameter_input_->setValue(parameters.GetGaugeDiameter());

	switch (parameters.GetUnitSelection())
	{
	case UnitSelection::MILLIMETERS:
	{
		diameter_unit_selection_->setCurrentIndex(0);
		break;
	}
	case UnitSelection::INCHES:
	{
		diameter_unit_selection_->setCurrentIndex(1);
		break;
	}
	}

	threshold_input_slider_->setValue(parameters.GetThresholdValue());

	switch (parameters.GetThresholdMode())
	{
	case ThresholdMode::STANDARD:
	{
		threshold_mode_combo_box_->setCurrentIndex(0);
		break;
	}
	case ThresholdMode::INVERTED:
	{
		threshold_mode_combo_box_->setCurrentIndex(1);
		break;
	}
	}

	DisplaySelectedImage(parameters.GetImageFileName());
}
