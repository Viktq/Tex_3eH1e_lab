#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>
#include "aruco_samples_utility.h"
#include "stdio.h"
#include <opencv2/imgproc.hpp>
#include "cmath"
#include "opencv2/barcode.hpp"

int main()
{
    //создание всяких полезных параметров - входного видоса, номер словаря и тд
    cv::VideoCapture inputVideo;
    inputVideo.open(0);
    cv::Mat cameraMatrix, distCoeffs;
    int dict = cv::aruco::DICT_5X5_1000;
    int markerLength = 50;
    std::string cameraParamsFilename = "C:\\WIP\\Tex3penie\\Laba_5_Clion_2\\Cube_creation\\calibration.yml";

    //считывание параметров из файлика
    readCameraParameters(cameraParamsFilename, cameraMatrix, distCoeffs);

    //создание выходного видоса
    cv::VideoWriter outputVideo;
    outputVideo.open("Cube_done.avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 30, cv::Size(640, 480), true);

    // создается система координат
    cv::Mat objPoints(4, 1, CV_32FC3);
    objPoints.ptr<cv::Vec3f>(0)[0] = cv::Vec3f(-markerLength/2.f, markerLength/2.f, 0);
    objPoints.ptr<cv::Vec3f>(0)[1] = cv::Vec3f(markerLength/2.f, markerLength/2.f, 0);
    objPoints.ptr<cv::Vec3f>(0)[2] = cv::Vec3f(markerLength/2.f, -markerLength/2.f, 0);
    objPoints.ptr<cv::Vec3f>(0)[3] = cv::Vec3f(-markerLength/2.f, -markerLength/2.f, 0);

    //точки кубаs
    std::vector<cv::Point3f> points(8);
    points[0] = cv::Point3f(-markerLength / 2, -markerLength / 2, 0); //пол нижняя левая
    points[1] = cv::Point3f(markerLength / 2, -markerLength / 2, 0); //пол нижняя правая
    points[2] = cv::Point3f(markerLength / 2, markerLength / 2, 0); //пол верхняя правая
    points[3] = cv::Point3f(-markerLength / 2, markerLength / 2, 0); //пол верхняя левая
    points[4] = cv::Point3f(-markerLength / 2, -markerLength / 2, markerLength); //потолок нижняя левая
    points[5] = cv::Point3f(markerLength / 2, -markerLength / 2, markerLength); //потолок нижняя правая
    points[6] = cv::Point3f(markerLength / 2, markerLength / 2, markerLength); //потолок верхняя правая
    points[7] = cv::Point3f(-markerLength / 2, markerLength / 2, markerLength); //потолок верхняя левая

    std::vector<cv::Point2f> new_points(8);

    //распознавание маркеров
    cv::Ptr<cv::aruco::DetectorParameters> detectorParams = cv::aruco::DetectorParameters::create();
    cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(dict);
    cv::Mat image, imageCopy;

    //цикл работает пока работает камера
    while (inputVideo.grab()) {

        inputVideo >> image;

        imageCopy = image.clone();

        std::vector<int> ids;
        std::vector<std::vector<cv::Point2f>> corners, rejected;

        cv::aruco::detectMarkers(image, dictionary, corners, ids, detectorParams, rejected);

        //если будет найден хотя бы 1 маркер произойдет магия
        if (ids.size() > 0) {

            cv::aruco::drawDetectedMarkers(imageCopy, corners);

            std::vector<cv::Vec3d> rvecs, tvecs;
            cv::aruco::estimatePoseSingleMarkers(corners, markerLength, cameraMatrix, distCoeffs, rvecs, tvecs);
            // расчитается позиция маркера
            for (int i = 0; i < rvecs.size(); i++) {
                //дальше блок перепроицирования
                cv::projectPoints(points, rvecs[i], tvecs[i], cameraMatrix, distCoeffs, new_points);

                std::vector<cv::Point2i> left_edge(4); //левая грань для прямого расположенного маркера
                left_edge[0] = new_points[0];
                left_edge[1] = new_points[1];
                left_edge[2] = new_points[5];
                left_edge[3] = new_points[4];

                std::vector<cv::Point2i> top_edge(4); //верхняя грань для прямого расположенного маркера
                top_edge[0] = new_points[0];
                top_edge[1] = new_points[3];
                top_edge[2] = new_points[7];
                top_edge[3] = new_points[4];

                std::vector<cv::Point2i> right_edge(4); //правая грань для прямого расположенного маркера
                right_edge[0] = new_points[3];
                right_edge[1] = new_points[2];
                right_edge[2] = new_points[6];
                right_edge[3] = new_points[7];

                std::vector<cv::Point2i> bottom_edge(4); //нижняя грань для прямого расположенного маркера
                bottom_edge[0] = new_points[2];
                bottom_edge[1] = new_points[1];
                bottom_edge[2] = new_points[5];
                bottom_edge[3] = new_points[6];

                std::vector<cv::Point2i> head_edge(4); //грань-крышка для прямого расположенного маркера
                head_edge[0] = new_points[4];
                head_edge[1] = new_points[5];
                head_edge[2] = new_points[6];
                head_edge[3] = new_points[7];

                //подготовка к созданию матрицы поворота
                cv::Mat rmat;
                cv::Rodrigues(rvecs[i], rmat);

                cv::Mat transformations_mat = cv::Mat::zeros(4, 4, CV_64F);
                for(int row = 0; row < 3; row++) {
                    for(int col = 0; col < 3; col++) {
                        transformations_mat.at<double>(row,col) = rmat.at<double>(row,col);
                    }
                }
                //дополнение матрицы поворота матрицей перемещения
                transformations_mat.at<double>(0,3) = tvecs[i][0];
                transformations_mat.at<double>(1,3) = tvecs[i][1];
                transformations_mat.at<double>(2,3) = tvecs[i][2];
                transformations_mat.at<double>(3,3) = 1.0;

                //рассчет расстояния от грани до центра камеры
                std::vector<double> distance_vector;
                for(int dot = 0; dot < 8; dot++){
                    cv::Mat def_mat(4, 1, CV_64F);
                    def_mat.at<double>(0,0) = points[dot].x;
                    def_mat.at<double>(0,1) = points[dot].y;
                    def_mat.at<double>(0,2) = points[dot].z;
                    def_mat.at<double>(0,3) = 1;

                    cv::Mat result_mat = transformations_mat * def_mat;
                    distance_vector.emplace_back(result_mat.at<double>(2,0));
                }

                //полезные штучки для хранения граней и цветов - чтобы не отрисовывать все подряд
                std::map<double, std::vector<cv::Point2i>> map_4_edges;
                std::map<double, cv::Scalar> map_4_colors;
                std::vector<double> distance_2_cam_vec;
                double distance_2_cam;

                //рассчет расстояния грани и ее последующее закидывание в стек
                //левая грань
                distance_2_cam = (distance_vector[0] + distance_vector[1] + distance_vector[5] + distance_vector[4])/4;
                distance_2_cam_vec.push_back(distance_2_cam);
                map_4_edges[distance_2_cam] = left_edge;
                map_4_colors[distance_2_cam] = cv::Scalar(0,0,255);

                //верхняя грань
                distance_2_cam = (distance_vector[0] + distance_vector[3] + distance_vector[7] + distance_vector[4])/4;
                distance_2_cam_vec.push_back(distance_2_cam);
                map_4_edges[distance_2_cam] = top_edge;
                map_4_colors[distance_2_cam] = cv::Scalar(0,255,0);

                //правая грань
                distance_2_cam = (distance_vector[3] + distance_vector[2] + distance_vector[6] + distance_vector[7])/4;
                distance_2_cam_vec.push_back(distance_2_cam);
                map_4_edges[distance_2_cam] = right_edge;
                map_4_colors[distance_2_cam] = cv::Scalar(255,0,0);

                //нижняя грань
                distance_2_cam = (distance_vector[2] + distance_vector[1] + distance_vector[5] + distance_vector[6])/4;
                distance_2_cam_vec.push_back(distance_2_cam);
                map_4_edges[distance_2_cam] = bottom_edge;
                map_4_colors[distance_2_cam] = cv::Scalar(255,0,255);

                //крышка-грань
                distance_2_cam = (distance_vector[4] + distance_vector[5] + distance_vector[6] + distance_vector[7])/4;
                distance_2_cam_vec.push_back(distance_2_cam);
                map_4_edges[distance_2_cam] = head_edge;
                map_4_colors[distance_2_cam] = cv::Scalar(255,255,0);

                //сортировочка стека - так найдутся самые близкие грани
                std::sort(distance_2_cam_vec.begin(), distance_2_cam_vec.end(), std::greater<>());

                //ну и рисуем грани
                for(auto &elem: distance_2_cam_vec) {
                    cv::fillPoly(imageCopy, map_4_edges[elem], map_4_colors[elem]);
                    cv::polylines(imageCopy, map_4_edges[elem], true, cv::Scalar(3,3,3), 1);
                }

            }

        }
        //  показываем видос
        cv::imshow("out", imageCopy);



        char key = (char) cv::waitKey(2);
        if (key == 27)
            break;

        //записываем видос
        outputVideo.write(imageCopy);

    }
}
