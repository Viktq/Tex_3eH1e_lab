#pragma once
#include "iostream"
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgproc/types_c.h"
#include "cmath"

class allababah {
public:
    explicit allababah(const char* number_of_test); // ����������� ������, � ������� ������������� ��������� ����������� ����
    // ����� �� �� ����� ��������
    ~allababah() = default;

public:
    // ������� ��� ���������� ����������. ��������� ������������� ����� � �������� ��������/����������� ����
    void find_democracy(int threshold, const char* imageName, const char* window_name);

private:
    const char* number_of_test; // �� ����� ����������� ����

};