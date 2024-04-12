#pragma once

#include "CalibrationGaugeParametersDAO.h"
#include "BinarizeImageHelper.h"
#include "ui_CalibrateWidget.h"
#include "ThreadPool.h"
#include "ImageCropDialog.h"
#include "BinaryGaugeLabel.h"
#include "BinaryDetailsLabel.h"
#include "ThresholdValueSpinBox.h"
#include <QCameraDevice>
#include <QComboBox>
#include <QString>
#include <QWidget>
#include <QPushButton>
#include <QFileDialog>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QSlider>
#include <QMouseEvent>
#include <opencv2/core.hpp>
#include <functional>
#include <future>
#include <memory>
#include <mutex>

#include "CalibrateData.h"


constexpr int IMAGE_WIDTH = 610;
constexpr int IMAGE_HEIGHT = 330;

using namespace cv;

QT_BEGIN_NAMESPACE
namespace Ui {
	class CalibrateWidget;
}

QT_END_NAMESPACE


QString GetUnitSuffix(UnitSelection current_unit_selection);

class CalibrateWidget : public QWidget
{
	Q_OBJECT
public:
	CalibrateWidget(const std::unique_ptr<bool>& gauge_helper_tool_flag, QWidget* parent = nullptr);
	static double GetGaugeDiameter();
	int GetThresholdValue() const;
	static double GetCalibrationFactor();
	void SetPreviewMat(Mat preview_mat);
	const ThreadPool& GetWidgetThreadPool() const;
	void ApplyLastSavedParameters();
	void ReceiveAndDisplayCameraImage(Mat mat_from_camera);
	void ReceiveAndDisplayCameraImage(const QString& image_name);
	void SetMirrorCallback(const std::function<void(bool)>& callback);
	void SetCalibrateDataCallback(const std::function<void(const CalibrateData&)>& callback);

	static UnitSelection current_unit_selection_;

signals:
	void UpdatePreviewMat();
	void OnCalibrationComplete(double calibration_factor);
	void ShouldClearHelperGaugeLines();
	void MirrorDrawnLinesToPreview();

private:
	Ui::CalibrateWidget* ui;
	const std::unique_ptr<bool>& gauge_helper_flag_;
	Mat current_image_mat_;
	Mat binarized_preview_image_mat_;
	unsigned long long num_total_pixels_ = 0;
	unsigned long long num_white_pixels_ = 0;

	std::function<void(bool)> mirror_action_callback_;
	std::function<void(const CalibrateData&)> save_calibrate_data_to_file_callback_;

	// Mutex and thread pool for better and faster image processing
	std::mutex mutex_;
	ThreadPool tp_;


	// Calibration Gauge Variables
	CalibrationGaugeParameters gauge_parameters_;
	static double gauge_diameter_;
	int threshold_value_ = 127;
	static double calibration_factor_;
	QString selected_image_filename_;
	bool isCurrentlyShowingPreview = false;

	// Start and End Points for the drawn line in the original input image, if there is a line
	QPoint start_;
	QPoint end_;

	// UI Elements
	std::unique_ptr<QDoubleSpinBox> diameter_input_;
	std::unique_ptr<ThresholdValueSpinBox> threshold_value_spin_box_;
	std::unique_ptr<QComboBox> diameter_unit_selection_;
	std::unique_ptr<QSlider> threshold_input_slider_;
	std::unique_ptr<QComboBox> threshold_mode_combo_box_;
	std::unique_ptr<QCheckBox> is_saline_checkbox_;
	std::unique_ptr<QLabel> threshold_value_label_;
	std::unique_ptr<QFileDialog> file_select_;
	std::unique_ptr<QPushButton> select_file_button_;
	std::unique_ptr<QVBoxLayout> widget_layout_;
	std::unique_ptr<CalibrationGaugeLabel> original_image_;
	std::unique_ptr<BinaryGaugeLabel> binarized_image_;
	std::unique_ptr<QPushButton> preview_btn_;
	std::unique_ptr<QPushButton> calibrate_btn_;
	std::unique_ptr<QLabel> calibration_factor_label_;
	std::unique_ptr<QLabel> threshold_mode_tooltip_label_;
	std::unique_ptr<BinaryDetailsLabel> binary_details_label_;
	std::unique_ptr<QLabel> saline_tooltip_label_;
	std::unique_ptr<QPushButton> clear_lines_btn_;
	std::unique_ptr<QPushButton> crop_image_btn_;
	std::unique_ptr<QPushButton> clear_image_btn_;

	std::unique_ptr<QPushButton> save_binary_btn_;
	std::unique_ptr<QFileDialog> save_binary_dialog_;

	void InitializeUIElements();
	void CheckForLastCalibrationParameters();
	void ConnectEventListeners();
	void DisplaySelectedImage(const QString& filename, bool should_show_binary_immediately = false);
	void DisplaySelectedImage(const Mat& selected_mat, const QImage::Format& image_format, bool should_show_binary_immediately = false);
	void DisplayPreviewMat();
	void DisplayCalibrationFactor();
	void SaveCurrentParametersToDatabase();
	void SetIsCurrentlyShowingPreview(bool status);
	void ClearGaugeLines();
	void RefreshWidgetsWithDatabase(const CalibrationGaugeParameters& parameters);
};

