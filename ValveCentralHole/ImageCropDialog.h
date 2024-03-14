#pragma once
#include <QDialog>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include "CameraConfirmLabel.h"
#include "ui_ImageCropDialog.h"
#include <condition_variable>
#include <future>


using namespace cv;


constexpr int LABEL_WIDTH = 1050;
constexpr int LABEL_HEIGHT = 600;

class ImageCropDialog : public QDialog
{
	Q_OBJECT
private:
	std::unique_ptr<CameraConfirmLabel> input_image_label_;
	std::unique_ptr<QPushButton> ok_btn_;
	QString image_file_name_;
	Mat input_image_;
	Mat cropped_;
	bool is_crop_enabled = true;
	std::function<void(const Mat& mat)> confirm_callback_;

	Ui::ImageCropDialog ui;

public:
	ImageCropDialog(QString image_filename, QWidget* parent = nullptr);
	void SetConfirmCallback(const std::function<void(const Mat& mat)>& callback);

signals:
	void CanSetPixmap(const Mat& mat);
};

