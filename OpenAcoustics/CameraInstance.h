#pragma once

#include <iostream>
#include <limits>
#include <vector>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;

class CameraInstance {
public:
	struct Center
	{
		Mat image;
		float xCoord;
		float yCoord;
	};
private:
	int camNum;
	Mat frame;
	VideoCapture capture;

	Center CalculateContours(Mat img) {
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

		float;
		Point2f point;

		float dist = 0;
		float maxDist = 0;

		float xx=0, yy=0;

		for (int i = 0; i < contours.size(); i++)
		{
			Scalar color = Scalar(167, 151, 0); // B G R values
			drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
			circle(drawing, mc[i], 1, color, -1, 8, 0);

			for (int j = 0; j < contours.size(); j++) {
				float x, y;
				x = +(mc[i].x - mc[j].x);
				y = +(mc[i].y - mc[j].y);

				dist = x * x + y * y;

				if (dist > maxDist) {
					maxDist = dist;
					point = mc[i];

					xx = x;
					yy = y;
				}
			}
		}

		Point2f p = Point2f(point.x-xx/2, point.y-yy/2);
		Scalar color = Scalar(255, 255, 0); // B G R values
		circle(drawing, p, 5, color, -1, 8, 0);

		Center c;
		c.image = drawing;
		c.xCoord = xx;
		c.yCoord = yy;
		return c;
	}
	Center ParseCameraToWorld(float WIDTH, float HEIGHT, float xCoord, float yCoord){
		Center c;
		float x;
		float y;

		x = (xCoord * 1) / WIDTH;
		y = (yCoord * 1) / HEIGHT;

		c.xCoord = abs(x);
		c.yCoord = abs(y);
		 
		return c;
	}

public:

	CameraInstance(int camNum) {
		this->camNum = camNum;
		capture.open(camNum);
		//capture.open("Movement Sample.mp4");
	}
	
	Center UpdateView(bool show) {
		capture >> frame;

		if(frame.empty()){
			capture.open("Movement Sample.mp4");
			capture >> frame;

			//cout << "Camera Down" << endl;
		}

		//frame = imread("sample.png");
		Center c = CalculateContours(frame);

		// show the resultant image
		if (show) {
			namedWindow("Contours", WINDOW_AUTOSIZE);
			imshow("Contours", c.image);
		}
		else{
			destroyAllWindows();
		}
		Center worldCenter = ParseCameraToWorld(frame.rows,frame.cols,c.xCoord,c.yCoord);
		return worldCenter;
	}
};