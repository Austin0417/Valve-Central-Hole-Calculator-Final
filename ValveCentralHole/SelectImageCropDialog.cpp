#include "SelectImageCropDialog.h"
#include "MessageBoxHelper.h"
#include "BinarizeImageHelper.h"

// Resizes the mat before saving the new cropped image
static void ThreadWorkOnConfirmClick(Mat& mat, int width, int height, const std::function<void()>& on_complete)
{
	cv::resize(mat, mat, Size(width, height));
	if (on_complete)
	{
		on_complete();
	}
}


static bool crop = true;

SelectImageCropDialog::SelectImageCropDialog(QString file, QWidget* parent) : image_file_name_(file), QDialog(parent)
{
	QScreen* screen = QGuiApplication::primaryScreen();
	qDebug() << "Monitor resolution: " << screen->size();

	setFixedWidth(screen->size().width());
	setFixedHeight(screen->size().height());

	InitializeUIElements(screen);
	ConnectEventListeners();
	SetPixmapForLabel();
}


void SelectImageCropDialog::InitializeUIElements(QScreen* screen)
{
	reselect_image_btn_ = std::make_unique<QPushButton>(this);
	reselect_image_btn_->setText("Select Another Image");

	image_label_ = std::make_unique<CameraConfirmLabel>(crop, this);
	image_label_->setFixedWidth(screen->size().width() * 0.70);
	image_label_->setFixedHeight(screen->size().height() * 0.70);
	image_label_->move(screen->size().width() * 0.15, 30);
	image_label_->setStyleSheet("background: black;");
	image_label_->SetCropCallback([this](int x, int y, int width, int height)
		{
			is_crop_translation_complete_ = false;
			SetPixmapForLabel(mat_, x, y, std::abs(width), std::abs(height));
		});

	save_as_new_file_checkbox_ = std::make_unique<QCheckBox>(this);
	save_as_new_file_checkbox_->move(width() * 0.45, static_cast<int>(height() * 0.80));
	save_as_new_file_checkbox_->setFixedHeight(save_as_new_file_checkbox_->height() * 1.2);
	save_as_new_file_checkbox_->setFixedWidth(save_as_new_file_checkbox_->width() * 1.2);
	QLabel* checkbox_caption = new QLabel("Save Cropped Image as New", this);
	checkbox_caption->move(save_as_new_file_checkbox_->pos().x() + 25, save_as_new_file_checkbox_->pos().y() + 6);
	checkbox_caption->setStyleSheet("font-size: 15px;");

	new_image_file_name_ = std::make_unique<QLineEdit>(this);
	new_image_file_name_->setPlaceholderText("New Image File Name...");
	new_image_file_name_->setVisible(false);
	new_image_file_name_->move(save_as_new_file_checkbox_->pos().x() + 20, save_as_new_file_checkbox_->pos().y() + 30);

	reset_btn_ = std::make_unique<QPushButton>("Reset Image to Default", this);
	reset_btn_->move(checkbox_caption->x(), image_label_->pos().y() + image_label_->height() + 15);

	confirm_btn_ = std::make_unique<QPushButton>("Confirm", this);
	confirm_btn_->move((image_label_->x() + image_label_->width()) - confirm_btn_->width(), reset_btn_->y());
	confirm_btn_->setEnabled(false);

	image_file_extension_ = std::make_unique<QButtonGroup>(this);
	jpg_btn_ = std::make_unique<QRadioButton>(".jpg", this);
	png_btn_ = std::make_unique<QRadioButton>(".png", this);
	image_file_extension_->addButton(jpg_btn_.get());
	image_file_extension_->addButton(png_btn_.get());
	image_file_extension_->setExclusive(true);
	jpg_btn_->move(checkbox_caption->x() + checkbox_caption->width() + 50, new_image_file_name_->y());
	png_btn_->move(checkbox_caption->x() + checkbox_caption->width() + 50, new_image_file_name_->y() + 20);
	jpg_btn_->setVisible(false);
	png_btn_->setVisible(false);

	file_dialog_ = std::make_unique<QFileDialog>(this);
	file_dialog_->setFileMode(QFileDialog::Directory);
}


