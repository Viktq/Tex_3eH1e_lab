#include "allababah.h"
#include "teplovizor.h"
#include "roboti.h"
#include "gk.h"

int main(int argc, char** argv) {

	//allababah first_test("first");
    // Оптимальный трешхолд - 228, он убирает все кроме зданий. Но при 205 на первой картинке еще и люди (люди ли?) видны
	//first_test.find_democracy(228, "allababah_0.jpg", "First_democrat"); 
    //first_test.find_democracy(228, "allababah_1.jpg", "Second_democrat");
    //first_test.find_democracy(228, "allababah_2.jpg", "Third_democrat");
    //
    //teplovizor second_test("second");
    // Все пороги я подбирал ручками, хотя потом и оказалось, что среди них есть схожие значения
    //second_test.find_democracy_colored(cv::Scalar(0, 110, 110), cv::Scalar(60, 255, 255),
    //    "teplovizor_0.jpg", "First_democrat_colored");
    //second_test.find_democracy_colored(cv::Scalar(0, 0, 0), cv::Scalar(60, 255, 255),
    //    "teplovizor_1.jpg", "Second_democrat_colored");
    //second_test.find_democracy_colored(cv::Scalar(0, 0, 0), cv::Scalar(60, 255, 255),
    //    "teplovizor_2.jpg", "Third_democrat_colored");
    //second_test.find_democracy_colored(cv::Scalar(0, 150, 150), cv::Scalar(40, 255, 255),
    //    "teplovizor_3.png", "Fourth_democrat_colored");
    //second_test.find_democracy_colored(cv::Scalar(0, 0, 0), cv::Scalar(60, 255, 255),
    //    "teplovizor_4.jpg", "Fifth_democrat_colored");

    //roboti third_test("third");
    //third_test.find_robots("roboti_0.jpg", "Find_robots");

    gk_shapes fourth_test("fourth");
    fourth_test.find_real_gk("gk.jpg", "Find_gk");

    //Кнопочка для выключения программы - ESC
    while (1)
    {
        char c = (char)cv::waitKey(500);
        if (c == 27)
        {
            break;
        }
    }

}