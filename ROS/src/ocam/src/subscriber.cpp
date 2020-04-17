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

using namespace cv;
using namespace std;

#define HSV_valores 1

/*
Inicializar imagenes en opencv
*/
cv::Mat imgOriginal;
cv::Mat imgHSV;
cv::Mat imgthreshold;
cv::Mat imgErosion;
cv::Mat imgDilation;

const String window_capture_name = "Video Capture";
const String window_detection_name = "Object Detection";

image_transport::Subscriber sub; //Crear el suscriptor
ros::Publisher coordinates_pub;  //Crear el publicador

int posX;
int posY;

void publisher(int x_, int y_); // Funcion que publica coordenadas

/*----------------------------------------      HSV     ----------------------------------------*/
#if HSV_valores
/* code */
const int max_value_H = 180; //en opencv el valor max es 180, aunque en realidad va a 360
const int max_value = 255;   //El valor real va en %, del 0 al 100. pero en opencv va de 0 a 255
int low_H = 0;
int low_S = 0;
int low_V = 0;
int high_H = max_value_H;
int high_S = max_value;
int high_V = max_value;
#else
int low_H = 75;
int low_S = 55;
int low_V = 55;
int high_H = 130;
int high_S = 255;
int high_V = 255;
#endif

/*----------------------------------------      Morphological operation     ----------------------------------------*/
/// Global variables
cv::Mat dst_opening;
cv::Mat dst_closing;

int morph_elem_opening = 0;
int morph_size_opening = 0;
int morph_elem_closing = 0;
int morph_size_closing = 0;

int morph_operator_opening = 0;
int morph_operator_closing = 1;
int const max_operator = 4;
int const max_elem = 2;
int const max_kernel_size = 21;

const String window_opening = "Morphology Transformations Opening";
const String window_closing = "Morphology Transformations Closing";

/** Function Headers */
void Morphology_Operations_Opening(int, void *);
void Morphology_Operations_Closing(int, void *);

#if HSV_valores
static void on_low_H_thresh_trackbar(int, void *)
{
    low_H = min(high_H - 1, low_H);
    setTrackbarPos("Low H", window_detection_name, low_H);
}
static void on_high_H_thresh_trackbar(int, void *)
{
    high_H = max(high_H, low_H + 1);
    setTrackbarPos("High H", window_detection_name, high_H);
}
static void on_low_S_thresh_trackbar(int, void *)
{
    low_S = min(high_S - 1, low_S);
    setTrackbarPos("Low S", window_detection_name, low_S);
}
static void on_high_S_thresh_trackbar(int, void *)
{
    high_S = max(high_S, low_S + 1);
    setTrackbarPos("High S", window_detection_name, high_S);
}
static void on_low_V_thresh_trackbar(int, void *)
{
    low_V = min(high_V - 1, low_V);
    setTrackbarPos("Low V", window_detection_name, low_V);
}
static void on_high_V_thresh_trackbar(int, void *)
{
    high_V = max(high_V, low_V + 1);
    setTrackbarPos("High V", window_detection_name, high_V);
}
#endif