void SelectImageCropDialog::ConnectEventListeners()
{
	connect(reselect_image_btn_.get(), &QPushButton::clicked, this, [this]()
		{
			file_dialog_->setFileMode(QFileDialog::AnyFile);
			is_reselecting_image_ = true;
			file_dialog_->exec();
		});

	connect(save_as_new_file_checkbox_.get(), &QCheckBox::stateChanged, this, [this](int state)
		{
			switch (state)
			{
			case Qt::Unchecked:
			{
				new_image_file_name_->setVisible(false);
				jpg_btn_->setVisible(false);
				png_btn_->setVisible(false);
				break;
			}
			case Qt::Checked:
			{
				new_image_file_name_->setVisible(true);
				jpg_btn_->setVisible(true);
				png_btn_->setVisible(true);
				break;
			}
			}
		});

	connect(this, &SelectImageCropDialog::IsReadyToDisplayPixmap, this, [this](const Mat& mat)
		{
			if (mat.empty())
			{
				QPixmap pixmap(image_file_name_);
				image_label_->setPixmap(pixmap.scaled(image_label_->width(), image_label_->height()));
				confirm_btn_->setEnabled(false);
				return;
			}

			QImage image(mat.data, mat.cols, mat.rows, QImage::Format::Format_BGR888);
			QPixmap pixmap = QPixmap::fromImage(image).scaled(image_label_->width(), image_label_->height());
			image_label_->setPixmap(pixmap);
			image_label_->SetNewOriginalPixmap(pixmap);
			confirm_btn_->setEnabled(true);
		});

	connect(confirm_btn_.get(), &QPushButton::clicked, this, [this]()
		{
			if (save_as_new_file_checkbox_->isChecked())
			{
				if (new_image_file_name_->text().isEmpty() || (!jpg_btn_->isChecked() && !png_btn_->isChecked()))
				{
					MessageBoxHelper::ShowErrorDialog("File name cannot be empty, or a file extension was not chosen!");
					return;
				}

				file_dialog_->setFileMode(QFileDialog::Directory);
				is_reselecting_image_ = false;
				file_dialog_->exec();
			}
			else
			{
				imwrite(image_file_name_.toStdString(), cropped_);
				emit this->ShouldCloseDialog();
			}
		});

	connect(reset_btn_.get(), &QPushButton::clicked, this, [this]()
		{
			SetPixmapForLabel();
		});

	connect(file_dialog_.get(), &QFileDialog::fileSelected, this, [this](const QString& file)
		{
			// Directory select (confirming image save as new)
			if (!is_reselecting_image_)
			{
				qDebug() << "Selected directory: " << file;
				latest_directory_entered_ = file;

				if (new_image_file_name_->text().contains(".jpg") || new_image_file_name_->text().contains(".png"))
				{
					if (imwrite((latest_directory_entered_ + "/" + new_image_file_name_->text()).toStdString(), cropped_))
					{
						this->close();
						return;
					}
				}
				else
				{
					QString file_extension = jpg_btn_->isChecked() ? ".jpg" : ".png";
					if (imwrite((latest_directory_entered_ + "/" + new_image_file_name_->text() + file_extension).toStdString(), cropped_))
					{
						this->close();
						return;
					}
				}
				MessageBoxHelper::ShowErrorDialog("An error occurred while trying to save the cropped image!");
			}
			// File select (reselecting image)
			else
			{
				if (!file.contains(".jpg") && !file.contains(".png"))
				{
					MessageBoxHelper::ShowErrorDialog("Selected file was not a valid image format (.jpg/.png)");
					return;
				}

				// Set the new pixmap and cv::Mat
				image_file_name_ = file;
				SetPixmapForLabel();
			}

		});

	connect(this, &SelectImageCropDialog::ShouldCloseDialog, this, [this]()
		{
			this->close();
		});
}


void SelectImageCropDialog::SetPixmapForLabel()
{
	if (image_file_name_.isEmpty())
	{
		return;
	}
	mat_ = imread(image_file_name_.toStdString());
	emit this->IsReadyToDisplayPixmap(mat_);
}

void SelectImageCropDialog::SetPixmapForLabel(Mat mat, int x, int y, int width, int height)
{
	// This thread will set cropped_ and perform the cropping translation
	auto thread_one = std::async(std::launch::async, [this, mat, x, y, width, height]()
		{
			BinarizeImageHelper::ApplyImageCroppingFromQLabel(mat_, cropped_, x, y, width, height, image_label_->width(), image_label_->height());
			is_crop_translation_complete_ = true;
			cv_.notify_one();
		});

	// This thread will emit the signal when the cropping thread has finished
	auto thread_two = std::async(std::launch::async, [this]()
		{
			{
				std::unique_lock<std::mutex> lock(mutex_);
				cv_.wait(lock, [this]()
					{
						return is_crop_translation_complete_;
					});
			}
			// After cropped_ has been set, we will create a temp mat of cropped_ that has its dimensions resized to match the dimensions of QLabel
			Mat temp = cropped_.clone();
			cv::resize(temp, temp, Size(image_label_->width(), image_label_->height()));

			emit this->IsReadyToDisplayPixmap(temp);
		});
}