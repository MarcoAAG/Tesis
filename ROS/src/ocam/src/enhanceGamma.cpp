#include <iostream>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "ros/ros.h"
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>

cv::Mat Image;
cv::Mat ImgGamma;
image_transport::Subscriber sub; //Crear el suscriptor
image_transport::Publisher pub;

int gamma_cor = 100;

using namespace cv;

void on_gamma_correction_trackbar(int, void *);
void gammaCorrection(const Mat &img, const double gamma_);
void publisher(Mat &_Image);

void imageCallback(const sensor_msgs::ImageConstPtr &msg_)
{
    cv_bridge::CvImagePtr cvimg = cv_bridge::toCvCopy(msg_, "bgr8");
    Image = cvimg->image;
    try
    {
        // imshow("Gamma correction", Image);

        /* code */
        ImgGamma = cv::Mat(Image.rows, Image.cols, Image.type());
        namedWindow("Gamma correction");
        createTrackbar("Gamma correction", "Gamma correction", &gamma_cor, 200, on_gamma_correction_trackbar);
        on_gamma_correction_trackbar(0, 0);
        cv::waitKey(10);
    }
    catch (const std::exception &e)
    {
        ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg_->encoding.c_str());
    }
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "GammaCorrection");
    ros::NodeHandle nh;
    image_transport::ImageTransport it(nh);
    sub = it.subscribe("ocam/Image", 1, imageCallback);
    pub = it.advertise("ocam/image_enhance",1);
    ros::spin();

    return 0;
}

void on_gamma_correction_trackbar(int, void *)
{
    double gamma_value = gamma_cor / 100.0;
    gammaCorrection(Image, gamma_value);
}
void gammaCorrection(const Mat &img, const double gamma_)
{
    CV_Assert(gamma_ >= 0);
    //! [changing-contrast-brightness-gamma-correction]
    Mat lookUpTable(1, 256, CV_8U);
    uchar *p = lookUpTable.ptr();
    for (int i = 0; i < 256; ++i)
        p[i] = saturate_cast<uchar>(pow(i / 255.0, gamma_) * 255.0);

    Mat res = img.clone();
    LUT(img, lookUpTable, res);
    //! [changing-contrast-brightness-gamma-correction]

    imshow("Gamma correction", res);
    publisher(res);
}
void publisher(Mat &_Image)
{
    sensor_msgs::ImagePtr MSG;
    MSG = cv_bridge::CvImage(std_msgs::Header(), "bgr8", _Image).toImageMsg();
    pub.publish(MSG);
}