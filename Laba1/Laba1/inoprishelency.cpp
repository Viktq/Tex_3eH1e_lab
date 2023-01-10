#include "inoprishelency.h"

Inoprishelency::Inoprishelency(const std::string &background_file,
                               const std::string &model_file,
                               const int reshape_width,
                               const int reshape_height) {
    /// ������������� ���� � �������
    bkg = cv::imread(background_file);
    model = cv::imread(model_file);
    cv::resize(model, model, cv::Size(reshape_width, reshape_height));
}

void Inoprishelency::fly(int initial_pos, int amplitude, int T) {
    cv::Point2i position(0, initial_pos); //������� ���������� ���������
    cv::Mat trajectory_layer = cv::Mat::zeros(bkg.rows, bkg.cols, CV_8UC3); // ������� ���� � �����������
    float omega = 2 * 3.14 / T; // ������� ��������

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
    // ��������� ���
    while (position.x != bkg.cols - model.cols / 2) {
        // ���������� ��������� ���������
        cv::Point2i new_position = cv::Point2i(position.x + 1, base_row + amplitude * cos(omega * (position.x + 1)));
        // �������� ������
        if (new_position.y >= bkg.rows)
            new_position.y = bkg.rows - 1;
        if (new_position.y < 0)
            new_position.y = 0;

        // ������� �����, �� ������� ���������
        cv::line(trajectory_layer, new_position, position, cv::Scalar(0, 0, 255), 5);
        position = new_position; // ���������� ������ �������� ���������� ������� �������
        cv::Mat output = bkg.clone(); // ������� �������� ��������
        modelOnBackground(trajectory_layer, output); // ������ ���������� �� �������� 
        modelOnBackground(model, output, position); // ������ ������ �� ��������
        cv::imshow("Inoprishelency", output); // ���������� �������� ��������
        
        // ������� ���� �� ������� s (STOP)
        if (cv::waitKey(10) == 's') {
            cv::destroyWindow("Inoprishelency");
            return false;
        }

        // ���������� ����������� ��� ���������� ��������
        if (position.x == bkg.cols / 2)
            cv::imwrite("target_img.png", output);
    }
    cv::destroyAllWindows(); // ��������� ���� ����� ���������� ������� ����
    exit(0); // ��������� ����������
    return true;
}

void Inoprishelency::modelOnBackground(cv::Mat &model,
                               cv::Mat &bkg,
                               const cv::Point2i &position) {

    // ��������� ������ ������/����
    if(model.rows > bkg.rows || model.cols > bkg.cols) {
        std::cout << "Model is too big for this background. Try to reshape model further!" << std::endl;
        return;
    }

    // ������� ������ �����������
    cv::Point2i pos;
    if(position == cv::Point2i(-1,-1))
        pos = cv::Point2i(0, 0);
    else
        pos = cv::Point2i(position.x - model.cols / 2, position.y - model.rows / 2);

    // �� ���� ����� ������ �� �������
    if(pos.x < 0)
        pos.x = 0;
    if(pos.x + model.cols > bkg.cols)
        pos.x = bkg.cols - model.cols;
    if(pos.y < 0)
        pos.y = 0;
    if(pos.y + model.rows > bkg.rows)
        pos.y = bkg.rows - model.rows;

    // ���������� ������ �� ���
    for(int i = 0; i != model.rows; ++i) {
        for(int j = 0; j != model.cols; ++j) {
            // ������ ������� = ��� ������
            if(model.at<cv::Vec3b>(i, j) == cv::Vec3b(0, 0, 0))
                continue;
            bkg.at<cv::Vec3b>(pos.y + i, pos.x + j) = model.at<cv::Vec3b>(i, j);
        }
    }
}