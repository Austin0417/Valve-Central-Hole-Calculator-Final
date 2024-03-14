#pragma once
#include <QWidget>
#include "CalibrateWidget.h"
#include "ui_MeasureWidget.h"
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QFileDialog>
#include <memory>

namespace Ui {
	class MeasureWidget;
}

class MeasureWidget : public QWidget
{
	Q_OBJECT

public:
	explicit MeasureWidget(QWidget* parent = nullptr);
	~MeasureWidget();
	void RefreshCalibrationFactor();
	int GetThresholdValue() const;
	void SetPreviewMat(Mat preview_mat);

signals:
	void UpdatePreviewMat();
	void OnPreviewMatResizeComplete(const Mat& resized);
	void onAreaCalculationComplete(double valve_area);

private:
	Ui::MeasureWidget* ui;
	int threshold_value_ = 127;
	Mat current_image_mat_;
	Mat binarized_image_preview_mat_;
	bool isCurrentlyShowingPreview = false;

	std::unique_ptr<QLabel> calibration_factor_label_;
	std::unique_ptr<QPushButton> select_valve_image_;
	std::unique_ptr<QPushButton> preview_btn_;
	std::unique_ptr<QPushButton> measure_btn_;
	std::unique_ptr<QLabel> original_image_;
	std::unique_ptr<QLabel> binarized_image_;
	std::unique_ptr<QLabel> calculated_area_label_;
	std::unique_ptr<QSlider> threshold_value_slider_;
	std::unique_ptr<QLabel> threshold_value_display_label_;
	std::unique_ptr<QFileDialog> file_dialog_;

	void InitializeUIElements();
	void ConnectEventListeners();
	void DisplayPreviewMat();
};
