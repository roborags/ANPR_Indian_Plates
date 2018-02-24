#include "Main.h"




const int MIN_CONTOUR_AREA = 100;

const int RESIZED_IMAGE_WIDTH = 20;
const int RESIZED_IMAGE_HEIGHT = 30;

const int RESIZED_PLATE_WIDTH = 250;
const int RESIZED_PLATE_HEIGHT = 50;

const int FILE_NAME_NUM_START = 21;


int main(int argc, char ** argv)
{

	Mat KNN_Char_Class_Mat, KNN_Char_Image_Mat;

	Ptr<KNearest>  KNN_Algo(KNearest::create());

	char Input_FIlename[100];

	int Input_File_Num = FILE_NAME_NUM_START;

	vector< vector< Point> > Input_contours;

	vector< vector< Point> > Plate_contours;

	vector<Contour_Class> Final_contour;

	vector<Contour_Class> Sorted_contour;

	vector<Contour_Class> Proc_contour;

	
	vector<vector<Point> >::iterator Itr_Contour;

	vector<RotatedRect> Input_Img_Rect;


	int Loop_int_1 = 0, Loop_int_2 = 0, Loop_int_3 = 0;

	Point2f Point_2f_1, Point_2f_2;

	Init_Train_KNN(KNN_Char_Class_Mat, KNN_Char_Image_Mat);

loop_back:

	Mat Input_Image, Thresh_Image, Thresh_Image_Copy, Plate_Image, Final_Image, Plate_Thresh_Clone, Plate_Thresh;

	sprintf(Input_FIlename, "Images/image%d.jpg", Input_File_Num);

	Input_Image = imread(Input_FIlename, CV_LOAD_IMAGE_COLOR);
	if (!Input_Image.data)
	{
		cout << "Error: Couldn't open Input Image, press any key to quit\n" << endl;
		waitKey(0);
		return 1;
	}

	KNN_Algo->train(KNN_Char_Image_Mat, ROW_SAMPLE, KNN_Char_Class_Mat);

	Preprocess_Image(Input_Image, Thresh_Image);

	Thresh_Image_Copy = Thresh_Image.clone();

	findContours(Thresh_Image_Copy, Input_contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);//CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

	Mat Pre_Contour_Image(Thresh_Image.size(), CV_8UC3, Scalar(0.0, 0.0, 0.0));
	drawContours(Pre_Contour_Image, Input_contours, -1, Scalar(0.0, 0.0, 255.0));

	Mat Contour_Image(Thresh_Image.size(), CV_8UC3, Scalar(0.0, 0.0, 0.0));
	Mat Rect_Contour_Image(Thresh_Image.size(), CV_8UC3, Scalar(0.0, 0.0, 0.0));
	

	Loop_int_1 = 0;

	Itr_Contour = Input_contours.begin();

	for (int i = 0; i < Input_contours.size(); i++)
	{
		Contour_Class contourWithData;
		contourWithData.ptContour = Input_contours[i];
		contourWithData.boundingRect = cv::boundingRect(contourWithData.ptContour);
		Sorted_contour.push_back(contourWithData);
	}

	std::sort(Sorted_contour.begin(), Sorted_contour.end(), Contour_Class::Sort_Contour_Size);

	for (int i = 0; i < 5; i++)
	{
		Contour_Class ContourToProcess;
		ContourToProcess.ptContour = Sorted_contour[i].ptContour;
		ContourToProcess.boundingRect = Sorted_contour[i].boundingRect;
		Proc_contour.push_back(ContourToProcess);
	}
	
	/*
	Itr_Contour = Input_contours.begin();

	for (int i = 0; i < 5; i++)
	{

	}
	
	for (; (Itr_Contour != Input_contours.end());)
	{
		/*
		//RotatedRect Contour_Min_Rect = minAreaRect(Mat(*Itr_Contour));
		if (!Limit_Plate_Size(Contour_Min_Rect, 3.75))
		{
			Itr_Contour = Input_contours.erase(Itr_Contour);
		}
		else

		{
			++Itr_Contour;

			cout << "First Width " << Contour_Min_Rect.size.width << " Height = " << Contour_Min_Rect.size.height
				<< " X = " << Contour_Min_Rect.center.x << " Y = " << Contour_Min_Rect.center.y << endl;

			Input_Img_Rect.push_back(Contour_Min_Rect);

			Point2f rect_points[4];
			Input_Img_Rect[Loop_int_1].points(rect_points);
			for (int Loop_int_2 = 0; Loop_int_2 < 4; Loop_int_2++)
				line(Rect_Contour_Image, rect_points[Loop_int_2], rect_points[(Loop_int_2 + 1) % 4], Scalar(0.0, 255.0, 0.0), 1, 8);

			Loop_int_1++;
		}
	}
	*/
	
	//cv::drawContours(Rect_Contour_Image, Input_contours, -1, Scalar(0.0, 0.0, 255.0));

	Loop_int_2 = 0;
	Loop_int_3 = 0;

	if (Input_contours.size() > 0)
	{

		double Contour_Dist[100];
		double Contour_Max_Dist = 0;
		int Contour_Max_Pos = 0;

		for (int i = 0; i < 5; i++)
		{
			Point_2f_1.x = Proc_contour[i].boundingRect.width / 2;
			Point_2f_1.y = Proc_contour[i].boundingRect.height / 2;

			for (Loop_int_3 = 0; Loop_int_3 < Proc_contour[i].ptContour.size(); Loop_int_3++)
			{
				Point_2f_2 = Proc_contour[i].ptContour.at(Loop_int_3);
				Contour_Dist[Loop_int_2] += norm(Point_2f_2 - Point_2f_1);
			}

			if (Contour_Dist[Loop_int_2] > Contour_Max_Dist)
			{
				Contour_Max_Dist = Contour_Dist[Loop_int_2];
				Contour_Max_Pos = Loop_int_2;
			}
		}
		/*
		Itr_Contour = Input_contours.begin();

		for (Loop_int_2 = 0; Loop_int_2<Loop_int_1; Loop_int_2++)
		{
			vector<Point> Indv_Contour = Input_contours.at(Loop_int_2);

			Point_2f_1 = Input_Img_Rect[Loop_int_2].center;

			Contour_Dist[Loop_int_2] = 0;

			for (Loop_int_3 = 0; Loop_int_3 < Indv_Contour.size(); Loop_int_3++)
			{
				Point_2f_2 = Indv_Contour.at(Loop_int_3);
				Contour_Dist[Loop_int_2] += norm(Point_2f_2 - Point_2f_1);
			}

			if (Contour_Dist[Loop_int_2] > Contour_Max_Dist)
			{
				Contour_Max_Dist = Contour_Dist[Loop_int_2];
				Contour_Max_Pos = Loop_int_2;
			}
		}
		*/
		cout << "Min " << Contour_Max_Dist << " pos = " << Contour_Max_Pos << endl;

		Contour_Class CopyContourclass;
		CopyContourclass.ptContour = Proc_contour[Contour_Max_Pos].ptContour;
		CopyContourclass.boundingRect = Proc_contour[Contour_Max_Pos].boundingRect;
		Final_contour.push_back(CopyContourclass);

		//cv::drawContours(Rect_Contour_Image, Final_contour[0].ptContour, -1, Scalar(0.0, 0.0, 255.0));

		Point2f rect_points[4];
		rect_points[0] = Point2f(Final_contour[0].boundingRect.x - 2, Final_contour[0].boundingRect.y - 2);
		rect_points[1] = Point2f(Final_contour[0].boundingRect.x + Final_contour[0].boundingRect.width + 2, Final_contour[0].boundingRect.y - 2);
		rect_points[2] = Point2f(Final_contour[0].boundingRect.x + Final_contour[0].boundingRect.width + 2, Final_contour[0].boundingRect.y + Final_contour[0].boundingRect.height + 2);
		rect_points[3] = Point2f(Final_contour[0].boundingRect.x - 2, Final_contour[0].boundingRect.y + Final_contour[0].boundingRect.height + 2);

		for (int Loop_int_2 = 0; Loop_int_2 < 4; Loop_int_2++)
		{
			line(Rect_Contour_Image, rect_points[Loop_int_2], rect_points[(Loop_int_2 + 1) % 4], Scalar(0.0, 255.0, 0.0), 1, 8);
		}

		/*
		Itr_Contour = Input_contours.begin();

		for (Loop_int_2 = 0; Loop_int_2 < 5; Loop_int_2++)
		{
			cout << "Length " << Loop_int_2 << " is = " << Contour_Dist[Loop_int_2] << endl;
			if (Loop_int_2 == Contour_Max_Pos)
			{
				Proc_contour[Loop_int_2].
				Itr_Contour = Input_contours.erase(Itr_Contour);
				cout << "Erase " << endl;
			}
			else
			{
				cout << "Copy " << endl;
				Itr_Contour++;
			}
		}
		*/
		Extract_Plate(Input_Image, Final_contour, Plate_Image);

		Final_Image = Plate_Image.clone();

		Mat Plate_Gray, Plate_Blurred;

		cvtColor(Plate_Image, Plate_Gray, CV_BGR2GRAY);

		GaussianBlur(Plate_Gray, Plate_Blurred, Size(5, 5), 0);

		adaptiveThreshold(Plate_Blurred, Plate_Thresh, 255.0, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY_INV, 11, 2);

		Plate_Thresh_Clone = Plate_Thresh.clone();

		findContours(Plate_Thresh_Clone, Plate_contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);//CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

		Mat Plate_Contour_Image(Plate_Thresh.size(), CV_8UC3, Scalar(0.0, 0.0, 0.0));

		Itr_Contour = Plate_contours.begin();

		Loop_int_1 = 0;

		int Plate_Contour_Area[100];
		Contour_Max_Dist = 0;
		Contour_Max_Pos = 0;

		for (; (Itr_Contour != Plate_contours.end());)
		{

			Rect Plate_Rec = boundingRect(Mat(*Itr_Contour));

			float Aspect_Contour = (float)Plate_Rec.width / (float)Plate_Rec.height;

			if (Aspect_Contour < 1)
			{
				++Itr_Contour;

				Plate_Contour_Area[Loop_int_1] = Plate_Rec.width * Plate_Rec.height;

				if (Plate_Contour_Area[Loop_int_1] > Contour_Max_Dist)
				{
					Contour_Max_Dist = Plate_Contour_Area[Loop_int_1];
					Contour_Max_Pos = Loop_int_1;
				}

				Loop_int_1++;
			}
			else
			{
				Itr_Contour = Plate_contours.erase(Itr_Contour);
			}

		}

		Plate_Contour_Area[Loop_int_1] = -1;

		Itr_Contour = Plate_contours.begin();

		Loop_int_1 = 0;

		for (; (Itr_Contour != Plate_contours.end());)
		{
			if (Plate_Contour_Area[Loop_int_1] < (Contour_Max_Dist - 200))
			{
				Itr_Contour = Plate_contours.erase(Itr_Contour);
			}
			else
			{
				Rect Plate_Rec = boundingRect(Mat(*Itr_Contour));

				Point2f rect_points[4];
				rect_points[0] = Point2f(Plate_Rec.x - 2, Plate_Rec.y - 2);
				rect_points[1] = Point2f(Plate_Rec.x + Plate_Rec.width + 2, Plate_Rec.y - 2);
				rect_points[2] = Point2f(Plate_Rec.x + Plate_Rec.width + 2, Plate_Rec.y + Plate_Rec.height + 2);
				rect_points[3] = Point2f(Plate_Rec.x - 2, Plate_Rec.y + Plate_Rec.height + 2);

				for (int Loop_int_2 = 0; Loop_int_2 < 4; Loop_int_2++)
				{
					line(Plate_Contour_Image, rect_points[Loop_int_2], rect_points[(Loop_int_2 + 1) % 4], Scalar(0.0, 255.0, 0.0), 1, 8);
					//line(Final_Image, rect_points[Loop_int_2], rect_points[(Loop_int_2 + 1) % 4], Scalar(0.0, 255.0, 0.0), 1, 8);
				}
				++Itr_Contour;
			}

			Loop_int_1++;
		}

		drawContours(Plate_Contour_Image, Plate_contours, -1, Scalar(0.0, 0.0, 255.0));

		//Insertion_Sort(Plate_contours, Plate_Sorted_contours);

		//vector< vector< Point> > Plate_Sorted_contours(Plate_contours.size());

		Final_contour.clear();

		for (int i = 0; i < Plate_contours.size(); i++)
		{
			Contour_Class contourWithData;
			contourWithData.ptContour = Plate_contours[i];
			contourWithData.boundingRect = cv::boundingRect(contourWithData.ptContour);
			Final_contour.push_back(contourWithData);
		}

		std::sort(Final_contour.begin(), Final_contour.end(), Contour_Class::Sort_Contour);


		string Plate_String;

		for (int Loop_int_1 = 0; Loop_int_1 < Final_contour.size(); Loop_int_1++)
		{
			Mat Plate_ROI, Plate_ROI_Resize, Plate_ROI_Float, Plate_ROI_Flat_Float;
			Mat Plate_Curr_Char(0, 0, CV_32F);

			Rect Char_Rect = Final_contour[Loop_int_1].boundingRect;

			rectangle(Plate_Image, Char_Rect, Scalar(0, 255, 0), 2);

			Plate_ROI = Plate_Thresh(Char_Rect);

			resize(Plate_ROI, Plate_ROI_Resize, Size(RESIZED_IMAGE_WIDTH, RESIZED_IMAGE_HEIGHT));

			Plate_ROI_Resize.convertTo(Plate_ROI_Float, CV_32FC1);

			Plate_ROI_Flat_Float = Plate_ROI_Float.reshape(1, 1);

			KNN_Algo->findNearest(Plate_ROI_Flat_Float, 1, Plate_Curr_Char);

			float fltCurrentChar = (float)Plate_Curr_Char.at<float>(0, 0);

			Plate_String = Plate_String + char(int(fltCurrentChar));

			Plate_ROI.release();
			Plate_ROI_Resize.release();
			Plate_ROI_Float.release();
			Plate_ROI_Flat_Float.release();
			Plate_Curr_Char.release();
		}

		cout << "\n\n" << "License plate read is = " << Plate_String << "\n\n";

		namedWindow("Plate Contour", CV_WINDOW_AUTOSIZE);
		imshow("Plate Contour", Plate_Contour_Image);

		namedWindow("Plate Image", CV_WINDOW_AUTOSIZE);
		imshow("Plate Image", Final_Image);

		Plate_String.clear();

		Plate_Gray.release();
		Plate_Blurred.release();
		Plate_Contour_Image.release();

	}
	else
	{
		cout << "No Contours found, skip image" << endl;
	}

	cout << "Press T to train, S to skip image or any other key to end" << endl;

	int Char_In = waitKey(0);
	if ((Char_In == 84) || (Char_In == 116))
	{
		cout << "Press on the key for the character in red box" << endl;

		for (int Loop_int_1 = 0; Loop_int_1 < Plate_contours.size(); Loop_int_1++)
		{
			Mat Plate_ROI, Plate_ROI_Resize, Plate_ROI_Float, Plate_ROI_Flat_Float;

			Rect Char_Rect = boundingRect(Plate_contours[Loop_int_1]);

			rectangle(Final_Image, Char_Rect, Scalar(0, 0, 255), 2);

			Plate_ROI = Plate_Thresh(Char_Rect);

			resize(Plate_ROI, Plate_ROI_Resize, Size(RESIZED_IMAGE_WIDTH, RESIZED_IMAGE_HEIGHT));

			imshow("Training Char", Plate_ROI);
			imshow("Training Char Resize", Plate_ROI_Resize);
			imshow("Training Plate", Final_Image);

			Char_In = waitKey(0);

			if (find(intValidChars.begin(), intValidChars.end(), Char_In) != intValidChars.end())
			{

				KNN_Char_Class_Mat.push_back(Char_In);

				Plate_ROI_Resize.convertTo(Plate_ROI_Float, CV_32FC1);

				Plate_ROI_Flat_Float = Plate_ROI_Float.reshape(1, 1);

				KNN_Char_Image_Mat.push_back(Plate_ROI_Flat_Float);
			}

			Plate_ROI.release();
			Plate_ROI_Resize.release();
			Plate_ROI_Float.release();
			Plate_ROI_Flat_Float.release();

		}

		//Update_Train_KNN(KNN_Char_Class_Mat, KNN_Char_Image_Mat);

		Input_Image.release();
		Thresh_Image.release();
		Thresh_Image_Copy.release();
		Plate_Image.release();
		Final_Image.release();
		Plate_Thresh_Clone.release();
		Plate_Thresh.release();
		Contour_Image.release();
		Rect_Contour_Image.release();

		//Input_File_Num++;
		//goto loop_back;

	}
	else if ((Char_In == 83) || (Char_In == 115))
	{
		Input_Image.release();
		Thresh_Image.release();
		Thresh_Image_Copy.release();
		Plate_Image.release();
		Final_Image.release();
		Plate_Thresh_Clone.release();
		Plate_Thresh.release();
		Contour_Image.release();
		Rect_Contour_Image.release();

		Input_File_Num++;
		//goto loop_back;
	}

	Update_Train_KNN(KNN_Char_Class_Mat, KNN_Char_Image_Mat);

	KNN_Char_Class_Mat.release();

	KNN_Char_Image_Mat.release();
}

