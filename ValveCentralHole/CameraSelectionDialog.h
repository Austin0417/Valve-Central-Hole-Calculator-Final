#pragma once

#include "ui_CameraSelectionDialog.h"
#include <QButtonGroup>
#include <QRadioButton>
#include <QDialog>
#include <memory>
#include <functional>
#include <vector>


class CameraSelectionDialog : public QDialog
{
private:
	std::unique_ptr<QButtonGroup> btn_group_;
	std::vector<std::unique_ptr<QRadioButton>> camera_selections_;
	std::function<void(int)> callback_;	// this callback will send the index of the selected camera back to the parent CameraDisplayDialog
	std::unique_ptr<QPushButton> apply_btn_;

	Ui::CameraSelectionDialog ui;
public:
	CameraSelectionDialog(const std::function<void(int)>& callback, QWidget* parent = nullptr);
};

