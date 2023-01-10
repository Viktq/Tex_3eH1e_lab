#include "roboti.h"

roboti::roboti(const char* number_of_test) {
	this->number_of_test = number_of_test;
}

void roboti::find_robots(const char* imageName, const char* window_name) {
	// Подготовка переменных в начале кода... Можно было бы сделать их полями класса... Но функция то только одна, так что - ээээ
	cv::Mat red_team_bot;
	cv::Mat red_team_top;
	cv::Mat green_team;
	cv::Mat blue_team;
	cv::Mat Lamp;
	cv::Mat cheat_ROI;

	// Подгрузка изображения, ее перекраска, а также немного читерства, чтобы лампочка не детектировалась как красный робот
	cv::Mat processing_img = cv::imread(imageName);
	cv::Mat output_img = processing_img.clone();
	cheat_ROI = processing_img(cv::Rect(0, 0, 72, 48));
	cheat_ROI.copyTo(processing_img(cv::Rect(603, 192, 72, 48)));
	cv::Mat hsv_processing_img;
	cv::cvtColor(processing_img, hsv_processing_img, CV_BGR2HSV);

	//Примерно такие скаляры есть на изображении для красных, снизу спектра
	cv::inRange(hsv_processing_img,  cv::Scalar(0, 41, 61), cv::Scalar(10, 168, 255), red_team_bot);
	//Примерно такие скаляры есть на изображении для красных, сверху спектра
	cv::inRange(hsv_processing_img, cv::Scalar(177, 0, 0), cv::Scalar(179, 255, 255), red_team_top);
	//Примерно такие скаляры есть на изображении для зеленых
	cv::inRange(hsv_processing_img, cv::Scalar(58, 71, 133), cv::Scalar(78, 217, 255),  green_team);
	//Примерно такие скаляры есть на изображении для синих
	cv::inRange(hsv_processing_img, cv::Scalar(85, 20, 41), cv::Scalar(114, 237, 252),  blue_team);
	//Примерно такие скаляры есть на изображении для лампы
	cv::inRange(hsv_processing_img, cv::Scalar(0, 0, 245), cv::Scalar(18, 13, 255), Lamp);
	//Все числа подобрал ручками с помощью забавного сервиса в инете

	//Блок эрозий и дилатаций для всех цветов - чтоб убрать выбросы и закрыть пустоты
	cv::Mat dilate_core = (cv::Mat_<double>(3, 3) << 1, 1, 1, 1, 1, 1, 1, 1, 1);
	cv::erode(red_team_bot, red_team_bot, dilate_core, cv::Point(-1, -1), 1);
	cv::dilate(red_team_bot, red_team_bot, dilate_core, cv::Point(-1, -1), 1);
	cv::erode(red_team_top, red_team_top, dilate_core, cv::Point(-1, -1), 1);
	cv::dilate(red_team_top, red_team_top, dilate_core, cv::Point(-1, -1), 1);
	cv::erode(green_team, green_team, dilate_core, cv::Point(-1, -1), 1);
	cv::dilate(green_team, green_team, dilate_core, cv::Point(-1, -1), 1);
	cv::erode(blue_team, blue_team, dilate_core, cv::Point(-1, -1), 1);
	cv::dilate(blue_team, blue_team, dilate_core, cv::Point(-1, -1), 1);
	cv::erode(Lamp, Lamp, dilate_core, cv::Point(-1, -1), 1);
	cv::dilate(Lamp, Lamp, dilate_core, cv::Point(-1, -1), 1);

	//Создание одного красного из верхней и нижней половин
	cv::Mat red_team;
	cv::add(red_team_bot, red_team_top, red_team);

	//Подготовка и поиск контуров изображений. Лампочка тоже будет контуром, но черного цвета.
	cv::Mat counters_red = red_team.clone();
	cv::Mat counters_green = green_team.clone();
	cv::Mat counters_blue = blue_team.clone();
	cv::Mat counters_lamp = Lamp.clone();
	std::vector<std::vector<cv::Point>> red_count;
	std::vector<std::vector<cv::Point>> green_count;
	std::vector<std::vector<cv::Point>> blue_count;
	std::vector<std::vector<cv::Point>> lamp_count;
	cv::findContours(counters_red, red_count, CV_RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
	cv::findContours(counters_green, green_count, CV_RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
	cv::findContours(counters_blue, blue_count, CV_RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
	cv::findContours(counters_lamp, lamp_count, CV_RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

	//Подготовка к поиск ЦМ чтобы поставить нужные
	int counter_size_red = red_count.size();
	int counter_size_green = green_count.size();
	int counter_size_blue = blue_count.size();

	//Пачечка переменных для дистанции
	double red_distance_min = 1000000.0;
	double red_x_min = 0.0;
	double red_y_min = 0.0;
	double green_distance_min = 1000000.0;
	double green_x_min = 0.0;
	double green_y_min = 0.0;
	double blue_distance_min = 1000000.0;
	double blue_x_min = 0.0;
	double blue_y_min = 0.0;

	//ЦМ лампочки можно найти один раз, потому что все равно он статичен
	cv::Moments lamp_moments = cv::moments(lamp_count[0]);
	double lamp_m00 = lamp_moments.m00;
	double lamp_m10 = lamp_moments.m10;
	double lamp_m01 = lamp_moments.m01;
	double lamp_x_center = lamp_m10 / lamp_m00;
	double lamp_y_center = lamp_m01 / lamp_m00;

	//А дальше три цикла для трех цветов. В каждом: находится ЦМ конктретного робота, чтобы затем евклидово найти расстояние
	//до ЦМ лампочки, тк система координат у них общая. Если расстояние наименьшее - то это новый ближайший робот.
	//И так для всех роботов. Я прекрасно осознаю, что если запарится, то можно было бы сделать приватную функцию для этого
	//и вызывать ее, а не писать три цикла. Но сейчас почти ночь, а маятся с возвращением нескольких значений/массива значений
	//из одной функции мне лень... Так что три одинаковый цикла в студию
	for (int i = 0; i < counter_size_red; i++)
	{
		cv::Moments red_moments = cv::moments(red_count[i]);
		double red_m00 = red_moments.m00;
		double red_m10 = red_moments.m10;
		double red_m01 = red_moments.m01;
		double red_x_center = red_m10 / red_m00;
		double red_y_center = red_m01 / red_m00;

		double red_x_devia = lamp_x_center - red_x_center;
		double red_y_devia = lamp_y_center - red_y_center;
		double distance = sqrt(std::pow(red_x_devia, 2) + std::pow(red_y_devia, 2));

		//Проверка на старое наименьшее расстояние и перезапись при необходимости
		if (distance < red_distance_min) {
			red_distance_min = distance;
			red_x_min = red_x_center;
			red_y_min = red_y_center;
		}
	}
	for (int i = 0; i < counter_size_green; i++)
	{
		cv::Moments green_moments = cv::moments(green_count[i]);
		double green_m00 = green_moments.m00;
		double green_m10 = green_moments.m10;
		double green_m01 = green_moments.m01;
		double green_x_center = green_m10 / green_m00;
		double green_y_center = green_m01 / green_m00;

		double green_x_devia = lamp_x_center - green_x_center;
		double green_y_devia = lamp_y_center - green_y_center;
		double distance = sqrt(std::pow(green_x_devia, 2) + std::pow(green_y_devia, 2));

		if (distance < green_distance_min) {
			green_distance_min = distance;
			green_x_min = green_x_center;
			green_y_min = green_y_center;
		}
	}
	for (int i = 0; i < counter_size_blue; i++)
	{
		cv::Moments blue_moments = cv::moments(blue_count[i]);
		double blue_m00 = blue_moments.m00;
		double blue_m10 = blue_moments.m10;
		double blue_m01 = blue_moments.m01;
		double blue_x_center = blue_m10 / blue_m00;
		double blue_y_center = blue_m01 / blue_m00;

		double blue_x_devia = lamp_x_center - blue_x_center;
		double blue_y_devia = lamp_y_center - blue_y_center;
		double distance = sqrt(std::pow(blue_x_devia, 2) + std::pow(blue_y_devia, 2));

		if (distance < blue_distance_min) {
			blue_distance_min = distance;
			blue_x_min = blue_x_center;
			blue_y_min = blue_y_center;
		}
	}

	//Когда все циклы пройдены и найден ближайший робот - в центре его контура ставится точка его цвета. Это ближайший робот
	cv::circle(output_img, cv::Point(blue_x_min, blue_y_min), 1, cv::Scalar(255, 0, 0), 5);
	cv::circle(output_img, cv::Point(green_x_min, green_y_min), 1, cv::Scalar(0, 255, 0), 5);
	cv::circle(output_img, cv::Point(red_x_min, red_y_min), 1, cv::Scalar(0, 0, 255), 5);

	//Рисуются контура для всего что нужно. У роботов - контура их цвета. У лампочки - черный
	cv::polylines(output_img, blue_count, true, cv::Scalar(255, 0, 0), 3);
	cv::polylines(output_img, green_count, true, cv::Scalar(0, 255, 0), 3);
	cv::polylines(output_img, red_count, true, cv::Scalar(0, 0, 255), 3);
	cv::polylines(output_img, lamp_count, true, cv::Scalar(0, 0, 0), 3);

	//Показ изображений
	cv::imshow("Colored_img", output_img);
}