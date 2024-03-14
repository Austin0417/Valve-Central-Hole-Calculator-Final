#pragma once
#include <QDialog>
#include <QButtonGroup>
#include <QFileDialog>
#include "CameraConfirmLabel.h"
#include "ui_CameraConfirmation.h"
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <functional>

using namespace cv;

class CameraConfirmationDialog : public QDialog
{
private:
	Ui::CameraConfirmationDialog ui;
	std::unique_ptr<QToolButton> crop_btn_;
	std::unique_ptr<CameraConfirmLabel> displayed_mat_label_;
	std::unique_ptr<QPushButton> retake_btn_;;
	std::unique_ptr<QPushButton> confirm_btn_;
	std::unique_ptr<QPushButton> clear_crop_btn;

	std::unique_ptr<QButtonGroup> radio_btn_group_;
	std::unique_ptr<QRadioButton> jpg_btn_;
	std::unique_ptr<QRadioButton> png_btn_;
	std::unique_ptr<QLineEdit> image_name_input_;
	std::unique_ptr<QFileDialog> file_dialog_;

	std::function<void()> on_retake_requested_callback_;
	std::function<void(Mat, const QString&)> on_confirm_callback_;
	bool is_crop_enabled_ = false;

	Mat captured_mat_;
	Mat cropped_captured_mat_;

	void InitializeUiElements();
	void ConnectEventListeners();

public:
	CameraConfirmationDialog(Mat captured_mat, QWidget* parent = nullptr);
	void SetRetakeCallback(const std::function<void()>& callback);
	void SetConfirmCallback(const std::function<void(Mat, const QString&)>& callback);
};

