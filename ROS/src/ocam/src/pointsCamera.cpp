#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include "ros/ros.h"
#include "std_msgs/MultiArrayLayout.h"
#include "std_msgs/MultiArrayDimension.h"
#include "std_msgs/Int32MultiArray.h"
#include <visualization_msgs/Marker.h>

void arrayCallback(const std_msgs::Int32MultiArray::ConstPtr &array);

int centroid[2];
ros::Publisher marker_pub;

int main(int argc, char **argv)
{
    ros::init(argc, argv, "centroid");
    ros::NodeHandle n;
    ros::Subscriber sub = n.subscribe("coordinates", 100, arrayCallback);
    marker_pub = n.advertise<visualization_msgs::Marker>("visualization_centroid", 10);

    ros::spin();

    return 0;
}

void arrayCallback(const std_msgs::Int32MultiArray::ConstPtr &array)
{

    int i = 0;
    // print all the remaining numbers
    for (std::vector<int>::const_iterator it = array->data.begin(); it != array->data.end(); ++it)
    {
        centroid[i] = *it;
        i++;
    }
    visualization_msgs::Marker points, line_strip, line_list;
    points.header.frame_id = line_strip.header.frame_id = line_list.header.frame_id = "/ocam";
    points.header.stamp = line_strip.header.stamp = line_list.header.stamp = ros::Time::now();
    points.ns = line_strip.ns = line_list.ns = "points_and_lines";
    points.action = line_strip.action = line_list.action = visualization_msgs::Marker::ADD;
    points.pose.orientation.w = line_strip.pose.orientation.w = line_list.pose.orientation.w = 1.0;

    points.id = 0;
    line_list.id = 1;

    points.type = visualization_msgs::Marker::POINTS;
    line_list.type = visualization_msgs::Marker::LINE_LIST;

    // POINTS markers use x and y scale for width/height respectively
    points.scale.x = 0.2;
    points.scale.y = 0.2;

    line_list.scale.x = 0.1;

    // Points are green
    points.color.g = 1.0f;
    points.color.a = 1.0;

    // Line list is red
    line_list.color.r = 1.0;
    line_list.color.a = 1.0;

    geometry_msgs::Point p;
    p.x = centroid[0] * 0.01;
    p.y = centroid[1] * 0.01;
    p.z = 0;
    points.points.push_back(p);

    // The line list needs two points for each line
    line_list.points.push_back(p);
    p.z += 1.0;
    line_list.points.push_back(p);

    marker_pub.publish(points);
    marker_pub.publish(line_list);
}
