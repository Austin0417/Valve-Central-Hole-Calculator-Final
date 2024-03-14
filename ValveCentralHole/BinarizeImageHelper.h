#pragma once
#include "ThreadPool.h"
#include "CalibrationGaugeParameters.h"
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <atomic>
#include <utility>
#include <future>
#include <mutex>
#include <string>
#include <vector>

#define TEMP_FILENAME "preview_temp.jpg"
constexpr double PI = 3.14159;

using namespace cv;



class BinarizeImageHelper
{
private:
	static std::mutex helper_mutex_;
public:
	BinarizeImageHelper();
	static void BinarizeImage(Mat& input, int threshold_value, ThresholdMode mode = ThresholdMode::STANDARD);
	static std::pair<unsigned long long, unsigned long long> GetNumberOfOnAndOffPixels(Mat& input);
	static std::pair<unsigned long long, unsigned long long> GetNumberOfOnAndOffPixels(Mat& input, ThreadPool& thread_pool);
	static double GetCalibrationGaugeFactor(unsigned long long num_on_pixels, double gauge_diameter);
	static double CalculateValveArea(unsigned long long number_on_pixels, double calibration_factor);
	static void InvertBinaryImage(Mat& mat);
	static double ApplySalineTransformation(double calibration_factor);
	static void ApplyImageCroppingFromQLabel(const Mat& input, Mat& cropped_output, int crop_x, int crop_y, int crop_width, int crop_height, int label_width, int label_height);
};
