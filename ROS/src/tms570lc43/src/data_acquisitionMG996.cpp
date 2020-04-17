#include "ros/ros.h"
#include "serial/serial.h"
#include <sstream>
#include <iostream>
#include "std_msgs/MultiArrayLayout.h"
#include "std_msgs/MultiArrayDimension.h"
#include "std_msgs/Int32MultiArray.h"
#include <vector>

int CENTROID[2];
bool FLAG = 0;

ros::Subscriber sub;

void arrayCallback(const std_msgs::Int32MultiArray::ConstPtr &array);
void main_serial(serial::Serial *pSerial);
void serialInit(serial::Serial *pSerial);
void check_for_string(serial::Serial *pSerial, std::string expected_string);

int main(int argc, char **argv)
{
    /**
     * ********************************************************************************
     * SERIAL INIT
     * ********************************************************************************
    **/
    std::string port;
    int baudrate;
    std::string received_line = "";
    uint8_t buffer[1];

    port = "/dev/ttyACM0";
    baudrate = 115200;

    std::cout << "Puerto Conectado" << std::endl;
    ROS_INFO("Puerto conectado");

    serial::Serial my_serial(port, baudrate);
    serial::Serial *pMySerial = &my_serial;
    my_serial.close();
    my_serial.open();
    main_serial(pMySerial);

    /**
     * ********************************************************************************
     * ROS INIT AND SUBSCRIBER
     * ********************************************************************************
    **/
    ros::init(argc, argv, "tms570lc43_serial");
    ros::NodeHandle n;
    sub = n.subscribe("coordinates", 100, arrayCallback);
    ros::spin();

    return 0;
}

void arrayCallback(const std_msgs::Int32MultiArray::ConstPtr &array)
{
    /**
     * ********************************************************************************
     * COORDINATES RECEIVED
     * ********************************************************************************
    **/
    int i = 0;
    for (std::vector<int>::const_iterator it = array->data.begin(); it != array->data.end(); ++it)
    {
        CENTROID[i] = *it;
        i++;
    }
    // std::cout << CENTROID << std::endl;
    // main_serial();
}
void main_serial(serial::Serial *pSerial)
{
    if (FLAG == 0)
    {
        /**
     * ********************************************************************************
     * SERIAL COMMUNICATION
     * ********************************************************************************
    **/
        // my_serial.write("@");
        pSerial->write("@");
        std::cout << "@ has been sent" << std::endl;
        // check_for_string(pMySerial, "OK\n");
        check_for_string(pSerial, "OK\n");
        FLAG = 1;
    }
}
void check_for_string(serial::Serial *pSerial, std::string expected_string)
{
    std::string received_line = "";
    while (received_line != expected_string)
    {
        if (pSerial->waitReadable())
        {
            received_line = pSerial->readline();
        }
    }
    std::cout << received_line << std::endl;
}
// void serialInit(serial::Serial *pSerial)
// {
//     pSerial->write("@");
// }