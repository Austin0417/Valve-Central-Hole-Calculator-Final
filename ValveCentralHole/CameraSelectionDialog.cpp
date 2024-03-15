#include "CameraSelectionDialog.h"
#include "MessageBoxHelper.h"
#include <QCameraDevice>
#include "qmediadevices.h"

CameraSelectionDialog::CameraSelectionDialog(const std::function<void(int)>& callback, QWidget* parent) : callback_(callback), QDialog(parent)
{
	// Instantiating UI elements
	ui.setupUi(this);

	btn_group_ = std::make_unique<QButtonGroup>(this);
	btn_group_->setExclusive(true);

	apply_btn_.reset(ui.apply_btn);

	// Grab the list of available cameras on the system
	QList<QCameraDevice> cameras = QMediaDevices::videoInputs();

	if (cameras.empty())
	{
		MessageBoxHelper::ShowErrorDialog("No additional cameras were detected!");
		return;
	}

	for (const QCameraDevice& camera : cameras)
	{
		camera_selections_.push_back(std::make_unique<QRadioButton>(camera.description(), this));
	}

	int current_camera_id = 0;
	for (std::unique_ptr<QRadioButton>& radio_btn : camera_selections_)
	{
		radio_btn->move(width() * 0.30, (current_camera_id + 1) * 30);
		btn_group_->addButton(radio_btn.get(), current_camera_id);
		current_camera_id++;
	}

	connect(apply_btn_.get(), &QPushButton::clicked, this, [this]()
		{
			if (callback_)
			{
				callback_(btn_group_->checkedId());
				this->close();
			}
		});
}