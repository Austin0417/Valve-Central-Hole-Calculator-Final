#pragma once
#include <QPushButton>
#include <QFileDialog>
#include <memory>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

using namespace cv;

class SaveBinaryImageButton
{
private:
	QPushButton* save_binary_image_btn_;
	QFileDialog* file_dialog_;
	Mat* binary_mat_;
	QString current_image_filename_;

	void ConnectEventHandlers();
public:
	static QString GenerateSavedBinaryImageFileLocation(const QString& current_filename, const QString& current_file_location);
	SaveBinaryImageButton(QWidget* parent = nullptr);
	SaveBinaryImageButton(int x, int y, QWidget* parent = nullptr);
	SaveBinaryImageButton(int x, int y, int width, int height, QWidget* parent = nullptr);
	void BindBinaryMat(Mat* binary_mat);
	bool SaveBinaryImage(const Mat& binary_mat, const QString& file_location);
	void UpdateCurrentImageFilename(const QString& updated_filename);
	const QPushButton* Button() const;

	~SaveBinaryImageButton();
};

