#include "ImportBinaryImageButton.h"

void ImportBinaryImageButton::ConnectEventHandlers()
{
	if (import_binary_image_btn_->parentWidget() != nullptr)
	{
		QWidget* parent_widget = import_binary_image_btn_->parentWidget();
		parent_widget->connect(import_binary_image_btn_, &QPushButton::clicked, parent_widget, [this]()
			{
				file_dialog_->exec();
			});
		parent_widget->connect(file_dialog_, &QFileDialog::fileSelected, parent_widget, [this](const QString& file)
			{
				if (parent_widget_callback_)
				{
					parent_widget_callback_(file);
				}
				if (parent_label_ != nullptr)
				{
					parent_label_->setPixmap(QPixmap(file).scaled(parent_label_->width(), parent_label_->height()));
				}
			});
	}
}


ImportBinaryImageButton::ImportBinaryImageButton(QWidget* parent)
{
	import_binary_image_btn_ = new QPushButton("Import Binary Image", parent);
	import_binary_image_btn_->setStyleSheet("background: white;");
	file_dialog_ = new QFileDialog(parent);
	ConnectEventHandlers();
}

ImportBinaryImageButton::ImportBinaryImageButton(int x, int y, QWidget* parent)
{
	import_binary_image_btn_ = new QPushButton("Import Binary Image", parent);
	import_binary_image_btn_->setStyleSheet("background: white;");
	import_binary_image_btn_->move(x, y);

	file_dialog_ = new QFileDialog(parent);

	ConnectEventHandlers();
}

ImportBinaryImageButton::ImportBinaryImageButton(int x, int y, int width, int height, QWidget* parent)
{
	import_binary_image_btn_ = new QPushButton("Import Binary Image", parent);
	import_binary_image_btn_->setStyleSheet("background: white;");
	import_binary_image_btn_->move(x, y);
	import_binary_image_btn_->setFixedHeight(height);
	import_binary_image_btn_->setFixedWidth(width);

	file_dialog_ = new QFileDialog(parent);

	ConnectEventHandlers();
}


void ImportBinaryImageButton::AttachParentWidgetCallback(const std::function<void(const QString&)>& callback)
{
	parent_widget_callback_ = callback;
}

void ImportBinaryImageButton::AttachToParentLabel(QLabel* parent_label)
{
	parent_label_ = parent_label;
}
