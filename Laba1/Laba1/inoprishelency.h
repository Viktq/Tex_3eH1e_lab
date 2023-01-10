#pragma once
#include "opencv2/opencv.hpp"


class Inoprishelency {
public:
    explicit Inoprishelency(const std::string& bkg_file,
        const std::string& model_file,
        const int reshape_width,
        const int reshape_height);

    ~Inoprishelency() = default;

public:
    void fly(int initial_pos, int amplitude, int T);

private:
    bool inoprishelency_Cos(const cv::Mat& bkg,
                            cv::Mat& trajectory_layer, // Две штуки для фона
                            cv::Mat& model,
                            cv::Point2i& position, // Две штуки для модельки
                            int amplitude,
                            float omega); // Две штуки для чисел

    void modelOnBackground(cv::Mat& model,
        cv::Mat& bkg,
        const cv::Point2i& position = cv::Point2i(-1, -1));

private:
    cv::Mat bkg;
    cv::Mat model;
};