void imageCallback(const sensor_msgs::ImageConstPtr &msg_)
{
    cv_bridge::CvImagePtr cvimg = cv_bridge::toCvCopy(msg_, "bgr8");
    int iLastX = 0;
    int iLastY = 0;

    //Capture a temporary image from the camera
    Mat imgTmp;
    imgTmp = cvimg->image;

    //Create a black image with the size as the camera output
    Mat imgLines = Mat::zeros(imgTmp.size(), CV_8UC3);

    try
    {
        imgOriginal = cvimg->image;
        namedWindow(window_capture_name);
        namedWindow(window_detection_name);

#if HSV_valores
        // Trackbars to set thresholds for HSV values
        //para azules : low = ([110,50,50])  high ([130,255,255])
        createTrackbar("Low H", window_detection_name, &low_H, max_value_H, on_low_H_thresh_trackbar);
        createTrackbar("High H", window_detection_name, &high_H, max_value_H, on_high_H_thresh_trackbar);
        createTrackbar("Low S", window_detection_name, &low_S, max_value, on_low_S_thresh_trackbar);
        createTrackbar("High S", window_detection_name, &high_S, max_value, on_high_S_thresh_trackbar);
        createTrackbar("Low V", window_detection_name, &low_V, max_value, on_low_V_thresh_trackbar);
        createTrackbar("High V", window_detection_name, &high_V, max_value, on_high_V_thresh_trackbar);
#endif

        cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV);

        inRange(imgHSV, Scalar(low_H, low_S, low_V), Scalar(high_H, high_S, high_V), imgthreshold);
        // Show the frames
        imshow(window_capture_name, imgOriginal);
        imshow(window_detection_name, imgthreshold);

        /// Create window
        namedWindow(window_opening, CV_WINDOW_AUTOSIZE);

        /// Create Trackbar to select kernel type
        createTrackbar("Element:\n 0: Rect - 1: Cross - 2: Ellipse", window_opening,
                       &morph_elem_opening, max_elem,
                       Morphology_Operations_Opening);

        /// Create Trackbar to choose kernel size
        createTrackbar("Kernel size:\n 2n +1", window_opening,
                       &morph_size_opening, max_kernel_size,
                       Morphology_Operations_Opening);

        /// Create window
        namedWindow(window_closing, CV_WINDOW_AUTOSIZE);

        /// Create Trackbar to select kernel type
        createTrackbar("Element:\n 0: Rect - 1: Cross - 2: Ellipse", window_closing,
                       &morph_elem_closing, max_elem,
                       Morphology_Operations_Closing);

        /// Create Trackbar to choose kernel size
        createTrackbar("Kernel size:\n 2n +1", window_closing,
                       &morph_size_closing, max_kernel_size,
                       Morphology_Operations_Closing);

        /// Default start
        Morphology_Operations_Opening(0, 0);

        /// Default start
        Morphology_Operations_Closing(0, 0);

        Moments oMoments = moments(dst_closing);
        double dM01 = oMoments.m01;
        double dM10 = oMoments.m10;
        double dArea = oMoments.m00;

        // if the area <= 10000, I consider that the there are no object in the image and it's because of the noise, the area is not zero
        // if (dArea > 100)
        // {
        //calculate the position of the ball
        posX = dM10 / dArea;
        posY = dM01 / dArea;

        // if (iLastX >= 0 && iLastY >= 0 && posX >= 0 && posY >= 0)
        // {
        //Draw a red line from the previous point to the current point
        // line(imgLines, Point(posX, posY), Point(iLastX, iLastY), Scalar(0, 0, 255), 2);
        //640 x 480
        line(imgLines, Point2d(320, 240), Point2d(posX, posY), Scalar(0, 0, 255), 2, LINE_8);
        circle(imgLines, Point2d(320, 240), 3, Scalar(0, 0, 255), 2, LINE_8, 0);
        circle(imgLines, Point2d(posX, posY), 3, Scalar(0, 255, 0), 2, LINE_8, 0);
        // cout << "X = " << posX << "\t Y = " << posY << endl;
        publisher(posX, posY);

        // }

        iLastX = posX;
        iLastY = posY;
        // }
        imshow("Thresholded Image", dst_closing); //show the thresholded image
        imgOriginal = imgOriginal + imgLines;
        imshow("Original", imgOriginal); //show the original image

    }
    catch (cv_bridge::Exception &e)
    {
        ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg_->encoding.c_str());
    }
}

void publisher(int x_, int y_)
{

    std_msgs::Int32MultiArray array;
    //Clear array
    array.data.clear();
    array.data.push_back(posX);
    array.data.push_back(posY);
    coordinates_pub.publish(array);
    ROS_INFO("Publicando Coordenadas");
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "image_listener");
    ros::NodeHandle nh;
    image_transport::ImageTransport it(nh);
    sub = it.subscribe("/my_camera/image_rect_color", 1, imageCallback);
    coordinates_pub = nh.advertise<std_msgs::Int32MultiArray>("coordinates", 100);
    ros::spin();
    return 0;
}

void Morphology_Operations_Opening(int, void *)
{
    // Since MORPH_X : 2,3,4,5 and 6
    int operation = morph_operator_opening + 2;

    Mat element = getStructuringElement(morph_elem_opening, Size(2 * morph_size_opening + 1, 2 * morph_size_opening + 1), Point(morph_size_opening, morph_size_opening));

    /// Apply the specified morphology operation
    morphologyEx(imgthreshold, dst_opening, operation, element);
    imshow(window_opening, dst_opening);
}

void Morphology_Operations_Closing(int, void *)
{
    // Since MORPH_X : 2,3,4,5 and 6
    int operation = morph_operator_closing + 2;

    Mat element = getStructuringElement(morph_elem_closing, Size(2 * morph_size_closing + 1, 2 * morph_size_closing + 1), Point(morph_size_closing, morph_size_closing));

    /// Apply the specified morphology operation
    morphologyEx(dst_opening, dst_closing, operation, element);
    imshow(window_closing, dst_closing);
}