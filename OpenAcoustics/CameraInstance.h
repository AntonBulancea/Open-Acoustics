#pragma once

#include <iostream>
#include <limits>
#include <vector>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;
using namespace std;

class CameraInstance {
private:
	int camNum;
	Mat frame;
	VideoCapture capture;

	Mat CalculateContours(Mat img) {
		Mat canny_output;
		vector<vector<Point> > contours;
		vector<Vec4i> hierarchy;

		Canny(img, canny_output, 50, 150, 3);
		findContours(canny_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

		vector<Moments> mu(contours.size());
		for (int i = 0; i < contours.size(); i++)
		{
			mu[i] = moments(contours[i], false);
		}

		vector<Point2f> mc(contours.size());
		for (int i = 0; i < contours.size(); i++)
		{
			mc[i] = Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);
		}

		Mat drawing(canny_output.size(), CV_8UC3, Scalar(255, 255, 255));
		for (int i = 0; i < contours.size(); i++)
		{
			Scalar color = Scalar(167, 151, 0); // B G R values
			drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
			circle(drawing, mc[i], 4, color, -1, 8, 0);
		}
		return canny_output;
	}

public:
	CameraInstance(int camNum) {
		this->camNum = camNum;
		capture.open(camNum);
	}

	bool UpdateView(bool show) {
		capture >> frame;
		//frame = imread("sample.png");

		// show the resultant image
		if (show) {
			namedWindow("Contours", WINDOW_AUTOSIZE);
			imshow("Contours", CalculateContours(frame));
			return true;
		}

		if (!show || cv::waitKey()) {
			destroyAllWindows();
			cout << "eferget";
			return false;
		}
	}
};