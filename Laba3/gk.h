#pragma once
#include "iostream"
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgproc/types_c.h"
#include "cmath"

class gk_shapes {
public:
    explicit gk_shapes(const char* number_of_test); //Ну вы поняли
    ~gk_shapes() = default;

public:
    //Функция поиска ключей. Принимает только картинку и как будет называться окно
    void find_real_gk(const char* imageName, const char* window_name);

private:
    const char* number_of_test; //Ага

};