#include "SaveBinaryImageButton.h"

#include "messageboxhelper.h"


QString SaveBinaryImageButton::GenerateSavedBinaryImageFileLocation(const QString& current_image_filename, const QString& current_file_location)
{
	QList<QString> split_file_location = current_file_location.split("/");
	QList<QString> split_current_image_filename = current_image_filename.split("/");
	QString extracted_filename = split_current_image_filename[split_current_image_filename.size() - 1];
	QList<QString> split_extracted_filename = extracted_filename.split(".");	// 1st element is filename, second element is file extension

	split_file_location.push_back(split_extracted_filename[0] + " - Binary" + "." + split_extracted_filename[1]);
	return split_file_location.join("/");
}


void SaveBinaryImageButton::ConnectEventHandlers()
{
	if (save_binary_image_btn_->parentWidget() != nullptr)
	{
		QWidget* parent_widget = save_binary_image_btn_->parentWidget();
		parent_widget->connect(save_binary_image_btn_, &QPushButton::clicked, save_binary_image_btn_->parentWidget(), [this]()
			{
				// On the button click, it should open a file dialog allowing the user to select a file location to save the binary image
				if (binary_mat_ == nullptr || binary_mat_->empty())
				{
					MessageBoxHelper::ShowErrorDialog("The binary image is currently empty!");
					return;
				}

				file_dialog_->exec();
			});
		parent_widget->connect(file_dialog_, &QFileDialog::fileSelected, parent_widget, [this](const QString& directory)
			{
				if (binary_mat_ != nullptr && !binary_mat_->empty() && !current_image_filename_.isEmpty())
				{
					imwrite(GenerateSavedBinaryImageFileLocation(current_image_filename_, directory).toStdString(), *binary_mat_);
				}
			});
	}
}


SaveBinaryImageButton::SaveBinaryImageButton(QWidget* parent)
{
	save_binary_image_btn_ = new QPushButton("Save Binary Image", parent);
	save_binary_image_btn_->setStyleSheet("background: white;");
	file_dialog_ = new QFileDialog(parent);
	file_dialog_->setFileMode(QFileDialog::Directory);
	ConnectEventHandlers();
}

SaveBinaryImageButton::SaveBinaryImageButton(int x, int y, QWidget* parent)
{
	save_binary_image_btn_ = new QPushButton("Save Binary Image", parent);
	save_binary_image_btn_->move(x, y);
	save_binary_image_btn_->setStyleSheet("background: white;");
	file_dialog_ = new QFileDialog(parent);
	file_dialog_->setFileMode(QFileDialog::Directory);
	ConnectEventHandlers();
}

void SaveBinaryImageButton::UpdateCurrentImageFilename(const QString& updated_filename)
{
	current_image_filename_ = updated_filename;
}

SaveBinaryImageButton::SaveBinaryImageButton(int x, int y, int width, int height, QWidget* parent)
{
	save_binary_image_btn_ = new QPushButton("Save Binary Image", parent);
	save_binary_image_btn_->move(x, y);
	save_binary_image_btn_->setFixedHeight(height);
	save_binary_image_btn_->setFixedWidth(width);
	save_binary_image_btn_->setStyleSheet("background: white;");
	file_dialog_ = new QFileDialog(parent);
	file_dialog_->setFileMode(QFileDialog::Directory);
	ConnectEventHandlers();
}

void SaveBinaryImageButton::BindBinaryMat(Mat* binary_mat)
{
	binary_mat_ = binary_mat;
}


bool SaveBinaryImageButton::SaveBinaryImage(const Mat& binary_mat, const QString& filename)
{
	return imwrite(filename.toStdString(), binary_mat);
}

const QPushButton* SaveBinaryImageButton::Button() const
{
	return save_binary_image_btn_;
}

SaveBinaryImageButton::~SaveBinaryImageButton()
{
	delete save_binary_image_btn_;
	delete file_dialog_;
}