int Init_Train_KNN(Mat& Char_Classify, Mat& Char_Image)
{

	FileStorage Char_Class_File("KNN_Classify_Char.xml", 0);
	if (Char_Class_File.isOpened() == false)
	{
		cout << "Error opening KNN_Classify_Char.xml , press any key to exit" << endl;
		waitKey(0);
		return(0);
	}

	Char_Class_File["Char_classify"] >> Char_Classify;
	Char_Class_File.release();

	FileStorage Char_Image_File("KNN_Char_Images.xml", 0);
	if (Char_Image_File.isOpened() == false)
	{
		cout << "Error opening KNN_Char_Images.xml , press any key to exit" << endl;
		waitKey(0);
		return(0);
	}

	Char_Image_File["Char_Images"] >> Char_Image;
	Char_Image_File.release();

	return(1);

}

int Update_Train_KNN(Mat& Char_Classify, Mat& Char_Image)
{

	FileStorage Char_Class_File("KNN_Classify_Char.xml", FileStorage::WRITE);
	if (Char_Class_File.isOpened() == false)
	{
		cout << "Error writing KNN_Classify_Char.xml , press any key to exit" << endl;
		waitKey(0);
		return(0);
	}

	Char_Class_File << "Char_classify" << Char_Classify;


	FileStorage Char_Image_File("KNN_Char_Images.xml", FileStorage::WRITE);
	if (Char_Image_File.isOpened() == false)
	{
		cout << "Error writing KNN_Char_Images.xml , press any key to exit" << endl;
		waitKey(0);
		return(0);
	}
	Char_Image_File << "Char_Images" << Char_Image;
	Char_Class_File.release();
	Char_Image_File.release();

	return(1);
}

