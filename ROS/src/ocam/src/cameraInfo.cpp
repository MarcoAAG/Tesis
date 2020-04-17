#include <ros/ros.h>
#include <sensor_msgs/CameraInfo.h>

int main(int argc, char **argv)
{
    ros::init(argc, argv, "image_Info");
    ros::NodeHandle nh;
    sensor_msgs::CameraInfo camera_info_;
    ros::Publisher pub = nh.advertise<sensor_msgs::CameraInfo>("/ocam/camera_info", 100);

    camera_info_.height = 480;
    camera_info_.width = 640;
    camera_info_.distortion_model = "plumb_bob";

    camera_info_.K[0] = 656.41089;
    camera_info_.K[1] = 0.000000;
    camera_info_.K[2] = 320.06053;
    camera_info_.K[3] = 0.000000;
    camera_info_.K[4] = 654.93593;
    camera_info_.K[5] = 222.93267;
    camera_info_.K[6] = 0.000000;
    camera_info_.K[7] = 0.000000;
    camera_info_.K[8] = 1.000000;

    //IDK why this doesn't work
    // camera_info_.D[0] = -0.451566;
    // camera_info_.D[1] = 0.238355;
    // camera_info_.D[2] = -0.000850;
    // camera_info_.D[3] = -0.002483;
    // camera_info_.D[4] = 0.000000;

    camera_info_.R[0] = 1.000000;
    camera_info_.R[1] = 0.000000;
    camera_info_.R[2] = 0.000000;
    camera_info_.R[3] = 0.000000;
    camera_info_.R[4] = 1.000000;
    camera_info_.R[5] = 0.000000;
    camera_info_.R[6] = 0.000000;
    camera_info_.R[7] = 0.000000;
    camera_info_.R[8] = 1.000000;

    camera_info_.P[0] = 581.95032;
    camera_info_.P[1] = 0.000000;
    camera_info_.P[2] = 324.43609;
    camera_info_.P[3] = 0.000000;
    camera_info_.P[4] = 0.000000;
    camera_info_.P[5] = 613.90649;
    camera_info_.P[6] = 221.23437;
    camera_info_.P[7] = 0.000000;
    camera_info_.P[8] = 0.000000;
    camera_info_.P[9] = 0.000000;
    camera_info_.P[10] = 1.000000;
    camera_info_.P[11] = 0.000000;

    ros::Rate loop_rate(60); /*HZ*/

    while (nh.ok())
    {
        pub.publish(camera_info_);
        ros::spinOnce();
        loop_rate.sleep();
    }

    return 0;
}

// camera_info_.height = 640;
// camera_info_.width = 400;
// camera_info_.distortion_model = "plumb_bob";

// camera_info_.K[0] = 653.418544;
// camera_info_.K[1] = 0.000000;
// camera_info_.K[2] = 358.857783;
// camera_info_.K[3] = 0.000000;
// camera_info_.K[4] = 652.863595;
// camera_info_.K[5] = 235.538805;
// camera_info_.K[6] = 0.000000;
// camera_info_.K[7] = 0.000000;
// camera_info_.K[8] = 1.000000;

// camera_info_.D[0] = -0.451566;
// camera_info_.D[1] = 0.238355;
// camera_info_.D[2] = -0.000850;
// camera_info_.D[3] = -0.002483;
// camera_info_.D[4] = 0.000000;

// camera_info_.R[0] = 1.000000;
// camera_info_.R[1] = 0.000000;
// camera_info_.R[2] = 0.000000;
// camera_info_.R[3] = 0.000000;
// camera_info_.R[4] = 1.000000;
// camera_info_.R[5] = 0.000000;
// camera_info_.R[6] = 0.000000;
// camera_info_.R[7] = 0.000000;
// camera_info_.R[8] = 1.000000;

// camera_info_.P[0] = 575.133606;
// camera_info_.P[1] = 0.000000;
// camera_info_.P[2] = 367.129653;
// camera_info_.P[3] = 0.000000;
// camera_info_.P[4] = 0.000000;
// camera_info_.P[5] = 610.114441;
// camera_info_.P[6] = 234.605856;
// camera_info_.P[7] = 0.000000;
// camera_info_.P[8] = 0.000000;
// camera_info_.P[9] = 0.000000;
// camera_info_.P[10] = 1.000000;
// camera_info_.P[11] = 0.000000;

// infocam = nh.advertise<sensor_msgs::CameraInfo>("camera/info", 1);