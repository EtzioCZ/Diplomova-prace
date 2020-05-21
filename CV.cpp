#include "CV.h"
#include <exception>
#include <cstdlib>

using namespace cv;

CV::CV() 
{
}


void CV::imageRead(string filename)
{
    m_mat = cv::imread(filename, cv::IMREAD_COLOR);
    m_mat_original = m_mat;

    if (m_mat.empty()) {
        printf(" Error opening image\n");
       
        return;

         throw std::exception("Error opening image");
    }
}

void CV::imageShow(string window_name)
{
    cv::imshow(window_name, m_mat);
}

void CV::imageResizeOriginal(int target_x, int target_y)
{
    cv::resize(m_mat_original, m_mat_resized, cv::Size(target_x, target_y), 0.0, 0.0, cv::INTER_LINEAR);
    m_mat = m_mat_resized;
    cv::imwrite("resized.png", m_mat_resized);
}

void CV::imageResize(int target_x, int target_y)
{
    cv::resize(m_mat, m_mat_resized, cv::Size(target_x, target_y), 0.0, 0.0, cv::INTER_LINEAR);
    m_mat = m_mat_resized;
    cv::imwrite("resized.png", m_mat_resized);
}

void CV::RGB2HSV() 
{
    cv::cvtColor(m_mat, m_mat_hsv, cv::ColorConversionCodes::COLOR_BGR2HSV); 
}

void CV::RGB2YCrCb()
{
    cv::cvtColor(m_mat, m_mat_YCrCb, cv::ColorConversionCodes::COLOR_BGR2YCrCb);
}

void CV::RGB2HLS()
{
    cv::cvtColor(m_mat, m_mat_hls, cv::ColorConversionCodes::COLOR_BGR2HLS);
}

void CV::splitRGB()
{    
    for (int i = 0; i < m_mat.cols; i++) {
        //std::vector<my_pixel> row;
        for (int j = 0; j < m_mat.rows; j++) {
            cv::Point3_<uchar>* p = m_mat.ptr<cv::Point3_<uchar> >(i, j);
            RGB_pixel pix;
            pix.B = p->x;
            pix.G = p->y;
            pix.R = p->z;
            RGB.push_back(pix);
        }
        //image.push_back(row);
    }

}

void CV::splitHLS()
{
    for (int i = 0; i < m_mat.cols; i++) {
       //std::vector<my_pixel> row;
       for (int j = 0; j < m_mat.rows; j++) {
           cv::Point3_<uchar>* p = m_mat_hls.ptr<cv::Point3_<uchar> >(i, j);
           HLS_pixel pix;
           pix.H = p->x;
           pix.L = p->y;
           pix.S = p->z;
           HLS.push_back(pix);
       }
       //image.push_back(row);
   }

}

void CV::splitHSV()
{
    for (int i = 0; i < m_mat.cols; i++) {
        //std::vector<my_pixel> row;
        for (int j = 0; j < m_mat.rows; j++) {
            cv::Point3_<uchar>* p = m_mat_hsv.ptr<cv::Point3_<uchar> >(i, j);
            HSV_pixel pix;
            pix.H = p->x;
            pix.S = p->y;
            pix.V = p->z;
            HSV.push_back(pix);
        }
        //image.push_back(row);
    }
}

void CV::splitYCrCb()
{
    for (int i = 0; i < m_mat.cols; i++) {
        //std::vector<my_pixel> row;
        for (int j = 0; j < m_mat.rows; j++) {
            cv::Point3_<uchar>* p = m_mat_YCrCb.ptr<cv::Point3_<uchar> >(i, j);
            YCbCr_pixel pix;
            pix.Y  = p->x;
            pix.Cr = p->y;
            pix.Cb = p->z;
            YCbCr.push_back(pix);
        }
        //image.push_back(row);
    }
} 

void CV::quantizeUchar(unsigned char in, unsigned char& out, float minIN, float maxIN, float minOUT, float maxOUT)
{
    float baf = ((in - minIN) / (maxIN - minIN)) * (maxOUT - minOUT) + minOUT;
    out = float2uchar(round(baf));
}

void CV::clearAllExceptOriginal()
{
   
    m_mat_resized.release();			//prevzorkovaný file
    m_mat_hsv.release();
    m_mat_YCrCb.release();;
    m_mat_hls.release();

    m_mat.release();
    RGB.clear();
    HSV.clear();
    HLS.clear();
    YCbCr.clear();

    RGB_q.clear();
    HSV_q.clear();
    HLS_q.clear();
    YCbCr_q.clear();
}

//void CV::quantizeFloat(unsigned char in, float& out, float minIN, float maxIN, float minOUT, float maxOUT)
//{
//    out = (in - minIN) / (maxIN - minIN) * (maxOUT - minOUT) + minOUT;
//}
