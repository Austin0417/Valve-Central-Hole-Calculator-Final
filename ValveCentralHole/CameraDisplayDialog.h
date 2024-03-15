#pragma once
#include "CameraConfirmationDialog.h"
#include "CameraSelectionDialog.h"
#include <condition_variable>
#include <mutex>
#include <functional>
#include <QDialog>
#include <ui_CameraDisplayDialog.h>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>

using namespace cv;

constexpr int DIALOG_WIDTH = 1200;
constexpr int DIALOG_HEIGHT = 720;

class CameraDisplayDialog : public QDialog
{
	Q_OBJECT
private:
	Ui::CameraDisplayDialog ui;
	std::unique_ptr<QLabel> live_video_display_;
	std::unique_ptr<QPushButton> capture_img_btn_;
	std::unique_ptr<QPushButton> configure_camera_btn_;
	bool should_capture_image_;
	bool stop_camera_ = false;
	bool is_camera_stream_ongoing_ = false;
	std::function<void(Mat, const QString&)> on_camera_complete_;

	std::mutex mutex_;
	std::condition_variable cv_;

	void ConnectEventListeners();
	void StartVideoStream();
	void StartVideoStream(int camera_index);

public:
	CameraDisplayDialog(QWidget* parent = nullptr);
	void SetOnCameraCompleteCallback(const std::function<void(Mat, const QString&)>& callback);
	virtual ~CameraDisplayDialog();

signals:
	void SendImageMatToLabel(QPixmap pixmap);
	void OnCameraOpenFailed();
	void OnImageCaptured(Mat captured_mat);
};