int Preprocess_Image(Mat& Input_Image, Mat& Threshold_Image)
{
	Mat HSV_Image, Top_Hat, Black_Hat, GS_Top_Hat, GS_Top_Neg_Black_Hat, Blur_Image, Morph_Image, Gray_Image;

	vector<Mat> HSV_Channels;

	Mat Hat_Struct = getStructuringElement(CV_SHAPE_RECT, Size(3, 3));

	cvtColor(Input_Image, HSV_Image, CV_BGR2HSV);

	split(HSV_Image, HSV_Channels);

	Gray_Image = HSV_Channels[2];

	morphologyEx(Gray_Image, Top_Hat, CV_MOP_TOPHAT, Hat_Struct);
	cv::morphologyEx(Gray_Image, Black_Hat, CV_MOP_BLACKHAT, Hat_Struct);

	GS_Top_Hat = Gray_Image + Top_Hat;
	GS_Top_Neg_Black_Hat = GS_Top_Hat - Black_Hat;

	Morph_Image = GS_Top_Neg_Black_Hat.clone();

	GaussianBlur(Gray_Image, Blur_Image, Size(5, 5), 0);

	adaptiveThreshold(Blur_Image, Threshold_Image, 255.0, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY_INV, 19, 9);

	HSV_Image.release();
	Top_Hat.release();
	Black_Hat.release();
	GS_Top_Hat.release();
	GS_Top_Neg_Black_Hat.release();
	Blur_Image.release();
	Morph_Image.release();
	Gray_Image.release();

	return(1);
}

