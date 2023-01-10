#include "TAAAAANKS.h"


Bullet::Bullet(int orientation, cv::Point2i& position, int bullet_speed) {
    this->orientation = orientation;
    this->position = position;
    this->bullet_speed = bullet_speed;
}

void Bullet::Pos_UPD() {
    /// ������������ ���� �������������� ����������
    switch (orientation) {
    case 0:
        position.y -= bullet_speed;
        break;
    case 1:
        position.x += bullet_speed;
        break;
    case 2:
        position.y += bullet_speed;
        break;
    case 3:
        position.x -= bullet_speed;
        break;
    default:
        break;
    }
}

Tank::Tank(const std::string& map_file,
    const std::string& tank_file,
    const std::string& bullet_file,
    int tank_speed,
    int bullet_speed) {
    /// ������������� ����������� ����
    map = cv::imread(map_file);
    /// ������ ������� ��������
    cv::resize(map, map, cv::Size(1000, 1000));
    /// ������������� ������ �����
    init_Tank(tank_file);
    /// ������������� ������� �������
    init_Bullet(bullet_file);
    /// ������������� ���������
    this->tank_speed = tank_speed;
    this->bullet_speed = bullet_speed;
}

/*Tank::Tank(const std::string& map_file,
    const std::string& tank_file,
    int speed,
    int balls_speed,
    const std::string& ball_file) {
    /// ������������� ����������� ����
    map = cv::imread(map_file);
    /// ������ ������� ��������
    cv::resize(map, map, cv::Size(1000, 1000));
    /// ������������� ������ �����
    init_Tank(tank_file);
    /// ������������� ������� �������
    init_Bullet(ball_file);
    /// ������������� ���������
    this->tank_speed = tank_speed;
    this->bullet_speed = bullet_speed;
}*/

void Tank::init_Tank(const std::string& tank_model_file) {
    tank_image[0] = cv::imread(tank_model_file);
    /// ��������� ������� ������ � ������� ��� 4 ����������
    cv::resize(tank_image[0], tank_image[0], cv::Size(50, 50));
    cv::rotate(tank_image[0], tank_image[1], cv::ROTATE_90_CLOCKWISE);
    cv::rotate(tank_image[0], tank_image[2], cv::ROTATE_180);
    cv::rotate(tank_image[0], tank_image[3], cv::ROTATE_90_COUNTERCLOCKWISE);
}

void Tank::init_Bullet(const std::string& bullet_model_file) {
    /// ������� ���������� ������ (� ��� �� ���������� ����)
    if (bullet_model_file == "_") {
        bullet_image[0] = cv::Mat::zeros(cv::Size(30, 30), CV_8UC3);
        cv::rectangle(bullet_image[0], cv::Point2i(0, 0), cv::Point2i(5, 5), cv::Scalar(203, 192, 255), -1);
    }
    /// ��� ������ ������ �� �������� 
    else {
        bullet_image[0] = cv::imread(bullet_model_file);
        cv::resize(bullet_image[0], bullet_image[0], cv::Size(50, 50));
    }
    cv::rotate(bullet_image[0], bullet_image[1], cv::ROTATE_90_CLOCKWISE);
    cv::rotate(bullet_image[0], bullet_image[2], cv::ROTATE_180);
    cv::rotate(bullet_image[0], bullet_image[3], cv::ROTATE_90_COUNTERCLOCKWISE);
}

void Tank::start_battle() {
    /// ��� �������� ��������� ������� �������
    tank_position = cv::Point2i(map.cols / 2 - tank_image[0].cols / 2,
        map.rows / 2 - tank_image[0].rows / 2);
    while (keyboard_Handler()) {
        /// � � ����������� ����� ��������� ��������� ����� � ��������
        bullet_UPD();
        tank_UPD();
        cv::Mat output = image_return();
        cv::imshow("Ras ras i gotovo", output);
    }
}

