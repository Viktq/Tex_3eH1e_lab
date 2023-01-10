#pragma once
#include "iostream"
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgproc/types_c.h"
#include "cmath"

class teplovizor {
public:
    explicit teplovizor(const char* number_of_test); // ¬се тот же бесполезный конструктор с бесполезным полем
    ~teplovizor() = default;

public:
    //‘ункци€ дл€ поиска раскрашенных демократов. ¬ нее передаютс€ кроме картинки и названи€ окна еще два порога дл€ поиска нужных мест
    void find_democracy_colored(cv::Scalar lower, cv::Scalar upper, const char* imageName, const char* window_name);

private:
    const char* number_of_test; // Ѕесполезное поле

};