int Extract_Plate(Mat& Input_Image, vector< Contour_Class>& Input_contours, Mat& Output_Plate_Image)
{
	//vector<vector<Point> >::iterator Itr_Contour;
	RotatedRect Contour_Min_Rect;
	float Contour_Aspect, Contour_Angle = 0;
	Mat Plate_Rot_Matrix, Rot_Image, Crop_Image;
	Mat Rect_Contour_Image(Input_Image.size(), CV_8UC3, Scalar(0.0, 0.0, 0.0));
	Size Plate_size;
	Point2f Contour_Center;

	Contour_Min_Rect = minAreaRect(Mat(Input_contours[0].ptContour));

	Point2f rect_points[4];
	Contour_Min_Rect.points(rect_points);
	for (int Loop_int_2 = 0; Loop_int_2 < 4; Loop_int_2++)
		line(Rect_Contour_Image, rect_points[Loop_int_2], rect_points[(Loop_int_2 + 1) % 4], Scalar(0.0, 255.0, 0.0), 1, 8);


	/*
	Contour_Aspect = (float)Input_contours[0].boundingRect.width / (float)Input_contours[0].boundingRect.height;
	Contour_Center.x = (float)(Input_contours[0].boundingRect.width/2.00);
	Contour_Center.y = (float)(Input_contours[0].boundingRect.height/2.00);

	*/

	//Itr_Contour = Input_contours.begin();

	//Contour_Min_Rect = minAreaRect(Mat(*Itr_Contour));

	Contour_Aspect = (float)Contour_Min_Rect.size.width / (float)Contour_Min_Rect.size.height;
	Contour_Angle = Contour_Min_Rect.angle;
	Contour_Center = Contour_Min_Rect.center;

	if (Contour_Aspect < 1)
		Contour_Angle = 90 + Contour_Angle;

	Plate_Rot_Matrix = getRotationMatrix2D(Contour_Center, Contour_Angle, 1);

	warpAffine(Input_Image, Rot_Image, Plate_Rot_Matrix, Input_Image.size(), CV_INTER_CUBIC);

	Plate_size = Contour_Min_Rect.size;

	if (Contour_Aspect < 1)
		swap(Plate_size.width, Plate_size.height);

	getRectSubPix(Rot_Image, Plate_size, Contour_Center, Crop_Image);

	Output_Plate_Image.create(RESIZED_PLATE_HEIGHT, RESIZED_PLATE_WIDTH, CV_8UC3);
	resize(Crop_Image, Output_Plate_Image, Output_Plate_Image.size(), 0, 0, INTER_CUBIC);

	return(1);
}



bool Limit_Plate_Size(RotatedRect Plate_Rect, float Plate_Aspect)
{
	float Limit_err, Limit_rmin, Limit_rmax, Limit_Aspect;
	int Limit_min, Limit_max, Limit_area;

	Limit_err = 0.4;

	Limit_min = 2500 * Plate_Aspect;
	Limit_max = 250000 * Plate_Aspect;

	Limit_rmin = (Plate_Aspect - (Plate_Aspect * Limit_err));
	Limit_rmax = (Plate_Aspect + (Plate_Aspect * Limit_err));

	Limit_area = (Plate_Rect.size.height * Plate_Rect.size.width);
	Limit_Aspect = (float)Plate_Rect.size.width / (float)Plate_Rect.size.height;

	if ((Limit_area < Limit_min || Limit_area > Limit_max) || (Limit_Aspect < Limit_rmin || Limit_Aspect > Limit_rmax))
	{
		return false;
	}
	else
	{
		return true;
	}

}