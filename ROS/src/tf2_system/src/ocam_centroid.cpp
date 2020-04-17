#include <ros/ros.h>
#include <tf2/LinearMath/Quaternion.h>
#include <tf2_ros/transform_broadcaster.h>
#include <geometry_msgs/TransformStamped.h>

#include "std_msgs/MultiArrayLayout.h"
#include "std_msgs/MultiArrayDimension.h"
#include "std_msgs/Int32MultiArray.h"

void arrayCallback(const std_msgs::Int32MultiArray::ConstPtr &array);
int centroid[2];

int main(int argc, char **argv)
{
    ros::init(argc, argv, "tf2_centroid");
    ros::NodeHandle n;
    ros::Subscriber sub = n.subscribe("coordinates", 100, arrayCallback);
    ros::spin();
    return 0;
};

void arrayCallback(const std_msgs::Int32MultiArray::ConstPtr &array)
{
    int i = 0;
    // print all the remaining numbers
    for (std::vector<int>::const_iterator it = array->data.begin(); it != array->data.end(); ++it)
    {
        centroid[i] = *it;
        i++;
    }
    static tf2_ros::TransformBroadcaster br;
    geometry_msgs::TransformStamped transformStamped;

    transformStamped.header.stamp = ros::Time::now();
    transformStamped.header.frame_id = "/ocam";
    transformStamped.child_frame_id = "tf2";
    transformStamped.transform.translation.x = centroid[0]*0.01;
    transformStamped.transform.translation.y = centroid[1]*0.01;
    transformStamped.transform.translation.z = 0.0;
    tf2::Quaternion q;
    q.setRPY(0, 0, 0);
    transformStamped.transform.rotation.x = q.x();
    transformStamped.transform.rotation.y = q.y();
    transformStamped.transform.rotation.z = q.z();
    transformStamped.transform.rotation.w = q.w();

    br.sendTransform(transformStamped);
}