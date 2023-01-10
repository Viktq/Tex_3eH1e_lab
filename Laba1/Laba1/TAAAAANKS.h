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
    /// Конструктор, где можно пропустить задание скоростей и задать модель снаряда
    explicit Tank(const std::string& map_file,
        const std::string& tank_file,
        const std::string& ball_file = "_",
        int speed = 8,
        int balls_speed = 5);

    /// Конструктор, где можно пропустить задание модели снаряда и задать скорости
    explicit Tank(const std::string& map_file,
        const std::string& tank_file,
        int speed = 8,
        int balls_speed = 5,
        const std::string& ball_file = "_");

    ~Tank() = default;
private:
    /// Инициализация для танка и снаряда
    void init_Tank(const std::string& tank_model_file);
    void init_Bullet(const std::string& bullet_model_file);
public:
    /// Функция начала программы
    void start_battle();
private:
    /// Обработка нажатий на клавиатуру
    bool keyboard_Handler();
    /// Обработка параметров выстрела
    void bullet_Handler();
    /// Обновление положений выстрелов и танчика
    void bullet_UPD();
    void tank_UPD();
    /// Проверка на выход за пределы карты
    bool is_out(cv::Mat& map, cv::Mat& object, cv::Point2i& position);
    /// Отрисовка изображения
    cv::Mat image_return();
    /// Отрисовка модели на изображении
    void set_tank_on_map(cv::Mat& map,
        cv::Mat& background,
        const cv::Point2i& position);
private:
    cv::Mat map;                            /// Карта
    std::array<cv::Mat, 4> tank_image;      /// 4 варианта ориентаций модели танка
    cv::Point2i tank_position;              /// Локация танка
    int tank_orientation = 0;               /// Ориентация танка
    int tank_speed;                         /// Скорость танка
private:
    std::array<cv::Mat, 4> bullet_image;    /// 4 варианта ориентаций модели снаряда
    std::vector<Bullet> bullets;            /// Вектор с существующими объектами снарядов
    int bullet_speed;                       /// Скорость снарядов
};
