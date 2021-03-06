#pragma once


#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/core/cuda.hpp>
#include <opencv2/cudaimgproc.hpp>
#include <opencv2/cudaarithm.hpp>

#define _USE_MATH_DEFINES
#define ALPHA 1.1
#define BIAS 10

#include <cmath>
#include <iostream>
#include <time.h>


using namespace cv;
using namespace std;

const double PI = 3.14159265358979;



class Vectorscope {

private:
	Mat _Y, _Cb, _Cr;

	int cols, rows, margin;
	int centerX, centerY;
	int radius;

	Scalar outLineColor;
	Scalar outLineColor_iq;
	int outLineThick;

	double rad_iq;


	vector<String> colorName;
	vector<int> colorPosAngle;
	int fontType;
	double largeThickRatio;
	double smallThickRatio;
	double theta;


	void CalcGridPos(double radius, double theta, Point* pos);
	void DrawAxis(Mat scope);
	void DrawScope(Mat src, Mat scope);
	void DenoteColor(Mat scope);


	Mat scope;

public:
	Vectorscope();

	void Draw();

};


class ApplyLUT {

	Mat lut;


public:

	void CreateLookup();
	void TestLUTinRGB();
	void TestLUT();
	void TestLUTinGpu();
};



Mat src;
Mat dst;


void ShowImg(Mat img);

