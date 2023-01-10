#include "teplovizor.h"

teplovizor::teplovizor(const char* number_of_test) {
	this->number_of_test = number_of_test;
}

void teplovizor::find_democracy_colored(cv::Scalar lower, cv::Scalar upper, const char* imageName, const char* window_name) {
	// В первом блоке подгружается картинка, красится в HSV и находятся контура на ней по варируемым порогам
	cv::Mat dst;
	cv::Mat processing_img = cv::imread(imageName);
	cv::Mat hsv_processing_img;
	cv::cvtColor(processing_img, hsv_processing_img, CV_BGR2HSV);
	cv::inRange(hsv_processing_img, lower, upper, dst);

	// В этом блоке картинку прогоняют через простенькую эрозию-дилатацию для избавления от лишних деталей
	cv::Mat show_img;
	cv::Mat final_img;	
	cv::Mat dilate_core = (cv::Mat_<double>(3, 3) << 1, 1, 1, 1, 1, 1, 1, 1, 1);
	cv::erode(dst, show_img, dilate_core, cv::Point(-1, -1), 1);
	cv::dilate(dst, show_img, dilate_core, cv::Point(-1,-1), 1);

	// Подготовка к поиску ЦМ - нахождение контуров
	cv::Mat find_mass_center = show_img.clone();
	std::vector<std::vector<cv::Point>> counter;
	cv::findContours(find_mass_center, counter, CV_RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
	int counter_size = counter.size();

	// Перекрас в BGR, чтобы можно было рисовать цветные точки
	cv::cvtColor(show_img, final_img, CV_GRAY2BGR);

	// Цикл, находящий ЦМ и ставящий точку в него. Для каждого контура - своя точка
	for (int i = 0; i < counter_size; i++)
	{
		cv::Moments moments = cv::moments(counter[i]);
		double m00 = moments.m00;
		double m10 = moments.m10;
		double m01 = moments.m01;
		double x_center = m10 / m00;
		double y_center = m01 / m00;
		cv::circle(final_img, cv::Point(x_center, y_center), 1, cv::Scalar(0, 0, 255), 5);
	}
	
	// Показ картинки
	cv::imshow(window_name, final_img);

}