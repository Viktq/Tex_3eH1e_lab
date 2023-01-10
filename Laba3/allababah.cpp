#include "allababah.h"

allababah::allababah(const char* number_of_test) {
	this->number_of_test = number_of_test;
}

// Этой функцией детектируются демократы. Оптимальный трешхолд - 228, он убирает все кроме зданий.
// Но если на первой картинке нужно еще и людей показать (если это люди), то передать 205
void allababah::find_democracy(int threshold, const char* imageName, const char* window_name) {
	// В этом блоке считывается картинка и по задаваемому порогу бинаризуется
	cv::Mat dst;
	cv::Mat processing_img = cv::imread(imageName, cv::IMREAD_GRAYSCALE);
	cv::threshold(processing_img, dst, threshold, 1000, cv::THRESH_BINARY);

	// В этом блоке инициализируются полезности и находятся контура с их размера для последующего нахождения ЦМ
	cv::Mat find_mass_center = dst.clone();
	std::vector<std::vector<cv::Point>> counter;
	cv::findContours(find_mass_center, counter, CV_RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
	int counter_size = counter.size();
	
	// Тут картинка перекрашивается обратно в цветную, чтобы рисуемые нами линии и точки были цветными
	cv::Mat show_img;
	cv::cvtColor(dst, show_img, CV_GRAY2BGR);

	// Цикл, в котором для каждого контура находится ЦМ и в него ставится красная точка
	for (int i = 0; i < counter_size; i++)
	{
		cv::Moments moments = cv::moments(counter[i]);
		double m00 = moments.m00;
		double m10 = moments.m10;
		double m01 = moments.m01;
		double x_center = m10 / m00;
		double y_center = m01 / m00;		
		cv::circle(show_img, cv::Point(x_center, y_center), 1, cv::Scalar(0, 0, 255), 5);
	}
	
	// Показ картинки
	cv::imshow(window_name, show_img);

}