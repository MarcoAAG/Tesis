//ROS stuff
#include "ros/ros.h"
#include "std_msgs/String.h"
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include "std_msgs/Int32.h"
#include "std_msgs/Int32MultiArray.h"
#include "std_msgs/MultiArrayLayout.h"
#include "std_msgs/MultiArrayDimension.h"

//OpenCV stuff
#include "opencv2/opencv.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video.hpp>

using namespace cv;
using namespace std;

image_transport::Subscriber sub; //Crear el suscriptor
image_transport::Subscriber sub2; //Crear el suscriptor

void imageCallback(const sensor_msgs::ImageConstPtr &msg_)
{

    /* code */
    cv_bridge::CvImagePtr cvimg = cv_bridge::toCvCopy(msg_, "bgr8");
    Mat img;
    img = cvimg->image;
    imshow("Original", img); //show the original image
    cv::waitKey(10);
}
void imageCallbackCalibrada(const sensor_msgs::ImageConstPtr &msg_)
{

    /* code */
    cv_bridge::CvImagePtr cvimg = cv_bridge::toCvCopy(msg_, "bgr8");
    Mat img;
    img = cvimg->image;
    imshow("Calibrada", img); //show the original image
    cv::waitKey(10);
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "image_listener");
    ros::NodeHandle nh;
    image_transport::ImageTransport it(nh);
    sub = it.subscribe("/ocam/image_rect_color", 1, imageCallbackCalibrada);
    sub2 = it.subscribe("/ocam/image_raw", 1, imageCallback);
    ros::spin();
    return 0;
}