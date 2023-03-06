#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>


int main() {
    std::string save_path = "C:\\WIP\\Tex3penie\\Laba_5_Clion_2\\Create_marker\\marker2.png";
    cv::Mat marker;
    int id = 35;
    int side_pixels = 300;
    int dict = cv::aruco::DICT_5X5_1000;
    cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::Dictionary::get(dict);
    cv::aruco::drawMarker(dictionary, id, side_pixels, marker);

    cv::imwrite(save_path, marker);
    cv::imshow("marker", marker);
    cv::waitKey(0);
    return 0;
}