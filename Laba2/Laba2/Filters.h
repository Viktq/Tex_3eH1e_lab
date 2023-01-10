#pragma once
#include "iostream"
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc.hpp"
#include "time.h"
#include "cmath"

class Filter_mission {
	
public:
    explicit Filter_mission(const char* imageName);//конструктор для инициализации всяких всячин

    ~Filter_mission() = default;

public:
	//int box_filter_my(const char* window_name, int kernel_size); //Бокс фильтр через filter2D
    //Далее window_name будет означать параметр, который просто называет окно с картинкой
    cv::Mat box_filter_builtIn(const char* window_name, int kernel_size); //Встроенный бокс филтр
    cv::Mat gaussian_filter_builtIn(const char* window_name, int kernel_size); //Встроенный фильтр гаусса
    cv::Mat differences(cv::Mat first_img, cv::Mat second_img, const char* window_name); //Функция нахождения различий двух изображений
    cv::Mat unsharp(cv::Mat substracted_img, int sharpness_factor, const char* window_name); //Функция для Unsharp'a
    cv::Mat laplacian_my(const char* window_name); //Лаплассиан - реализован через ядро и filter2D
    void compare_results(cv::Mat img1, cv::Mat img2); //Функция сравнения изображений
    cv::Mat filter_my_test(cv::Mat img,uint32_t size, const char* window_name); //Бокс фильтр через итераторы

private:
    uint32_t get_roi_sum(cv::Mat roi); //Приватная функция нахождения региона интереса

private:
    cv::Mat src; //Сурс картинка
    cv::Mat dst; //Целевая картинка - обычно именно она отрисовывается
    cv::Mat dst_2; //Если одной целевой картинки мало
    cv::Mat kernel; //Свертка
    cv::Point anchor; //Якорь для использования некоторых встроенных функций
    double delta; //То же, что и якорь, но дельта :)
    int ddepth; //Еще одна необходимость
    int kernel_size; //Размер ядра свертки - по дефолту выставлен в 1, но вы все равно не сможете запустить функции не задав свой размер
    const char* imageName; //Поле для названия используемой картинки

};
