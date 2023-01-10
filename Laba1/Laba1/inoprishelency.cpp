#include "inoprishelency.h"

Inoprishelency::Inoprishelency(const std::string &background_file,
                               const std::string &model_file,
                               const int reshape_width,
                               const int reshape_height) {
    /// Инициализация фона и тележки
    bkg = cv::imread(background_file);
    model = cv::imread(model_file);
    cv::resize(model, model, cv::Size(reshape_width, reshape_height));
}

void Inoprishelency::fly(int initial_pos, int amplitude, int T) {
    cv::Point2i position(0, initial_pos); //Создаем переменную положения
    cv::Mat trajectory_layer = cv::Mat::zeros(bkg.rows, bkg.cols, CV_8UC3); // Создаем слой с траекторией
    float omega = 2 * 3.14 / T; // Частота косинуса

    while(inoprishelency_Cos(bkg, trajectory_layer, model,  position, amplitude, omega)) {
        trajectory_layer = cv::Mat::zeros(bkg.rows, bkg.cols, CV_8UC3);
    }
}

bool Inoprishelency::inoprishelency_Cos(const cv::Mat &bkg,
                        cv::Mat& trajectory_layer,
                         cv::Mat &model,
                         cv::Point2i &position,
                         int amplitude,
                         float omega) {
    int base_row = position.y;
    // Сканируем фон
    while (position.x != bkg.cols - model.cols / 2) {
        // вычисляеем следующее положение
        cv::Point2i new_position = cv::Point2i(position.x + 1, base_row + amplitude * cos(omega * (position.x + 1)));
        // Проверка границ
        if (new_position.y >= bkg.rows)
            new_position.y = bkg.rows - 1;
        if (new_position.y < 0)
            new_position.y = 0;

        // Создаем линию, по которой двигаемся
        cv::line(trajectory_layer, new_position, position, cv::Scalar(0, 0, 255), 5);
        position = new_position; // Присвоение нового значения переменной позиции объекта
        cv::Mat output = bkg.clone(); // Создаем выходную картинку
        modelOnBackground(trajectory_layer, output); // Рисуем траекторию на картинке 
        modelOnBackground(model, output, position); // Рисуем модель на картинке
        cv::imshow("Inoprishelency", output); // Отображаем выходную картинку
        
        // Закрыть окно по нажатию s (STOP)
        if (cv::waitKey(10) == 's') {
            cv::destroyWindow("Inoprishelency");
            return false;
        }

        // Сохранение изображения при достижении середины
        if (position.x == bkg.cols / 2)
            cv::imwrite("target_img.png", output);
    }
    cv::destroyAllWindows(); // Закрываем окно после достижения правого края
    exit(0); // Завершаем приложение
    return true;
}

void Inoprishelency::modelOnBackground(cv::Mat &model,
                               cv::Mat &bkg,
                               const cv::Point2i &position) {

    // Проверяем размер водели/фона
    if(model.rows > bkg.rows || model.cols > bkg.cols) {
        std::cout << "Model is too big for this background. Try to reshape model further!" << std::endl;
        return;
    }

    // Создаем начала перемещения
    cv::Point2i pos;
    if(position == cv::Point2i(-1,-1))
        pos = cv::Point2i(0, 0);
    else
        pos = cv::Point2i(position.x - model.cols / 2, position.y - model.rows / 2);

    // Не даем выйти модели за границы
    if(pos.x < 0)
        pos.x = 0;
    if(pos.x + model.cols > bkg.cols)
        pos.x = bkg.cols - model.cols;
    if(pos.y < 0)
        pos.y = 0;
    if(pos.y + model.rows > bkg.rows)
        pos.y = bkg.rows - model.rows;

    // Перемещаем модель на фон
    for(int i = 0; i != model.rows; ++i) {
        for(int j = 0; j != model.cols; ++j) {
            // Черные пиксели = фон модели
            if(model.at<cv::Vec3b>(i, j) == cv::Vec3b(0, 0, 0))
                continue;
            bkg.at<cv::Vec3b>(pos.y + i, pos.x + j) = model.at<cv::Vec3b>(i, j);
        }
    }
}