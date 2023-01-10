#include "TAAAAANKS.h"


Bullet::Bullet(int orientation, cv::Point2i& position, int bullet_speed) {
    this->orientation = orientation;
    this->position = position;
    this->bullet_speed = bullet_speed;
}

void Bullet::Pos_UPD() {
    /// Передвижение пули соответственно ориентации
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
    /// Инициализация изображения фона
    map = cv::imread(map_file);
    /// Подгон удобных размеров
    cv::resize(map, map, cv::Size(1000, 1000));
    /// Инициализация модели танка
    init_Tank(tank_file);
    /// Инициализация объекта снаряда
    init_Bullet(bullet_file);
    /// Инициализация скоростей
    this->tank_speed = tank_speed;
    this->bullet_speed = bullet_speed;
}

/*Tank::Tank(const std::string& map_file,
    const std::string& tank_file,
    int speed,
    int balls_speed,
    const std::string& ball_file) {
    /// Инициализация изображения фона
    map = cv::imread(map_file);
    /// Подгон удобных размеров
    cv::resize(map, map, cv::Size(1000, 1000));
    /// Инициализация модели танка
    init_Tank(tank_file);
    /// Инициализация объекта снаряда
    init_Bullet(ball_file);
    /// Инициализация скоростей
    this->tank_speed = tank_speed;
    this->bullet_speed = bullet_speed;
}*/

void Tank::init_Tank(const std::string& tank_model_file) {
    tank_image[0] = cv::imread(tank_model_file);
    /// Подгоняем удобный размер и создаем все 4 ориентации
    cv::resize(tank_image[0], tank_image[0], cv::Size(50, 50));
    cv::rotate(tank_image[0], tank_image[1], cv::ROTATE_90_CLOCKWISE);
    cv::rotate(tank_image[0], tank_image[2], cv::ROTATE_180);
    cv::rotate(tank_image[0], tank_image[3], cv::ROTATE_90_COUNTERCLOCKWISE);
}

void Tank::init_Bullet(const std::string& bullet_model_file) {
    /// Создаем квадратный снаряд (у нас же пиксельная игра)
    if (bullet_model_file == "_") {
        bullet_image[0] = cv::Mat::zeros(cv::Size(30, 30), CV_8UC3);
        cv::rectangle(bullet_image[0], cv::Point2i(0, 0), cv::Point2i(5, 5), cv::Scalar(203, 192, 255), -1);
    }
    /// Или делаем снаряд из картинки 
    else {
        bullet_image[0] = cv::imread(bullet_model_file);
        cv::resize(bullet_image[0], bullet_image[0], cv::Size(50, 50));
    }
    cv::rotate(bullet_image[0], bullet_image[1], cv::ROTATE_90_CLOCKWISE);
    cv::rotate(bullet_image[0], bullet_image[2], cv::ROTATE_180);
    cv::rotate(bullet_image[0], bullet_image[3], cv::ROTATE_90_COUNTERCLOCKWISE);
}

void Tank::start_battle() {
    /// Тут задается начальная позиция танчика
    tank_position = cv::Point2i(map.cols / 2 - tank_image[0].cols / 2,
        map.rows / 2 - tank_image[0].rows / 2);
    while (keyboard_Handler()) {
        /// И в бесконечном цикле проверяем изменения танка и снарядов
        bullet_UPD();
        tank_UPD();
        cv::Mat output = image_return();
        cv::imshow("Ras ras i gotovo", output);
    }
}

bool Tank::keyboard_Handler() {
    int key = cv::waitKey(10);
    switch (key) {
    case 'x': // Закрывает приложение
        return false;
    case -1: // Тут ничего не происходит
        return true;

        /// Будет использована стандартная WASD раскладка
    case 'w': // Путь наверх
        tank_orientation = 0;
        tank_position.y -= tank_speed;
        break;
    case 'a': // Едем влево
        tank_orientation = 3;
        tank_position.x -= tank_speed;
        break;
    case 's': // Катимся вниз
        tank_orientation = 2;
        tank_position.y += tank_speed;
        break;
    case 'd': // Направляемся вправо
        tank_orientation = 1;
        tank_position.x += tank_speed;
        break;
    case 'q': //выстрел
        bullet_Handler();
        break;
    default:
        break;
    }
    return true;
}

void Tank::bullet_Handler() {
    /// Положение снаряда, очевидно, зависит от ориентации снаряда
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
    /// Пополняем вектор новым снарядом
    bullets.emplace_back(tank_orientation, pos, bullet_speed);
}

void Tank::bullet_UPD() {
    /// Положение всех снарядов надо обновить
    for (auto& bullet : bullets)
        bullet.Pos_UPD();
    /// А тут проверяется выход снаряда за границы карты
    for (int i = 0; i != bullets.size(); ++i) {
        if (is_out(map, bullet_image[bullets[i].orientation], bullets[i].position)) {
            bullets.erase(bullets.begin() + i);
            --i;
        }
    }
}

void Tank::tank_UPD() {
    /// Проверка на выход за границы карты
    if (!is_out(map, tank_image[tank_orientation], tank_position))
        return;
    /// Выталкиваем обратно в карту, если вышел за границы
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
    /// Копируем карту
    cv::Mat output = map.clone();
    /// Рисуем на ней танк
    set_tank_on_map(tank_image[tank_orientation], output, tank_position);
    /// Рисуем все выстрелы
    for (auto& bullet : bullets)
        set_tank_on_map(bullet_image[bullet.orientation], output, bullet.position);
    return output;
}

void Tank::set_tank_on_map(cv::Mat& object,
    cv::Mat& map,
    const cv::Point2i& position) {
    /// Цикл перемещения модели на фон
    for (int i = 0; i != object.rows; ++i) {
        for (int j = 0; j != object.cols; ++j) {
            /// Черные пикселим принимаем фоном модели
            if (object.at<cv::Vec3b>(i, j) == cv::Vec3b(0, 0, 0))
                continue;
            map.at<cv::Vec3b>(position.y + i, position.x + j) = object.at<cv::Vec3b>(i, j);
        }
    }
}