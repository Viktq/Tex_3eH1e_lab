#include "allababah.h"

allababah::allababah(const char* number_of_test) {
	this->number_of_test = number_of_test;
}

// ���� �������� ������������� ���������. ����������� �������� - 228, �� ������� ��� ����� ������.
// �� ���� �� ������ �������� ����� ��� � ����� �������� (���� ��� ����), �� �������� 205
void allababah::find_democracy(int threshold, const char* imageName, const char* window_name) {
	// � ���� ����� ����������� �������� � �� ����������� ������ ������������
	cv::Mat dst;
	cv::Mat processing_img = cv::imread(imageName, cv::IMREAD_GRAYSCALE);
	cv::threshold(processing_img, dst, threshold, 1000, cv::THRESH_BINARY);

	// � ���� ����� ���������������� ���������� � ��������� ������� � �� ������� ��� ������������ ���������� ��
	cv::Mat find_mass_center = dst.clone();
	std::vector<std::vector<cv::Point>> counter;
	cv::findContours(find_mass_center, counter, CV_RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
	int counter_size = counter.size();
	
	// ��� �������� ��������������� ������� � �������, ����� �������� ���� ����� � ����� ���� ��������
	cv::Mat show_img;
	cv::cvtColor(dst, show_img, CV_GRAY2BGR);

	// ����, � ������� ��� ������� ������� ��������� �� � � ���� �������� ������� �����
	for (int i = 0; i < counter_size; i++)
	{
		cv::Moments moments = cv::moments(counter[i]);
		double m00 = moments.m00;
		double m10 = moments.m10;
		double m01 = moments.m01;
		double x_center = m10 / m00;
		double y_center = m01 / m00;		
		cv::circle(show_img, cv::Point(x_center, y_center), 1, cv::Scalar(0, 0, 255), 5);
	}
	
	// ����� ��������
	cv::imshow(window_name, show_img);

}