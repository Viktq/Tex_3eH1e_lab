#pragma once
#include "iostream"
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgproc/types_c.h"
#include "cmath"

class allababah {
public:
    explicit allababah(const char* number_of_test); //  онструктор класса, в котором присваиваетс€ абсолютно бесполезное поле
    // »наче он не хотел работать
    ~allababah() = default;

public:
    // ‘ункци€ дл€ нахождени€ демократов. ¬биваетс€ настраиваемый порог и названи€ картинок/создаваемых окон
    void find_democracy(int threshold, const char* imageName, const char* window_name);

private:
    const char* number_of_test; // “о самое бесполезное поле

};