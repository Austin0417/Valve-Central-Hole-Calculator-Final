#include "CameraConfirmationDialog.h"
#include "CameraDisplayDialog.h"
#include "BinarizeImageHelper.h"
#include "MessageBoxHelper.h"


CameraConfirmationDialog::CameraConfirmationDialog(Mat captured_mat, QWidget* parent) : captured_mat_(captured_mat),
QDialog(parent)
{
	ui.setupUi(this);
	setWindowTitle("Confirm Image");
	setWindowIcon(QIcon("window_icon.jpg"));
	InitializeUiElements();
	ConnectEventListeners();
}


void CameraConfirmationDialog::InitializeUiElements()
{
	crop_btn_.reset(ui.camera_crop_btn);
	displayed_mat_label_ = std::make_unique<CameraConfirmLabel>(is_crop_enabled_, this);
	retake_btn_.reset(ui.camera_retake_btn);
	confirm_btn_.reset(ui.camera_confirm_btn);
	clear_crop_btn.reset(ui.clear_crop_lines_btn);
	image_name_input_.reset(ui.captured_image_name_line_edit);
	jpg_btn_.reset(ui.jpg_btn_);
	png_btn_.reset(ui.png_btn_);
	radio_btn_group_ = std::make_unique<QButtonGroup>(this);
	file_dialog_ = std::make_unique<QFileDialog>(this);
	file_dialog_->setFileMode(QFileDialog::FileMode::Directory);

	clear_crop_btn->setVisible(false);

	crop_btn_->setIcon(QIcon("crop_icon.png"));
	crop_btn_->setCheckable(true);

	displayed_mat_label_->setFixedWidth(1050);
	displayed_mat_label_->setFixedHeight(600);
	displayed_mat_label_->move(80, 10);

	// Callback that will be called everytime the user crops the image
	displayed_mat_label_->SetCropCallback([this](int x, int y, int width, int height)
		{
			// When the user crops the image, we will store the resulting sub matrix
			BinarizeImageHelper::ApplyImageCroppingFromQLabel(captured_mat_, cropped_captured_mat_, x, y,
			std::abs(width), std::abs(height), displayed_mat_label_->width(), displayed_mat_label_->height());
		});
	displayed_mat_label_->SetShouldApplyYOffset(true);

	displayed_mat_label_->setPixmap(QPixmap::fromImage(QImage(captured_mat_.data, captured_mat_.cols,
		captured_mat_.rows, QImage::Format_BGR888)).scaled(DIALOG_WIDTH, DIALOG_HEIGHT));

	radio_btn_group_->addButton(jpg_btn_.get());
	radio_btn_group_->addButton(png_btn_.get());
	radio_btn_group_->setExclusive(true);
}


void CameraConfirmationDialog::ConnectEventListeners()
{
	connect(crop_btn_.get(), &QPushButton::clicked, this, [this]()
		{
			is_crop_enabled_ = crop_btn_->isChecked();
		});

	connect(retake_btn_.get(), &QPushButton::clicked, this, [this]()
		{
			if (on_retake_requested_callback_)
			{
				on_retake_requested_callback_();
				close();
			}
		});

	connect(file_dialog_.get(), &QFileDialog::fileSelected, this, [this](const QString& directory)
		{
			QString file_extension = (jpg_btn_->isChecked()) ? ".jpg" : ".png";
			if (!cropped_captured_mat_.empty())
			{
				on_confirm_callback_(cropped_captured_mat_, directory + "/" + image_name_input_->text() + file_extension);
			} else
			{
				on_confirm_callback_(captured_mat_, directory + "/" + image_name_input_->text() + file_extension);
			}
		});

	connect(clear_crop_btn.get(), &QPushButton::clicked, this, [this]()
		{
			displayed_mat_label_->ClearCropLines();
		});

	connect(confirm_btn_.get(), &QPushButton::clicked, this, [this]()
		{
			if (image_name_input_->text().isEmpty())
			{
				MessageBoxHelper::ShowErrorDialog("Image file name cannot be empty");
				return;
			}

			if (!jpg_btn_->isChecked() && !png_btn_->isChecked())
			{
				MessageBoxHelper::ShowErrorDialog("An image file extension was not chosen!");
				return;
			}

			if (on_confirm_callback_)
			{
				file_dialog_->exec();
			}
			close();
		});
}


void CameraConfirmationDialog::SetRetakeCallback(const std::function<void()>& callback)
{
	on_retake_requested_callback_ = callback;
}


void CameraConfirmationDialog::SetConfirmCallback(const std::function<void(Mat, const QString&)>& callback)
{
	on_confirm_callback_ = callback;
}
