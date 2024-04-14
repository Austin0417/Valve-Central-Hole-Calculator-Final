#pragma once
#include <QPushButton>
#include <QFileDialog>
#include <QPixmap>
#include <QLabel>
#include <functional>


class ImportBinaryImageButton
{
private:
	QPushButton* import_binary_image_btn_;
	QLabel* parent_label_;
	QFileDialog* file_dialog_;

	std::function<void(const QString&)> parent_widget_callback_;

	void ConnectEventHandlers();
public:
	ImportBinaryImageButton(QWidget* parent = nullptr);
	ImportBinaryImageButton(int x, int y, QWidget* parent = nullptr);
	ImportBinaryImageButton(int x, int y, int width, int height, QWidget* parent = nullptr);
	void AttachParentWidgetCallback(const std::function<void(const QString&)>& callback);
	void AttachToParentLabel(QLabel* parent_pixmap);

};

