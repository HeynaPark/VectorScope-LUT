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

	Mat temp,img;
	//resize(src, temp, Size(480, 270));


	//temp.convertTo(img, CV_64F);
	//split(img, bgr);
	//b = bgr[0];
	//g = bgr[1];
	//r = bgr[2];

	resize(src, img, Size(480, 270));


	//img.convertTo(img, CV_64F);
	split(img, bgr);
	bgr[0].convertTo(b, CV_64FC1);
	bgr[1].convertTo(g, CV_64FC1);
	bgr[2].convertTo(r, CV_64FC1);



	int scale = 2;
	_Cb = Mat(img.size(), CV_64F);
	_Cr = Mat(img.size(), CV_64F);
	_Y = (0.299 * r) + (0.587 * g) + (0.114 * b);
	_Cb = (-0.169 * r) - (0.331 * g) + (0.499 * b) + 128.0 * scale;
	_Cr = (0.499 * r) - (0.418 * g) - (0.0813 * b) + 128.0 * scale;
	/*_Y = 16.0 + (65.481 * r + 128.553 * g + 24.966 * b);
	_Cb = 128.0  +(- 37.797 * r - 74.203 * g + 112.0 * b);
	_Cr = 128.0  +( + 112.0 * r - 93.786 * g - 18.214 * b);*/

	_Cr = 256.0 * scale - _Cr;
	//_Cb = 256*scale - _Cb;


	Mat Cb, Cr;
	_Cb.convertTo(Cb, CV_8UC1);
	_Cr.convertTo(Cr, CV_8UC1);
	
	
	//imshow("Cb", _Cb);
	//imshow("Cr", _Cr);


	                       
	Point pos;
	Mat scopebgr[3];
	split(scope, scopebgr);
	for (int row = 0; row < img.rows; row++) 
	{
		for (int col = 0; col < img.cols; col++)
		{
			pos.x = int(_Cr.at<float>(row, col));
			pos.y = int(_Cb.at<float>(row, col));

			cout << pos << endl;
		//	scope.push_back(pos)
			//scope.at<Vec3b>(pos) = ((bgr[0].at<uchar>(row, col)) << 3) | ((bgr[1].at<uchar>(row, col)) << 2) | (bgr[2].at<uchar>(row, col)) << 1;
		//	scope.at<Vec3b>(pos) = ((temp.at<Vec3b>(row, col)[0]) << 2) | ((temp.at<Vec3b>(row, col)[1]) << 2) | (temp.at<Vec3b>(row, col)[2]) << 0;
			scope.at<Vec3b>(pos) = img.at<Vec3b>(row, col);
			/*scope.at<Vec3b>(pos.x, pos.y)[0]  = src.at<Vec3b>(row, col)[0];
			scope.at<Vec3b>(pos.x, pos.y)[1]  = src.at<Vec3b>(row, col)[1];
			scope.at<Vec3b>(pos.x, pos.y)[2]  = src.at<Vec3b>(row, col)[2];*/
			//scope.at<uchar>(pos.x, pos.y)[0] = temp.at<uchar>(row, col)[0];

			//cout << to_string(src.at<Vec3b>(row, col)[0]) << endl;
			//cout << to_string(src.at<Vec3b>(row, col)[1]) << endl;
			//scope.at<Vec3b>(pos.x, pos.y) = src.at<Vec3b>(row, col)[0], src.at<Vec3b>(row, col)[1], src.at<Vec3b>(row, col)[2];
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
