#ifndef ANPR_MAIN         
#define ANPR_MAIN

#include <map>
#include <string>
#include <iostream>

//#include <opencv/cv.h>
//#include <opencv/cxcore.h>
//#include <opencv/highgui.h>
#include <opencv2/core/core.hpp>           
#include <opencv2/highgui/highgui.hpp>     
#include <opencv2/imgproc/imgproc.hpp>     
#include <opencv2/ml/ml.hpp>

using namespace std;
using namespace cv;
using namespace ml;

class Contour_Class {
public:

	std::vector<cv::Point> ptContour;
	cv::Rect boundingRect;
	int ContourSize = ptContour.size();

	static bool Sort_Contour(const Contour_Class& cwdLeft, const Contour_Class& cwdRight)
	{
		return(cwdLeft.boundingRect.x < cwdRight.boundingRect.x);
	}

	static bool Sort_Contour_Size(const Contour_Class& cwdLeft, const Contour_Class& cwdRight)
	{
		return((cwdLeft.boundingRect.width * cwdLeft.boundingRect.height) >
			(cwdRight.boundingRect.width * cwdRight.boundingRect.height));
	}

};

// possible chars we are interested in are digits 0 through 9 and capital letters A through Z, put these in vector intValidChars
vector<int> intValidChars = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
'U', 'V', 'W', 'X', 'Y', 'Z' };

int Init_Train_KNN(Mat&, Mat&);
int Preprocess_Image(Mat&, Mat&);
int Extract_Plate(Mat&, vector< Contour_Class>&, Mat&);
int Insertion_Sort(vector< vector< Point> >&, vector< vector< Point> >&);
int Update_Train_KNN(Mat&, Mat&);
bool Limit_Plate_Size(RotatedRect, float);


#endif