bool Tank::keyboard_Handler() {
    int key = cv::waitKey(10);
    switch (key) {
    case 'x': // ��������� ����������
        return false;
    case -1: // ��� ������ �� ����������
        return true;

        /// ����� ������������ ����������� WASD ���������
    case 'w': // ���� ������
        tank_orientation = 0;
        tank_position.y -= tank_speed;
        break;
    case 'a': // ���� �����
        tank_orientation = 3;
        tank_position.x -= tank_speed;
        break;
    case 's': // ������� ����
        tank_orientation = 2;
        tank_position.y += tank_speed;
        break;
    case 'd': // ������������ ������
        tank_orientation = 1;
        tank_position.x += tank_speed;
        break;
    case 'q': //�������
        bullet_Handler();
        break;
    default:
        break;
    }
    return true;
}

void Tank::bullet_Handler() {
    /// ��������� �������, ��������, ������� �� ���������� �������
    cv::Point2i pos;
    switch (tank_orientation) {
    case 0:
        pos = cv::Point2i(tank_position.x + tank_image[tank_orientation].cols / 2 - bullet_image[tank_orientation].cols / 2,
            tank_position.y - bullet_image[tank_orientation].rows);
        break;
    case 1:
        pos = cv::Point2i(tank_position.x + tank_image[tank_orientation].cols,
            tank_position.y + tank_image[tank_orientation].rows / 2 - bullet_image[tank_orientation].rows / 2);
        break;
    case 2:
        pos = cv::Point2i(tank_position.x + tank_image[tank_orientation].cols / 2 - bullet_image[tank_orientation].cols / 2,
            tank_position.y + tank_image[tank_orientation].rows);
        break;
    case 3:
        pos = cv::Point2i(tank_position.x - bullet_image[tank_orientation].cols,
            tank_position.y + tank_image[tank_orientation].rows / 2 - bullet_image[tank_orientation].rows / 2);
        break;
    default:
        break;
    }
    /// ��������� ������ ����� ��������
    bullets.emplace_back(tank_orientation, pos, bullet_speed);
}

void Tank::bullet_UPD() {
    /// ��������� ���� �������� ���� ��������
    for (auto& bullet : bullets)
        bullet.Pos_UPD();
    /// � ��� ����������� ����� ������� �� ������� �����
    for (int i = 0; i != bullets.size(); ++i) {
        if (is_out(map, bullet_image[bullets[i].orientation], bullets[i].position)) {
            bullets.erase(bullets.begin() + i);
            --i;
        }
    }
}

void Tank::tank_UPD() {
    /// �������� �� ����� �� ������� �����
    if (!is_out(map, tank_image[tank_orientation], tank_position))
        return;
    /// ����������� ������� � �����, ���� ����� �� �������
    switch (tank_orientation) {
    case 0:
        tank_position.y += tank_speed;
        break;
    case 1:
        tank_position.x -= tank_speed;
        break;
    case 2:
        tank_position.y -= tank_speed;
        break;
    case 3:
        tank_position.x += tank_speed;
        break;
    default:
        break;
    }
}

bool Tank::is_out(cv::Mat& map, cv::Mat& object, cv::Point2i& position) {
    if (position.x < 0)
        return true;
    if (position.y < 0)
        return true;
    if (position.x + object.cols > map.cols)
        return true;
    if (position.y + object.rows > map.rows)
        return true;
    return false;
}

cv::Mat Tank::image_return() {
    /// �������� �����
    cv::Mat output = map.clone();
    /// ������ �� ��� ����
    set_tank_on_map(tank_image[tank_orientation], output, tank_position);
    /// ������ ��� ��������
    for (auto& bullet : bullets)
        set_tank_on_map(bullet_image[bullet.orientation], output, bullet.position);
    return output;
}

void Tank::set_tank_on_map(cv::Mat& object,
    cv::Mat& map,
    const cv::Point2i& position) {
    /// ���� ����������� ������ �� ���
    for (int i = 0; i != object.rows; ++i) {
        for (int j = 0; j != object.cols; ++j) {
            /// ������ �������� ��������� ����� ������
            if (object.at<cv::Vec3b>(i, j) == cv::Vec3b(0, 0, 0))
                continue;
            map.at<cv::Vec3b>(position.y + i, position.x + j) = object.at<cv::Vec3b>(i, j);
        }
    }
}