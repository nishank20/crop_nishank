#include<iostream>
#include<conio.h>
#include<opencv2\opencv.hpp>
#include<opencv2\highgui.hpp>
#include <opencv2\cvconfig.h>

using namespace cv;
using namespace std;

double alpha; /**< Simple contrast control */
int beta;  /**< Simple brightness control */
int top, bottom, left, right;
int borderType;
char* window_name = "copyMakeBorder Demo";
Mat mat2gray(const Mat& src)
{
	Mat dst;
	normalize(src, dst, 0.0, 1.0, NORM_MINMAX);
	return dst;
}

int main(int argc, char** argv)
{
	Mat img;

	Mat cm_img0, cm_img1, cm_img2;
	Mat tmp;
	Mat a, b, c;
	Mat result, result1, result2, result3;
	Mat tmp1, tmp3;
	Mat Kern;
	Mat tmp2;
	for (int i = 0; i < 3; i++) {
		if (i == 0) {
			tmp1 = imread("C:/Users/NISHANK SINGHAL/Desktop/imagery_HH.tif", 0);
		}
		else if (i == 1) {
			tmp1 = imread("C:/Users/NISHANK SINGHAL/Desktop/imagery_HV.tif", 0);
			if (tmp1.empty())
			{
				cout << "Image cannot be loaded..!!" << endl;
				return -1;
			}
		}
		else {
			tmp1 = imread("C:/Users/NISHANK SINGHAL/Desktop/imagery_VV.tif", 0);
		}
		namedWindow("img_1", WINDOW_AUTOSIZE);
		imshow("img_1", tmp1);
		int kernel_size;   // permitted sizes: 3, 5, 7, 9 etc
		cout << "Select the size of kernel (it should be an odd number from 3 onwards): \n" << endl;
		cin >> kernel_size;
		//choise given to user to chose what he wants dose he wants to select the median filter, Average filter , Lee filter,(optional) identify operator need to be compleated it will detect by what way the result would be perfect.
		// Defining the kernel here
		int selection;
		cout << "Select the type of kernel:\n" << "1. Identity Operator \n2. Median Filter \n3. Average filter \n4. Lee Sigma Filter\n-> ";
		cin >> selection;
		switch (selection) {
		case 1:
			Kern = (Mat_<double>(kernel_size, kernel_size) << .5, 0, .5, 0, -200, 0, .5, 0, .5);
			break;
		case 2:
			Kern = (Mat_<double>(kernel_size, kernel_size) << 1, 1, 1, 1, -460, 1, 1, 1, 1) / (kernel_size * kernel_size);
			break;
		case 3:
			Kern = (Mat_<double>(kernel_size, kernel_size) << 0, 1, 0, 1, -500, 1, 0, 1, 0);
			break;
		case 4:
			Kern = (Mat_<double>(kernel_size, kernel_size) << 0, -1, 0, -1, 255, 0, -1, 0, -1) / (kernel_size * kernel_size);
			break;
		default:
			cerr << "Invalid selection";
			return 1;
			break;
		}
		cout << "my kernel:\n " << Kern << endl;

		// Adding the countour of nulls around the original image, to avoid border problems during convolution
		Mat img_conv = Mat::Mat(img.rows + Kern.rows - 1, img.cols + Kern.cols - 1, CV_64FC3, CV_RGB(0, 0, 0));

		for (int x = 0; x < img.rows; x++) {
			for (int y = 0; y < img.cols; y++) {
				img_conv.at<Vec3d>(x + 1, y + 1)[0] = img.at<Vec3d>(x, y)[0];
				img_conv.at<Vec3d>(x + 1, y + 1)[1] = img.at<Vec3d>(x, y)[1];
				img_conv.at<Vec3d>(x + 1, y + 1)[2] = img.at<Vec3d>(x, y)[2];
			}
		}

		//Performing the convolution
		Mat my_conv = Mat::Mat(tmp1.rows, tmp1.cols, CV_64FC3, CV_RGB(0, 0, 0));
		for (int x = (Kern.rows - 1) / 2; x < img_conv.rows - ((Kern.rows - 1) / 2); x++) {
			for (int y = (Kern.cols - 1) / 2; y < img_conv.cols - ((Kern.cols - 1) / 2); y++) {
				double comp_1 = 0;
				double comp_2 = 0;
				double comp_3 = 0;
				for (int u = -(Kern.rows - 1) / 2; u <= (Kern.rows - 1) / 2; u++) {
					for (int v = -(Kern.cols - 1) / 2; v <= (Kern.cols - 1) / 2; v++) {
						comp_1 = comp_1 + (img_conv.at<Vec3d>(x + u, y + v)[0] * Kern.at<double>(u + ((Kern.rows - 1) / 2), v + ((Kern.cols - 1) / 2)));
						comp_2 = comp_2 + (img_conv.at<Vec3d>(x + u, y + v)[1] * Kern.at<double>(u + ((Kern.rows - 1) / 2), v + ((Kern.cols - 1) / 2)));
						comp_3 = comp_3 + (img_conv.at<Vec3d>(x + u, y + v)[2] * Kern.at<double>(u + ((Kern.rows - 1) / 2), v + ((Kern.cols - 1) / 2)));
					}
				}
				my_conv.at<Vec3d>(x - ((Kern.rows - 1) / 2), y - (Kern.cols - 1) / 2)[0] = comp_1;
				my_conv.at<Vec3d>(x - ((Kern.rows - 1) / 2), y - (Kern.cols - 1) / 2)[1] = comp_2;
				my_conv.at<Vec3d>(x - ((Kern.rows - 1) / 2), y - (Kern.cols - 1) / 2)[2] = comp_3;
			}
		}
		my_conv.convertTo(my_conv, CV_8UC3);
		imshow("convolution - manual", my_conv);


		// Performing the filtering using the opencv funtions
		Mat dst;
		if (i == 0) {

			filter2D(tmp1, a, -1, Kern, Point(-1, -1), 0, BORDER_DEFAULT);
			//dst.convertTo(dst, CV_8UC3);
			// ok, now try different kernel
			a = a - tmp1;
			Size size(500, 800);//the dst image size,e.g.100x100

			a.convertTo(a, CV_8UC3);
			resize(a, a, size);
			//cv::resize(a, a, cv::Size(a.cols * 2, a.rows * 2), INTER_CUBIC);
			a.convertTo(a, CV_8U);
			tmp1.convertTo(tmp1, CV_32FC3);
			imshow("convlution - opencv", a);
			//blur(dst, dst, Size(kernel_size, kernel_size));
			//tmp1.convertTo(tmp1, CV_8UC3);
			//Apply median filter
			//medianBlur(tmp1, tmp1, 15);
			//filter2D(tmp1, dst, -1, Kern, Point(1, 1), 0, BORDER_DEFAULT);
			//		Size size(300, 800);//the dst image size,e.g.100x100
			//manualMedianFiler = manualMedianFiler - tmp1;
			//a = a - manualMedianFiler;

			//convertScaleAbs(dst, tmp1);
			//a = dst - a;
			Mat image1;
			//cv::resize(manualMedianFiler, manualMedianFiler, cv::Size(manualMedianFiler.cols * 1.5, manualMedianFiler.rows * 1.5), 0, 0, CV_INTER_LINEAR);

			//cv::resize(tmp1, tmp1, cv::Size(tmp1.cols * 0.75, tmp1.rows * 0.75), 0, 0, CV_INTER_LINEAR);
			imshow("source_1", tmp1);
			imshow("result_1", a);
			//imshow("median_1", manualMedianFiler);
		}
		else if (i == 1) {
			filter2D(tmp1, b, -1, Kern, Point(-1, -1), 0, BORDER_DEFAULT);
			//dst.convertTo(dst, CV_8UC3);
			b = b - tmp1;
			b.convertTo(b, CV_8U);
			imshow("convlution - opencv", b);
			//blur(dst, dst, Size(kernel_size, kernel_size));
			//tmp1.convertTo(tmp1, CV_8UC3);
			//Apply median filter
			//medianBlur(tmp1, tmp1, 15);
			//filter2D(tmp1, dst, -1, Kern, Point(1, 1), 0, BORDER_DEFAULT);
			Size size(500, 800);//the dst image size,e.g.100x100
			resize(b, b, size);//resize image
							   //cv::resize(tmp1, tmp1, cv::Size(tmp1.cols * 0.75, tmp1.rows * 0.75), 0, 0, CV_INTER_LINEAR);
			imshow("source_2", tmp1);
			imshow("result_2", b);
		}
		else {
			filter2D(tmp1, c, -1, Kern, Point(-1, -1), 0, BORDER_DEFAULT);
			//dst.convertTo(dst, CV_8UC3);


			Mat manualMedianFiler;
			//c = manualMedianFiler - c;
			c = c - tmp1;
			c.convertTo(c, CV_8U);
			imshow("convlution - opencv", c);
			//blur(dst, dst, Size(kernel_size, kernel_size));
			//tmp1.convertTo(tmp1, CV_8UC3);
			//Apply median filter
			//medianBlur(tmp1, tmp1, 15);
			//filter2D(tmp1, dst, -1, Kern, Point(1, 1), 0, BORDER_DEFAULT);

			Size size(500, 800);//the dst image size,e.g.100x100
			resize(c, c, size);//resize image
							   //cv::resize(tmp1, tmp1, cv::Size(tmp1.cols * 0.75, tmp1.rows * 0.75), 0, 0, CV_INTER_LINEAR);

							   //medianBlur(c, dst, 25);
							   //c = c - dst;
			imshow("source_3", tmp1);
			imshow("result_3", c);
		}
		waitKey(0);

		destroyAllWindows(); //destroy all open windows

	}
	vector<Mat> channels;
	Mat g;
	g = Mat::zeros(Size(tmp.rows, tmp.cols), CV_32F);
	//Apply paulidecomposition
	Mat alpha, Beta, Gamma;
	Beta = c + a;
	alpha = c - a;
	Gamma = b;
	namedWindow("img_2", WINDOW_AUTOSIZE);
	imshow("img_2", Beta);
	namedWindow("img_3", WINDOW_AUTOSIZE);
	imshow("img_3", alpha);
	namedWindow("img_4", WINDOW_AUTOSIZE);
	imshow("img_4", Gamma);
	//cout << alpha;
	channels.push_back(Beta);
	channels.push_back(Gamma);
	channels.push_back(alpha);

	merge(channels, cm_img0);
	//cv::resize(cm_img0, cm_img0, cv::Size(cm_img0.cols * 1.5, cm_img0.rows * 1.5), 0, 0, CV_INTER_LINEAR);
	//CvInvoke.Resize(cm_img0,cm_img0,cv::Drawing.Size(100, 100), 0, 0, Inter.Cubic);
	//use info to calculate your roi based on current location of mouse pointer

	//cv::resize(cm_img0, cm_img0, cv::Size(tmp3.cols *2, tmp3.rows *2), INTER_CUBIC);
	namedWindow("img", WINDOW_AUTOSIZE);
	imshow("img", cm_img0);
	waitKey(0);
	destroyAllWindows(); //destroy all open windows
	return 0;
}

