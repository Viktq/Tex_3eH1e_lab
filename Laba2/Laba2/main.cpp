#include "Filters.h"


int main(int argc, char** argv)
{
	//Общий совет - для перехода к следующей картинке нажимайте ESC - потому что именно на него забинжен выход из функций

	//В первом блоке у нас применение итераторного бокс фильта самоделкина
	Filter_mission box_filter_my_3("Lenna.png");
	cv::Mat filtered_my_B; //Сюда запишется выход картики
	cv::Mat photo = cv::imread(cv::samples::findFile("Lenna.png"), cv::IMREAD_GRAYSCALE);
	cv::resize(photo, photo, cv::Size(500, 500));
	cv::Mat test_photo = photo.clone(); //Тут в 3 строчки подгружаем тестовую картинку
	//И проходимся боксом (картинка появляется)
	filtered_my_B = box_filter_my_3.filter_my_test(test_photo, 3, "Box_iterared"); 

	//В этом блоке происходит появление картинки от ввстроенного Бокса
	Filter_mission box_filter_BI_3("Lenna.png");
	cv::Mat filtered_B;
	filtered_B = box_filter_BI_3.box_filter_builtIn("Box_builtIn", 3);


	//В этом - их сравнение. Создавал новый экземпляр класса, потому что в старых поля заполнялись и сравнение происходило криво
	Filter_mission box_filter_comp_3("Lenna.png");
	cv::Mat diff;
	box_filter_comp_3.compare_results(filtered_my_B, filtered_B);
	diff = box_filter_comp_3.differences(filtered_my_B, filtered_B, "diff");
		
	//В этом блоке вызывается встроенный Гаусс и его сглаживание. А потом смотрится разница между встроенным Гауссом и Боксом
	Filter_mission box_filter_GI_3("Lenna.png");
	cv::Mat filtered_G;
	filtered_G = box_filter_GI_3.gaussian_filter_builtIn("Gauss_builtIn", 3);
	diff = box_filter_GI_3.differences(filtered_B, filtered_G, "diff_B-G");

	//Отсюда начинаются Unsharp тесты. Множитель я выбрал 2, хотя видно хорошо чета начинается с ~7. А 2, потому что иначе
	//Лаплассовый фильтр соовсем осветляется. Для каждого Аншарп - свой экземпляр класса. Чтобы ничего не поломалось.
	//Первы - Бокс Аншарп.
	Filter_mission unsharp_test_B("Lenna.png");
	cv::Mat filtered_BU;
	cv::Mat unsharp_B;
	filtered_BU = unsharp_test_B.box_filter_builtIn("Box_unsharp", 3);
	unsharp_B = unsharp_test_B.unsharp(filtered_BU, 2, "unsharp_B");

	//Второй - Гаусс Аншарп. И сравнение Гаусса и Бокса
	Filter_mission unsharp_test_G("Lenna.png");
	cv::Mat filtered_GU;
	cv::Mat unsharp_G;
	cv::Mat unsharp_diff;
	filtered_GU = unsharp_test_G.gaussian_filter_builtIn("Gauss_unsharp",3);
	unsharp_G = unsharp_test_G.unsharp(filtered_GU, 2, "unsharp_G");
	unsharp_diff = unsharp_test_G.differences(unsharp_B, unsharp_G, "unsharp_diff");

	//За один блок создал применение фильтра Лапласа и его Аншарп. Картинки создаются поочереди.
	Filter_mission laplace_test("Lenna.png");
	cv::Mat filtered_L;
	cv::Mat unsharp_L;
	filtered_L = laplace_test.laplacian_my("laplace_filter");
	unsharp_L = unsharp_test_G.unsharp(filtered_L, 2, "unsharp_L");

}