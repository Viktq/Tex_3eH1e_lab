#include "Filters.h"

Filter_mission::Filter_mission(const char* imageName) {
    //Тут конструктор класса. Инициируются все используемые значения и загружается картинка
    kernel_size = 1;
    this->imageName = imageName;
    anchor = cv::Point(-1, -1);
    delta = 0;
    ddepth = -1;
    src = cv::imread(cv::samples::findFile(imageName), cv::IMREAD_GRAYSCALE); //Картинка тут. Еще и в серый перекрашивается
    cv::resize(src, src, cv::Size(500, 500)); //И к человеческому размеру приводится
}

/*int Filter_mission::box_filter_my(const char* window_name, int kernel_size) {

    while (1)
    {
        clock_t start_my = clock();
        kernel = cv::Mat::ones(kernel_size, kernel_size, CV_32F) / (float)(kernel_size * kernel_size);
        cv::filter2D(src, dst, ddepth, kernel, anchor, delta, cv::BORDER_DEFAULT);
        cv::imshow(window_name, dst);
        clock_t end_my = clock();
        double seconds_my = (double)(end_my - start_my) / CLOCKS_PER_SEC;
        printf("My time: %f seconds\n", seconds_my);

        char c = (char)cv::waitKey(500);
        if (c == 27)
        {
            break;
        }
    }
    return EXIT_SUCCESS;
}*/

cv::Mat Filter_mission::box_filter_builtIn(const char* window_name, int kernel_size) {
    //Тут у нас встроенный блюр с подстчетом времени. В бесконечном цикле показывается картинка. Для продолжения работы нажать ESC
    clock_t start_builtIn = clock();
    cv::blur(src, dst, cv::Size(kernel_size, kernel_size));
    clock_t end_builtIn = clock();
    double seconds_builtIn = (double)(end_builtIn - start_builtIn) / CLOCKS_PER_SEC;
    printf("BuiltIn time: %f seconds\n", seconds_builtIn);

    while (1) 
    {
        cv::imshow(window_name, dst);
        char c = (char)cv::waitKey(500);
        if (c == 27)
        {
            return dst;
        }
    }
}

cv::Mat Filter_mission::gaussian_filter_builtIn(const char* window_name, int kernel_size) {
    //Встроенный гауссов фильтр. Также для выхода из бесконечного цикла нажать ESC
    clock_t start_builtIn = clock();
    cv::GaussianBlur(src, dst, cv::Size(kernel_size, kernel_size), 0.0, 0.0, cv::BORDER_DEFAULT);
    clock_t end_builtIn = clock();
    double seconds_builtIn = (double)(end_builtIn - start_builtIn) / CLOCKS_PER_SEC;
    printf("BuiltIn gaussian time: %f seconds\n", seconds_builtIn);

    while (1)
    {
        cv::imshow(window_name, dst);
        char c = (char)cv::waitKey(500);
        if (c == 27)
        {
            return dst;
        }
    }
}

cv::Mat Filter_mission::differences(cv::Mat first_img, cv::Mat second_img, const char* window_name) {
    //Функция для подсчета разницы изображений. В простое вычитание я не верю, так что с помощью substract. Не забываем про ESC
    cv::subtract(first_img, second_img, dst_2);
    while (1)
    {
        //clock_t start_builtIn = clock(); //а время тут - чтобы раскоментить если понадобится
        cv::imshow(window_name, dst_2);
        //clock_t end_builtIn = clock();
        //double seconds_builtIn = (double)(end_builtIn - start_builtIn) / CLOCKS_PER_SEC;
        //printf("BuiltIn gaussian time: %f seconds\n", seconds_builtIn);

        char c = (char)cv::waitKey(500);
        if (c == 27)
        {
            return dst;
        }
    }

}

