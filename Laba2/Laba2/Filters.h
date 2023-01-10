#pragma once
#include "iostream"
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc.hpp"
#include "time.h"
#include "cmath"

class Filter_mission {
	
public:
    explicit Filter_mission(const char* imageName);//����������� ��� ������������� ������ ������

    ~Filter_mission() = default;

public:
	//int box_filter_my(const char* window_name, int kernel_size); //���� ������ ����� filter2D
    //����� window_name ����� �������� ��������, ������� ������ �������� ���� � ���������
    cv::Mat box_filter_builtIn(const char* window_name, int kernel_size); //���������� ���� �����
    cv::Mat gaussian_filter_builtIn(const char* window_name, int kernel_size); //���������� ������ ������
    cv::Mat differences(cv::Mat first_img, cv::Mat second_img, const char* window_name); //������� ���������� �������� ���� �����������
    cv::Mat unsharp(cv::Mat substracted_img, int sharpness_factor, const char* window_name); //������� ��� Unsharp'a
    cv::Mat laplacian_my(const char* window_name); //���������� - ���������� ����� ���� � filter2D
    void compare_results(cv::Mat img1, cv::Mat img2); //������� ��������� �����������
    cv::Mat filter_my_test(cv::Mat img,uint32_t size, const char* window_name); //���� ������ ����� ���������

private:
    uint32_t get_roi_sum(cv::Mat roi); //��������� ������� ���������� ������� ��������

private:
    cv::Mat src; //���� ��������
    cv::Mat dst; //������� �������� - ������ ������ ��� ��������������
    cv::Mat dst_2; //���� ����� ������� �������� ����
    cv::Mat kernel; //�������
    cv::Point anchor; //����� ��� ������������� ��������� ���������� �������
    double delta; //�� ��, ��� � �����, �� ������ :)
    int ddepth; //��� ���� �������������
    int kernel_size; //������ ���� ������� - �� ������� ��������� � 1, �� �� ��� ����� �� ������� ��������� ������� �� ����� ���� ������
    const char* imageName; //���� ��� �������� ������������ ��������

};
