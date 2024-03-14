#include "BinarizeImageHelper.h"

static void ThreadWorkCountOnOffPixels(Mat& input, int start_row, int end_row, std::atomic<int>& num_on_pixels,
	std::atomic<int>& num_off_pixels, std::atomic<int>& num_threads_finished, std::condition_variable& cv)
{
	int local_num_on_pixels = 0;
	int local_num_off_pixels = 0;

	for (int i = start_row; i < end_row; i++)
	{
		unsigned char* current_row = input.ptr(i);
		for (int j = 0; j < input.cols * input.channels(); j++)
		{
			switch (current_row[j])
			{
			case 0:
			{
				local_num_off_pixels++;
				break;
			}
			case 255:
			{
				local_num_on_pixels++;
				break;
			}
			}
		}
	}

	num_on_pixels += local_num_on_pixels;
	num_off_pixels += local_num_off_pixels;
	num_threads_finished++;
	cv.notify_one();
}


std::mutex BinarizeImageHelper::helper_mutex_;

BinarizeImageHelper::BinarizeImageHelper() {}



/**
 * \brief
 * \param input [in] : grayscale input matrix to binarize
 * \param threshold_value [in] : threshold value ranging from 0 - 255. In ThresholdMode::STANDARD, pixels with intensities above the threshold value will be white, and below will be off.
 * \param mode [in] : Input to determine thresholding mode. In inverted mode, pixel values above the threshold value will be off, and on if above (opposite of standard mode)
 */
void BinarizeImageHelper::BinarizeImage(Mat& input, int threshold_value, ThresholdMode mode) {
	for (int i = 0; i < input.rows; i++) {
		unsigned char* current_row = input.ptr(i);
		for (int j = 0; j < input.cols * input.channels(); j++) {
			switch (mode) {
			case ThresholdMode::STANDARD: {
				if (current_row[j] < threshold_value) {
					current_row[j] = 0;
				}
				else {
					current_row[j] = 255;
				}
			}
			case ThresholdMode::INVERTED: {
				if (current_row[j] < threshold_value) {
					current_row[j] = 255;
				}
				else {
					current_row[j] = 0;
				}
			}
			}
		}
	}

	// Fill holes
	int dilation_size = 1;
	Mat element = getStructuringElement(MORPH_RECT, Size(2 * dilation_size + 1, 2 * dilation_size + 1));
	morphologyEx(input, input, MORPH_OPEN, element);
}

/**
 * \brief
 * \param input [in] :  Binary input matrix that we want to count the number of on and off pixels of (has to be a binary image)
 * \return : std::pair where the first value is the number of on pixels, and second is the number of off pixels
 */
std::pair<unsigned long long, unsigned long long> BinarizeImageHelper::GetNumberOfOnAndOffPixels(Mat& input) {
	std::pair<unsigned long long, unsigned long long> result;
	unsigned long long on_pixels = 0;
	unsigned long long off_pixels = 0;

	for (int i = 0; i < input.rows; i++) {
		unsigned char* current_row = input.ptr(i);
		for (int j = 0; j < input.cols * input.channels(); j++) {
			switch (current_row[j]) {
			case 0: {
				off_pixels++;
				break;
			}
			case 255: {
				on_pixels++;
				break;
			}
			}
		}
	}
	return std::make_pair(on_pixels, off_pixels);
	return result;
}


std::pair<unsigned long long, unsigned long long> BinarizeImageHelper::GetNumberOfOnAndOffPixels(Mat& input, ThreadPool& thread_pool)
{
	int thread_interval_remainder = input.rows % thread_pool.getNumThreads();
	int thread_interval_length = input.rows / thread_pool.getNumThreads();
	int num_intervals = input.rows / thread_interval_length;

	std::atomic<int> num_on_pixels = 0;
	std::atomic<int> num_off_pixels = 0;

	std::atomic<int> num_threads_finished = 0;
	std::condition_variable cv;

	if (thread_interval_remainder == 0)
	{
		for (int i = 0; i < num_intervals; i++)
		{
			thread_pool.enqueue([&input, i, thread_interval_length, &num_on_pixels, &num_off_pixels, &num_threads_finished, &cv]()
				{
					ThreadWorkCountOnOffPixels(input, i * thread_interval_length, (i + 1) * thread_interval_length,
					num_on_pixels, num_off_pixels, num_threads_finished, cv);
				});
		}
	}
	else
	{
		for (int i = 0; i < num_intervals; i++)
		{
			if (i == num_intervals - 1)
			{
				thread_pool.enqueue([&]()
					{
						ThreadWorkCountOnOffPixels(input, i * thread_interval_length,
						(i + 1) * thread_interval_length + thread_interval_remainder, num_on_pixels, num_off_pixels, num_threads_finished, cv);
					});
				break;
			}
			thread_pool.enqueue([&]()
				{
					ThreadWorkCountOnOffPixels(input, i * thread_interval_length, (i + 1) * thread_interval_length,
					num_on_pixels, num_off_pixels, num_threads_finished, cv);
				});
		}
	}

	{
		std::unique_lock<std::mutex> lock(helper_mutex_);
		cv.wait(lock, [&]()
			{
				return num_threads_finished.load() >= thread_pool.getNumThreads();
			});
	}
	return std::make_pair(num_on_pixels.load(), num_off_pixels.load());
}


/**
 * \brief
 * \param num_on_pixels [in] : total number of on pixels within the binary calibration gauge image
 * \param gauge_diameter [in] : gauge diameter of the calibration gauge
 * \return : the calibration gauge factor (how much area each pixel is in mm^2)
 */
double BinarizeImageHelper::GetCalibrationGaugeFactor(unsigned long long num_on_pixels, double gauge_diameter)
{
	double actual_gauge_area = PI * gauge_diameter * gauge_diameter * 0.25;
	return actual_gauge_area / num_on_pixels;
}

double BinarizeImageHelper::CalculateValveArea(unsigned long long number_on_pixels, double calibration_factor)
{
	return number_on_pixels * calibration_factor;
}

void BinarizeImageHelper::InvertBinaryImage(Mat& mat)
{
	bitwise_xor(mat, Scalar(255), mat);
}


double BinarizeImageHelper::ApplySalineTransformation(double calibration_factor)
{
	// TODO Figure out how to adjust for the saline index of refraction here


	return calibration_factor;
}


void BinarizeImageHelper::ApplyImageCroppingFromQLabel(const Mat& input, Mat& cropped_output, int crop_x, int crop_y, int crop_width, int crop_height, int label_width, int label_height)
{
	// Idea is to get the percentage x, y, width, and height value for the cropped region compared to the QLabel's total width and height
	// We can then use these percentages to translate the cropped region from the scaled pixmap to the unscaled input mat image, and store the result in cropped_output

	if (input.empty())
	{
		return;
	}

	double percent_x = static_cast<double>(crop_x) / static_cast<double>(label_width);
	double percent_y = static_cast<double>(crop_y) / static_cast<double>(label_height);
	double percent_width = static_cast<double>((crop_x + crop_width)) / static_cast<double>(label_width);
	double percent_height = static_cast<double>((crop_y + crop_height)) / static_cast<double>(label_height);

	int cropped_x = input.cols * percent_x;
	int cropped_y = input.rows * percent_y;
	int cropped_width = input.cols * percent_width;
	int cropped_height = input.rows * percent_height;
	cropped_output = input(Rect(cropped_x, cropped_y, cropped_width - cropped_x, cropped_height - cropped_y));

}