cv::Mat Filter_mission::unsharp(cv::Mat substracted_img, int sharpness_factor, const char* window_name) {

    //Unsharp - это сначала сглаживание, потом вычитание и затем добавление к исходнику результатов вычитания.
    //Значит, можно сначала сгладить изображения другими функциями, а потом их вычитать и складывать. Тут это и сделано.
    //На вход получаем уже сглаженное изображение, и с каким коэффициентом его добавлять к исходнику - и поехали. ESC присутствует
    cv::subtract(src, substracted_img, dst);
    cv::addWeighted(src, 1, dst, sharpness_factor, 0, dst_2);

    while (1)
    {
        cv::imshow(window_name, dst_2);

        char c = (char)cv::waitKey(500);
        if (c == 27)
        {
            return dst;
        }
    }

}

cv::Mat Filter_mission::laplacian_my(const char* window_name) {

    //Писать что-то через итераторы еще раз мне не хочется. Поэтому использую удобную функцию filter2D
    cv::Mat laplacian_core = (cv::Mat_<double>(3, 3) << 1, 1, 1, 1, -8, 1, 1, 1, 1); //Тут задается ядро фильтра
    cv::filter2D(src, dst, ddepth, laplacian_core, anchor, delta, cv::BORDER_DEFAULT); //Тут использование этой функции
    //Не забываем про ESC
    while (1)
    {
        //clock_t start_my = clock();
        cv::imshow(window_name, dst);
        //clock_t end_my = clock();
        //double seconds_my = (double)(end_my - start_my) / CLOCKS_PER_SEC;
        //printf("My time: %f seconds\n", seconds_my);

        char c = (char)cv::waitKey(500);
        if (c == 27)
        {
            return dst;
        }
    }
}

void Filter_mission::compare_results(cv::Mat img1, cv::Mat img2) {
    //Функция попиксельного сравнения изображений. На вход два изображения, в конце получаем в консоль сообщение о их схожести
    int nRows = img1.rows;
    int nCols = img1.cols;

    uint32_t num_px = nRows * nCols;
    uint32_t num_px_eq = 0;

    uchar* p1, * p2;
    for (uint32_t i = 0; i < nRows; i++) {
        p1 = img1.ptr<uchar>(i);
        p2 = img2.ptr<uchar>(i);

        for (uint32_t j = 0; j < nCols; j++) {
            if (p1[j] == p2[j]) {
                num_px_eq++;
            }
        }
    }

    double acc = ((double)num_px_eq / (double)num_px) * 100.0;
    printf("Accuracy of 2 filters %f percent\n", acc);
}

uint32_t Filter_mission::get_roi_sum(cv::Mat roi) {
    //Приватная функция, чтобы находить регион интереса
    uint32_t roi_sum = 0;
    cv::MatIterator_<uchar> it, end;
    for (it = roi.begin<uchar>(), end = roi.end<uchar>(); it != end; ++it)
        roi_sum += *it;
    return roi_sum;
}

cv::Mat Filter_mission::filter_my_test(cv::Mat img, uint32_t size, const char* window_name) {
    //Бокс фильтр через итераторы... На мой взгляд лучше пользоваться втроенными функциями... Хотя бы функцией filter2D...
    //Потому что там все понятно, а это какой-то черный ящик, чес слово
    clock_t start_my = clock();
    cv::Mat init_img = src.clone();
    uint32_t img_edge = size / 2;

    int nRows = img.rows;
    int nCols = img.cols;

    uchar* p;
    for (int i = 0; i < nRows - size + 1; i++) {
        p = img.ptr<uchar>(i + img_edge);
        for (int j = 0; j < nCols - size + 1; j++) {
            cv::Mat roi = init_img(cv::Rect(j, i, size, size));
            double roi_sum = (double)get_roi_sum(roi) / (size * size);
            roi_sum = round(roi_sum);
            p[j + img_edge] = (uchar)roi_sum;
        }
    }
    clock_t end_my = clock();
    double seconds_my = (double)(end_my - start_my) / CLOCKS_PER_SEC;
    printf("My time: %f seconds\n", seconds_my);
    //Показ картинки до нажатия ESC
    while (1)
    {
        cv::imshow(window_name, img);

        char c = (char)cv::waitKey(500);
        if (c == 27)
        {
            return img;
        }
    }

}