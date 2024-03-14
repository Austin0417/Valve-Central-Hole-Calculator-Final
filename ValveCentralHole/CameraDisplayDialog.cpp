#include "CameraDisplayDialog.h"
#include "MessageBoxHelper.h"


CameraDisplayDialog::CameraDisplayDialog(QWidget* parent) : QDialog(parent), should_capture_image_(false)
{
	ui.setupUi(this);
	setWindowTitle("Camera");
	setWindowIcon(QIcon("window_icon.jpg"));

	live_video_display_.reset(ui.live_video_display_label);
	capture_img_btn_.reset(ui.capture_img_btn);

	ConnectEventListeners();
	StartVideoStream();
}

void CameraDisplayDialog::SetOnCameraCompleteCallback(const std::function<void(Mat, const QString&)>& callback)
{
	on_camera_complete_ = callback;
}


void CameraDisplayDialog::ConnectEventListeners()
{
	connect(capture_img_btn_.get(), &QPushButton::clicked, this, [this]()
		{
			should_capture_image_ = true;
		});

	connect(this, &CameraDisplayDialog::SendImageMatToLabel, this, [this](QPixmap pixmap)
		{
			live_video_display_->setPixmap(pixmap);
		});

	connect(this, &CameraDisplayDialog::OnCameraOpenFailed, this, [this]()
		{
			MessageBoxHelper::ShowErrorDialog("No camera was detected, please try again");
			this->close();
		});

	connect(this, &CameraDisplayDialog::OnImageCaptured, this, [this](Mat captured_image_mat)
		{
			// Change the width and height of the captured image mat to equal the dialog width and dialog height values
			cv::resize(captured_image_mat, captured_image_mat, Size(DIALOG_WIDTH, DIALOG_HEIGHT));
			CameraConfirmationDialog* confirmation_dialog = new CameraConfirmationDialog(captured_image_mat, this->parentWidget());
			confirmation_dialog->SetRetakeCallback([this]()
				{
					should_capture_image_ = false;
					StartVideoStream();
				});
			confirmation_dialog->SetConfirmCallback([this](Mat confirmed_mat, const QString& image_name)
				{
					if (on_camera_complete_)
					{
						if (!imwrite(image_name.toStdString(), confirmed_mat))
						{
							MessageBoxHelper::ShowErrorDialog("An error occurred while trying to save the image, please try again");
							return;
						}
						on_camera_complete_(confirmed_mat, image_name);
					}
					close();
				});
			confirmation_dialog->exec();
		});

	connect(this, &QDialog::rejected, this, [this]()
		{
			stop_camera_ = true;
		});
}


void CameraDisplayDialog::StartVideoStream()
{
	// Create a worker thread that will continuously read from the camera and display the image for as long as should_capture_image_ is false
	std::thread([this]()
		{
			VideoCapture videoCapture;
			for (int i = 0; i < 6; i++)
			{
				if (videoCapture.open(i, CAP_DSHOW))
				{
					break;
				}
			}

			if (!videoCapture.isOpened())
			{
				emit OnCameraOpenFailed();
				return;
			}

			Mat current_image_frame;
			while (!should_capture_image_ && videoCapture.read(current_image_frame) && !stop_camera_)
			{
				QImage image(current_image_frame.data, current_image_frame.cols, current_image_frame.rows, current_image_frame.step, QImage::Format_BGR888);
				QPixmap pixmap = QPixmap::fromImage(image).scaled(DIALOG_WIDTH, DIALOG_HEIGHT);

				emit SendImageMatToLabel(pixmap.copy());
				std::this_thread::sleep_for(std::chrono::milliseconds(33));
			}

			if (stop_camera_)
			{
				videoCapture.release();
				return;
			}

			if (should_capture_image_)
			{
				emit OnImageCaptured(current_image_frame);
			}
			
		}).detach();
}

CameraDisplayDialog::~CameraDisplayDialog()
{
	stop_camera_ = true;
}