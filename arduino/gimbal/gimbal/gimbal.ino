#include <ros.h>
#include <std_msgs/String.h>
#include <std_msgs/MultiArrayLayout.h>
#include <std_msgs/MultiArrayDimension.h>
#include <std_msgs/Int32MultiArray.h>

//! Arduino stuff
#include <Servo.h>


ros::NodeHandle nh;

//* creamos objeto de clase servo
Servo myServo1;

void callBack(const std_msgs::Int32MultiArray &array)
{
  nh.loginfo("recibiendo datos");
  uint16_t Centroid[2];
  Centroid[0] = array.data[0];  
}

ros::Subscriber<std_msgs::Int32MultiArray> sub("coordinates", callBack);

void setup()
{
  nh.initNode();
  nh.subscribe(sub);
}

void loop()
{
  nh.spinOnce();
  delay(1);

}
