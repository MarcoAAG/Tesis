//ROS stuff
#include "ros/ros.h"
#include "std_msgs/String.h"
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include "std_msgs/Int32.h"
#include "std_msgs/Int32MultiArray.h"
#include "std_msgs/MultiArrayLayout.h"
#include "std_msgs/MultiArrayDimension.h"

//C++ stuff
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <unistd.h>
#include <vector>
#include <algorithm>
#include <math.h>

//OpenCV stuff
#include "opencv2/opencv.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video.hpp>
#include "opencv2/imgcodecs.hpp"

using namespace cv;
using namespace std;

#define Azul 1
#define Amarillo 0
#define Rojo 0
#define Verde 0

#if Azul
int low_H = 95;
int low_S = 55;
int low_V = 55;
int high_H = 130;
int high_S = 255;
int high_V = 255;
#endif

#if Amarillo
int low_H = 25;
int low_S = 50;
int low_V = 50;
int high_H = 35;
int high_S = 255;
int high_V = 255;
#endif

#if Rojo
int low_H = 0; //165
int low_S = 70;
int low_V = 70;
int high_H = 10; //185
int high_S = 255;
int high_V = 255;
#endif

#if Verde
int low_H = 35;
int low_S = 50;
int low_V = 50;
int high_H = 90;
int high_S = 255;
int high_V = 255;
#endif

#define width 640
#define height 480

ros::Publisher coordinates_pub; //Crear el publicador

class objectTracking
{
private:
    image_transport::Subscriber sub; //Crear el suscriptor
    string nameOriginal = "Imagen Original";
    string nameHSV = "HSV";
    string nameFilter = "Filtro";
    string nameCentroid = "Centroide";
    Mat ImgHSV;
    Mat ImgOpening;
    Mat ImgClosing;
    Mat ImgCentroid;

public:
    Mat Img;
    int imageX = width / 2;
    int imageY = height / 2;
    uint16_t posX = 320;
    uint16_t posY = 240;
    uint16_t lastX = 0;
    uint16_t lastY = 0;

    objectTracking(image_transport::ImageTransport *it);
    ~objectTracking();
    void callBack(const sensor_msgs::ImageConstPtr &msg_);
    void imageViewer(Mat &Image, string _windowName);
    void rgb2hsv(Mat &_Image, Mat &Image_);
    void morphologyOperation(Mat &_Image, Mat &Image_, int oper, int size, int elemen);
    void centroid(Mat &_Image, uint16_t &x, uint16_t &y);
    void drawCentroid(Mat &_Image, Mat &Image_, uint16_t posx, uint16_t posy, uint16_t lastx, uint16_t lasty);
    void publishCoordinates(int _x, int _y);
};

int main(int argc, char **argv)
{
    ros::init(argc, argv, "Object_tracking");
    ros::NodeHandle nh;
    image_transport::ImageTransport it(nh);
    objectTracking ot = objectTracking(&it);
    coordinates_pub = nh.advertise<std_msgs::Int32MultiArray>("coordinates", 100);
    ros::spin();

    return 0;
}

objectTracking::objectTracking(image_transport::ImageTransport *it)
{
    sub = it->subscribe("ocam/image_enhance", 1, &objectTracking::callBack, this);
}

objectTracking::~objectTracking()
{
}
void objectTracking::callBack(const sensor_msgs::ImageConstPtr &msg_)
{
    try
    {
        cv_bridge::CvImagePtr cvImg = cv_bridge::toCvCopy(msg_, "bgr8");
        Img = cvImg->image;
        Mat ImgLines = Mat::zeros(Img.size(), CV_8UC3);

        rgb2hsv(Img, ImgHSV);
        // imageViewer(Img, nameOriginal);
        imageViewer(ImgHSV, nameHSV);
        //opening
        morphologyOperation(ImgHSV, ImgOpening, 0, 5, 0);
        // morphologyOperation(ImgHSV, ImgOpening, 0, 3, 0);
        //closing
        morphologyOperation(ImgOpening, ImgClosing, 1, 2, 2);
        // morphologyOperation(ImgOpening, ImgClosing, 1, 0, 0);
        imageViewer(ImgClosing, nameFilter);
        centroid(ImgClosing, posX, posY);
        drawCentroid(ImgLines, Img, posX, posY, lastX, lastY);
        imageViewer(Img, nameCentroid);
        publishCoordinates(posX, posY);
    }
    catch (const std::exception &e)
    {
        ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg_->encoding.c_str());
    }
}
void objectTracking::imageViewer(Mat &Image, string _windowName)
{
    imshow(_windowName, Image);
    waitKey(10);
}
void objectTracking::rgb2hsv(Mat &_Image, Mat &Image_)
{
    cvtColor(_Image, Image_, COLOR_BGR2HSV);
    inRange(Image_, Scalar(low_H, low_S, low_V), Scalar(high_H, high_S, high_V), Image_);
}
void objectTracking::morphologyOperation(Mat &_Image, Mat &Image_, int oper, int size, int elemen)
{
    // int const max_operator = 4;
    // int const max_elem = 2;
    // int const max_kernel_size = 21;
    int operation = oper + 2;
    Mat element = getStructuringElement(elemen, Size(2 * size + 1, 2 * size + 1), Point(size, size));
    morphologyEx(_Image, Image_, operation, element);
}
void objectTracking::centroid(Mat &_Image, uint16_t &x, uint16_t &y)
{
    Moments oMoments = moments(_Image);
    double dM01 = oMoments.m01;
    double dM10 = oMoments.m10;
    double dArea = oMoments.m00;

    x = dM10 / dArea;
    y = dM01 / dArea;

    if (x > 640 || x <= 0)
    {
        x = 320;
    }
    if (y > 480 || y <= 0)
    {
        y = 240;
    }
}
void objectTracking::drawCentroid(Mat &_Image, Mat &Image_, uint16_t posx, uint16_t posy, uint16_t lastx, uint16_t lasty)
{
    line(_Image, Point2d(320, 240), Point2d(posx, posy), Scalar(0, 0, 255), 2, LINE_8);
    circle(_Image, Point2d(320, 240), 3, Scalar(0, 0, 255), 2, LINE_8, 0);
    circle(_Image, Point2d(posx, posy), 3, Scalar(0, 255, 0), 2, LINE_8, 0);
    Image_ = Image_ + _Image;
}
void objectTracking::publishCoordinates(int _x, int _y)
{
    std_msgs::Int32MultiArray array;
    array.data.clear();
    array.data.push_back(_x);
    array.data.push_back(_y);
    coordinates_pub.publish(array);
    // ROS_INFO("Publicando Coordenadas");
}