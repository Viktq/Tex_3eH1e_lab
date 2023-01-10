#include "gk.h"

gk_shapes::gk_shapes(const char* number_of_test) {
	this->number_of_test = number_of_test;
}

void gk_shapes::find_real_gk(const char* imageName, const char* window_name) {
	//Сначала подготовка - подгрузка картинки для поиска и картинки-шаблона. Обе картинки - в серый и найти на них
	//ключи через бинарный поиск
	cv::Mat dst;
	cv::Mat dst_2;
	cv::Mat processing_img = cv::imread(imageName, cv::IMREAD_GRAYSCALE);
	cv::Mat template_gk = cv::imread("gk_tmplt.jpg", cv::IMREAD_GRAYSCALE);
	cv::threshold(processing_img, dst, 230, 1000, cv::THRESH_BINARY_INV);
	cv::threshold(template_gk, dst_2, 230, 1000, cv::THRESH_BINARY);

	//Здесь находятся контура ключей на картинке и контур-шаблон, с которым сравнивается найденный контур
	cv::Mat find_gk_counters = dst.clone();
	cv::Mat find_gk_template = dst_2.clone();
	std::vector<std::vector<cv::Point>> gk_counters;
	std::vector<std::vector<cv::Point>> tamplate_counter;
	cv::findContours(find_gk_counters, gk_counters, CV_RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
	cv::findContours(find_gk_template, tamplate_counter, CV_RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
	int counter_size = gk_counters.size();
	cv::Mat show_img = processing_img.clone();

	//Перекраска, чтобы было нормально видно цветные контура
	cv::cvtColor(show_img, show_img, CV_GRAY2BGR);

	//Переменная для показа схожести контуров
	double counters_identity = 0.0;
	
	//Цикл, в котором будуьт проверены все ключи
	for (int i = 0; i < counter_size; i++)
	{
		//Здесь сравниваются контура
		counters_identity = cv::matchShapes(gk_counters[i], tamplate_counter[0], CV_CONTOURS_MATCH_I2, 0);
		//И те из них, что после сравнение я меньше порога - красятся в зеленый
		if (counters_identity < 0.15)
		{
			cv::polylines(show_img, gk_counters[i], true, cv::Scalar(0, 255, 0), 3);
			continue;
		}
		//А все остальные - в красный
		cv::polylines(show_img, gk_counters[i], true, cv::Scalar(0, 0, 255), 3);
	}

	//Показывается раскрашенное изображение
	cv::imshow(window_name, show_img);

}