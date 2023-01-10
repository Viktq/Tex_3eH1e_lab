#pragma once

#include "opencv2/opencv.hpp"


class Bullet {
public:
    explicit Bullet(int orientation,
        cv::Point2i& position,
        int bullet_speed = 5);

    ~Bullet() = default;
public:
    void Pos_UPD();
public:
    cv::Point2i position;
    int orientation;
    int bullet_speed;
};

class Tank {
public:
    /// �����������, ��� ����� ���������� ������� ��������� � ������ ������ �������
    explicit Tank(const std::string& map_file,
        const std::string& tank_file,
        const std::string& ball_file = "_",
        int speed = 8,
        int balls_speed = 5);

    /// �����������, ��� ����� ���������� ������� ������ ������� � ������ ��������
    explicit Tank(const std::string& map_file,
        const std::string& tank_file,
        int speed = 8,
        int balls_speed = 5,
        const std::string& ball_file = "_");

    ~Tank() = default;
private:
    /// ������������� ��� ����� � �������
    void init_Tank(const std::string& tank_model_file);
    void init_Bullet(const std::string& bullet_model_file);
public:
    /// ������� ������ ���������
    void start_battle();
private:
    /// ��������� ������� �� ����������
    bool keyboard_Handler();
    /// ��������� ���������� ��������
    void bullet_Handler();
    /// ���������� ��������� ��������� � �������
    void bullet_UPD();
    void tank_UPD();
    /// �������� �� ����� �� ������� �����
    bool is_out(cv::Mat& map, cv::Mat& object, cv::Point2i& position);
    /// ��������� �����������
    cv::Mat image_return();
    /// ��������� ������ �� �����������
    void set_tank_on_map(cv::Mat& map,
        cv::Mat& background,
        const cv::Point2i& position);
private:
    cv::Mat map;                            /// �����
    std::array<cv::Mat, 4> tank_image;      /// 4 �������� ���������� ������ �����
    cv::Point2i tank_position;              /// ������� �����
    int tank_orientation = 0;               /// ���������� �����
    int tank_speed;                         /// �������� �����
private:
    std::array<cv::Mat, 4> bullet_image;    /// 4 �������� ���������� ������ �������
    std::vector<Bullet> bullets;            /// ������ � ������������� ��������� ��������
    int bullet_speed;                       /// �������� ��������
};
