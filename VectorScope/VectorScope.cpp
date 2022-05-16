#include "VectorScope.h"


string filename = "D:/color/test/volly.png";



int main(int ac, char** av) {

	src = imread(filename);


	Vectorscope vecScope;

	vecScope.Draw();


	return 0;
}



void DrawVectorscope() {



}

void ShowImg(Mat img) {

	imshow("Image :	 " + filename, img);
	waitKey(0);
}



Vectorscope::Vectorscope()
{
	cols = 512;
	rows = 512;
	margin = 10;

	outLineColor = Scalar(0, 100, 100);
	outLineColor_iq = Scalar(0, 60, 60);
	outLineThick = 1;

	largeThickRatio = 0.99;
	smallThickRatio = 0.95;

}




void Vectorscope::CalcGridPos(double radius, double theta, Point* pos)
{
	int x, y;
	x = int(Vectorscope::centerX) + radius * cos(theta);
	y = int(Vectorscope::centerY) - radius * sin(theta);


	pos->x = x;
	pos->y = y;


}

void Vectorscope::DrawAxis(Mat scope)
{
	//draw Axis
	circle(scope, Point(centerX, centerY), radius, outLineColor, outLineThick + 1, LINE_AA);
	line(scope, Point(centerX - radius, centerY), Point(centerX + radius, centerY), outLineColor, outLineThick + 1);
	line(scope, Point(centerX, centerY - radius), Point(centerX, centerY + radius), outLineColor, outLineThick + 1);


	//draw IQ line
	Point start, end;
	CalcGridPos(radius, rad_iq, &start);
	CalcGridPos(radius, rad_iq + PI, &end);
	line(scope, start, end, outLineColor_iq, outLineThick, LINE_AA);

	CalcGridPos(radius, rad_iq + PI * 0.5, &start);
	CalcGridPos(radius, rad_iq + PI * 1.5, &end);
	line(scope, start, end, outLineColor_iq, outLineThick, LINE_AA);

	//draw Grid lines
	double r_s;
	Point grid_start, grid_end;
	for (int angle = 0; angle < 360; angle += 5) {
		theta = double(angle / 180.0) * PI;
		if (angle % 10 == 10)
			r_s = radius * largeThickRatio;
		else
			r_s = radius * smallThickRatio;
		CalcGridPos(radius, theta, &grid_start);
		CalcGridPos(r_s, theta, &grid_end);
		line(scope, grid_start, grid_end, outLineColor, outLineThick, LINE_AA);
	}

}

void Vectorscope::DrawScope(Mat src, Mat scope)
{
	Mat bgr[3];
	Mat b, g, r;

	Mat temp, img;
	resize(src, img, Size(480, 270));


	split(img, bgr);
	bgr[0].convertTo(b, CV_32FC1);
	bgr[1].convertTo(g, CV_32FC1);
	bgr[2].convertTo(r, CV_32FC1);



	int scale = 2;
	_Cb = Mat(img.size(), CV_32F);
	_Cr = Mat(img.size(), CV_32F);
	_Y = (0.299 * r) + (0.587 * g) + (0.114 * b);
	_Cb = (-0.169 * r) - (0.331 * g) + (0.499 * b) + 128.0 * scale;
	_Cr = (0.499 * r) - (0.418 * g) - (0.0813 * b) + 128.0 * scale;


	_Cr = 256.0 * scale - _Cr;
	//_Cb = 256*scale - _Cb;


	Mat Cb, Cr;
	_Cb.convertTo(Cb, CV_8UC1);
	_Cr.convertTo(Cr, CV_8UC1);



	Point pos;
	Mat scopebgr[3];
	split(scope, scopebgr);
	for (int row = 0; row < img.rows; row++)
	{
		for (int col = 0; col < img.cols; col++)
		{
			pos.y = int(_Cr.at<float>(row, col));
			pos.x = int(_Cb.at<float>(row, col));

			scope.at<Vec3b>(pos) = img.at<Vec3b>(row, col);

		}
	}




}

void Vectorscope::DenoteColor(Mat scope)
{
	colorName = { "B","Cy","G","Y","R","M" };
	colorPosAngle = { -20, -80, -120, -190, -250, -300 };
	fontType = cv::FONT_HERSHEY_DUPLEX;

	float colorAngle;
	Point colorPos;


	for (int i = 0; i < 6; i++) {
		colorAngle = colorPosAngle[i] * PI / 180.0;
		colorPos.x = int(centerX + radius * cos(colorAngle) * 0.7);
		colorPos.y = int(centerY - radius * sin(colorAngle) * 0.7);
		putText(scope, colorName[i], colorPos, fontType, 1, outLineColor, outLineThick, LINE_AA);
	}


}

void Vectorscope::Draw()
{
	centerX = int(cols / 2);
	centerY = int(rows / 2);
	radius = int(rows / 2 - margin);
	rad_iq = 33 * PI / 180;


	scope = Mat::zeros(cols, rows, src.type());

	DrawAxis(scope);
	DenoteColor(scope);
	DrawScope(src, scope);

	imshow("Vectorscope", scope);


	ShowImg(src);
}
