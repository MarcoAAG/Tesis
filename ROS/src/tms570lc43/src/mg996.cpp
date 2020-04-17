/* ROS stuff  */
#include "ros/ros.h"
#include "serial/serial.h"
#include "std_msgs/MultiArrayLayout.h"
#include "std_msgs/MultiArrayDimension.h"
#include "std_msgs/Int32MultiArray.h"

/* C++ stuff  */
#include <sstream>
#include <iostream>
#include <vector>
#include <string>

#include <stdio.h>
#include <stdlib.h>

serial::Serial my_serial("/dev/ttyACM0", 115200);

class sciROS
{
private:
    ros::Subscriber sub;
    serial::Serial *pMyserial = &my_serial;

public:
    uint16_t CENTROID[2];

    sciROS(ros::NodeHandle *n); //Constructor
    ~sciROS();                  //Destructor
    void arrayCallback(const std_msgs::Int32MultiArray::ConstPtr &array);
    void serialInit();
    void sendData();
};

sciROS::sciROS(ros::NodeHandle *n)
{
    sub = n->subscribe("coordinates", 100, &sciROS::arrayCallback, this);
    serialInit();
}

sciROS::~sciROS()
{
    pMyserial->close();
}

void sciROS::serialInit()
{
    std::string received_line = "";

    pMyserial->close();
    pMyserial->open();
    pMyserial->flushInput();
    pMyserial->flushOutput();
    ROS_INFO("Microcontrolador Conectado\n");
    pMyserial->write("@");
    if (pMyserial->waitReadable())
    {
        received_line = pMyserial->readline();
        ROS_INFO("%s\n", received_line.c_str());
    }
}

void sciROS::sendData()
{
    pMyserial->flush();

    std::string received_line = "";

    /*Transform int to string*/
    std::stringstream ss_x;
    ss_x << CENTROID[0];
    std::string X_str = ss_x.str();

    std::stringstream ss_y;
    ss_y << CENTROID[1];
    std::string Y_str = ss_y.str();

    /* concatenate X and Y  */
    std::string XY = X_str + Y_str;

    //Send X and Y ina single sring
    pMyserial->write(XY.c_str());
    pMyserial->flush();
    if (pMyserial->waitReadable())
    {
        received_line = pMyserial->readline();
        ROS_INFO("%s\n", received_line.c_str());
    }
}

void sciROS::arrayCallback(const std_msgs::Int32MultiArray::ConstPtr &array)
{
    try
    {
        int i = 0;

        //Receive coordinates
        for (std::vector<int>::const_iterator it = array->data.begin(); it != array->data.end(); ++it)
        {
            CENTROID[i] = *it;
            i++;
        }

        sendData(); //send data from subscriber
    }

    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}

int main(int argc, char **argv)
{
    /**
     * ********************************************************************************
     * ROS INIT AND SUBSCRIBER
     * ********************************************************************************
    **/
    ros::init(argc, argv, "tms570lc43_serial");
    ros::NodeHandle n;
    sciROS mysci = sciROS(&n);
    ros::spin();

    return 0;
}