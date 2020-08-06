/* ROS stuff  */
#include "ros/ros.h"
#include "serial/serial.h"
#include "std_msgs/MultiArrayLayout.h"
#include "std_msgs/MultiArrayDimension.h"
#include "std_msgs/Int32MultiArray.h"

/* C++ stuff  */
#include <sstream>
// #include <iostream>
#include <vector>
#include <string>

#include <stdio.h>
#include <stdlib.h>

serial::Serial my_serial;

class data_acquisition
{
private:
    /* data */
    ros::Subscriber sub;
    serial::Serial *pMyserial = &my_serial;

public:
    uint16_t Centroid[2];
    std::string data_received = "";

    data_acquisition(ros::NodeHandle *n);
    ~data_acquisition();
    void arrayCallback(const std_msgs::Int32MultiArray::ConstPtr &array);
    void serialInit(void);
    void initialization(void);
};

data_acquisition::data_acquisition(ros::NodeHandle *n)
{
    sub = n->subscribe("coordinates", 100, &data_acquisition::arrayCallback, this);
    serialInit();
    initialization();
}

data_acquisition::~data_acquisition()
{
    pMyserial->close();
}

void data_acquisition::serialInit(void)
{
    pMyserial->setPort("/dev/ttyUSB0");
    pMyserial->setBaudrate(9600);
    pMyserial->open();
    pMyserial->flushInput();
    pMyserial->flushOutput();
    ROS_INFO("Microcontrolador Conectado\n");
}

void data_acquisition::initialization(void)
{
    std::string received_char;

    if (pMyserial->waitReadable())
    {
        received_char = pMyserial->readline();
    }
    while (received_char != "@")
    {
        if (pMyserial->waitReadable())
        {
            received_char = pMyserial->readline();
        }
    }
    // std::cout << received_char << std::endl;
    pMyserial->write("a");
    pMyserial->flushInput();
    pMyserial->flushOutput();
    std::cout << "a fue enviado" << std::endl;
}

void data_acquisition::arrayCallback(const std_msgs::Int32MultiArray::ConstPtr &array)
{
    try
    {

        int i = 0;

        //Receive coordinates
        for (std::vector<int>::const_iterator it = array->data.begin(); it != array->data.end(); ++it)
        {
            Centroid[i] = *it;
            i++;
        }
        // pMyserial->write("6");
        // pMyserial->flushOutput();
        // pMyserial->write("4");
        // pMyserial->flushOutput();
        // pMyserial->write("0");
        /*Transform int to string*/
        std::stringstream ss_x;
        ss_x << Centroid[0];
        std::string X_str = ss_x.str();
        // std::cout << X_str.c_str() << std::endl;

        pMyserial->write(X_str.c_str());
        pMyserial->flushOutput();
        // std::cout << "Transmitiendo datos" << std::endl;
        if (pMyserial->waitReadable())
        {
            data_received = pMyserial->readline();
        }
        std::cout << data_received << std::endl;
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
    ros::init(argc, argv, "msp430_serial");
    ros::NodeHandle n;
    data_acquisition mspROS = data_acquisition(&n);
    ros::spin();

    return 0;
}
