#pragma once
#include "iostream"
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgproc/types_c.h"
#include "cmath"

class roboti {
public:
    explicit roboti(const char* number_of_test); // И снова юзлесс конструктор
    ~roboti() = default;

public:
    // Функция поиска роботов
    void find_robots(const char* imageName, const char* window_name);

private:
    const char* number_of_test; // Юзлесс поле 

};