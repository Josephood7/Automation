// DieEyeArea.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

Mat cropToPupil(Mat img, int offset) {
    Rect frame = Rect(offset, offset, img.size().width - (offset * 2), img.size().height - (offset * 2));
    Mat crop = img(frame);
    return crop;
}

Mat SizedownGray(Mat img, int scale) {
	Mat img_resized;
	resize(img, img_resized, Size(img.cols / scale, img.rows / scale), 0, 0);
	Mat img_gray;
	cvtColor(img_resized, img_gray, COLOR_BGR2GRAY);
	return img_gray;
}

Mat match(Mat img, Mat match_template, Mat template_img) {
    Mat match;
    matchTemplate(img, match_template, match, TM_CCOEFF_NORMED);
    normalize(match, match_template, 0, 1, NORM_MINMAX, -1, Mat());
    double minVal_iris, maxVal_iris;
    Point minLoc_iris, maxLoc_iris;
    minMaxLoc(match, &minVal_iris, &maxVal_iris, &minLoc_iris, &maxLoc_iris);
    rectangle(img, maxLoc_iris, Point(maxLoc_iris.x + template_img.cols / 2, maxLoc_iris.y + template_img.rows / 2), Scalar(0, 255, 0), 2);
    Mat roi = img(Rect(maxLoc_iris.x, maxLoc_iris.y, template_img.cols / 2, template_img.rows / 2));
    imshow("test", img);
    waitKey(0);
    return roi;
}

tuple<array<double, 3>, Mat> ContourAreaCal(Mat img, double scale, double shrink) {
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(img, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    Mat img_contours = Mat::zeros(img.size(), CV_8UC3);
    array<double, 3> max_area = { 0, 0, 0 }; // area, widht, height
    for (size_t i = 0; i < contours.size(); i++) {
        Scalar color = Scalar(0, 255, 0);
        drawContours(img_contours, contours, (int)i, color, 2, LINE_8, hierarchy, 0);
        double area = contourArea(contours[i]) / (scale * scale);
        Rect rect = boundingRect(contours[i]);
        double w = rect.width / scale;
        double h = rect.height / scale;
        cout << area << ", " << w << ", " << h << endl;

        if (area > max_area[0]) {
            max_area[0] = area;
            max_area[1] = w;
            max_area[2] = h;
        }
    }
    return make_tuple(max_area, img_contours);
}

int main()
{
    // Hyper parameters
    double scale = 10;
    double shrink = 2;
    double um = 0.224; // Apply your own pixel to um ratio
    double um2 = um * um;
    int offset = 15;

    namedWindow("test");
    String img_dir = "images/";
    Mat img = imread(img_dir + "VCSEL.jpg");
    cout << img.size() << endl;

    // resize & convert to grayscale
    Mat img_gray = SizedownGray(img, shrink);
    cout << img_gray.size() << endl;

    // eye template
    Mat eye_img = imread(img_dir + "VCSEL_eye.jpg");
    Mat eye = SizedownGray(eye_img, shrink);

    // eye matching
    Mat roi = match(img_gray, eye, eye_img);

    // iris template
    Mat img_iris = imread(img_dir + "VCSEL_iris.jpg");
    Mat iris = SizedownGray(img_iris, shrink);

    // iris matching
    Mat roi_iris = match(roi, iris, img_iris);

    // resize iris
    Mat roi_iris_resized;
    resize(roi_iris, roi_iris_resized, Size(roi_iris.cols * scale, roi_iris.rows * scale), 0, 0);

    // enhanced masking
    Mat roi_iris_enhanced, roi_iris_masked;
    equalizeHist(roi_iris_resized, roi_iris_enhanced);
    inRange(roi_iris_enhanced, Scalar(180, 180, 180), Scalar(250, 250, 250), roi_iris_masked);
    imshow("test", roi_iris_masked);
    waitKey(0);

    // boundary detection
    Mat roi_iris_contour, roi_iris_contour_ir;
    Canny(roi_iris_masked, roi_iris_contour, 50, 60, 3);
    roi_iris_contour_ir = roi_iris_contour.clone();
    imshow("test", roi_iris_contour);
    waitKey(0);

    // IR contour area
    array<double, 3> max_area_ir;
    tuple<array<double, 3>, Mat> tmp_ir = ContourAreaCal(roi_iris_contour_ir, scale, shrink);
    max_area_ir = get<0>(tmp_ir);
    Mat img_contours_ir = get<1>(tmp_ir);

    cout << "max: area, width, height" << endl << max_area_ir[0] << ", " << max_area_ir[1] << ", " << max_area_ir[2] << endl;
    cout << "max: area, width, height" << endl << max_area_ir[0] * um2 << " um^2" << ", " << max_area_ir[1] * um << " um" << ", " << max_area_ir[2] * um << " um" << endl;
	imshow("test", img_contours_ir);
    waitKey(0);

    // crop to pupil
    Mat crop = cropToPupil(roi_iris_contour, offset);

    // contour area
    array<double, 3> max_area;
    tuple<array<double, 3>, Mat> tmp = ContourAreaCal(crop, scale, shrink);
    max_area = get<0>(tmp);
    Mat img_contours = get<1>(tmp);

    cout << "IR max: area, width, height" << endl << max_area[0] << ", " << max_area[1] << ", " << max_area[2] << endl;
    cout << "IR max: area, width, height" << endl << max_area[0] * um2 << " um^2" << ", " << max_area[1] * um << " um" << ", " << max_area[2] * um << " um" << endl;
    imshow("test", img_contours);
    waitKey(0);

    return 0;
}
