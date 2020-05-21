#pragma once
#include <opencv2/imgcodecs.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <opencv2/core.hpp>
#include <string>
#include "Utils.h"

using namespace std;

class CV
{

private:


public:
	cv::Mat m_mat_original;			//originální file 
	cv::Mat m_mat_resized;			//prevzorkovaný file
	cv::Mat m_mat_resized_nahled;	//převzorkovaný do náhledu
	cv::Mat m_mat_hsv;
	cv::Mat m_mat_YCrCb;
	cv::Mat m_mat_hls;

	cv::Mat m_mat;					//putovní file

	struct RGB_pixel {
		uchar R;
		uchar G;
		uchar B;
	};

	struct HSV_pixel {
		uchar H;
		uchar S;
		uchar V;
	};

	struct HLS_pixel {
		uchar H;
		uchar L;
		uchar S;
	};

	struct YCbCr_pixel {
		uchar Y;
		uchar Cr;
		uchar Cb;
	};

	std::vector<RGB_pixel> RGB;
	std::vector<HSV_pixel> HSV;
	std::vector<HLS_pixel> HLS;
	std::vector<YCbCr_pixel> YCbCr;

	std::vector<RGB_pixel> RGB_q;
	std::vector<HSV_pixel> HSV_q;
	std::vector<HLS_pixel> HLS_q;
	std::vector<YCbCr_pixel> YCbCr_q;
		
	CV();
	void imageRead(string filename);
	void imageShow(string window_name);
	void imageResizeOriginal(int target_x, int target_y);
	void imageResize(int target_x, int target_y);

	void RGB2HSV();
	void RGB2YCrCb();
	void RGB2HLS();

	void splitRGB();
	void splitHLS();
	void splitHSV();
	void splitYCrCb();

	void quantizeUchar(unsigned char in, unsigned char& out, float minIN, float maxIN, float minOUT, float maxOUT);
	//void quantizeFloat(unsigned char in, float& out, float minIN, float maxIN, float minOUT, float maxOUT);
	
	void clearAllExceptOriginal();
};

