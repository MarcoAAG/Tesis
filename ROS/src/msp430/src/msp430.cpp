//! c++ stuff
#include <iostream>
#include "stdint.h"

//! ROS stuff
#include <ros/ros.h>
#include <std_msgs/MultiArrayLayout.h>
#include <std_msgs/MultiArrayDimension.h>
#include <std_msgs/Int32MultiArray.h>
#include "serial/serial.h"

serial::Serial my_serial;

class UART
{
public:
    uint16_t Centroid[2];

    UART(ros::NodeHandle *n);
    ~UART();

    void arrayCallback(const std_msgs::Int32MultiArray::ConstPtr &array);
    void serialInit();
    void initialization();

private:
    ros::Subscriber sub;
    serial::Serial *pMyserial = &my_serial; //* objeto de la comunicacion serial
    uint8_t coordinateX;
    uint8_t coordinateY;
    int bytes_writen;

    void sendData();
};

UART::UART(ros::NodeHandle *n)
{
    serialInit();
    sub = n->subscribe("coordinates", 100, &UART::arrayCallback, this);
    initialization();
}

UART::~UART()
{
    pMyserial->close();
}

void UART::serialInit()
{
    pMyserial->setPort("/dev/ttyUSB0");
    pMyserial->setBaudrate(9600);
    pMyserial->open();
    pMyserial->flushInput();
    pMyserial->flushOutput();
    ROS_INFO("Microcontrolador Conectado\n");
}

void UART::initialization()
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
    std::cout << "Sincronizado" << std::endl;
}

void UART::sendData()
{
    uint8_t data_test = 240;

    bytes_writen = 0;

    coordinateX = 0xFF & Centroid[0];
    bytes_writen += pMyserial->write(&coordinateX, sizeof(coordinateX));
    // bytes_writen += pMyserial->write(&data_test, sizeof(coordinateX));
    coordinateX = 0xFF & (Centroid[0] >> 8);
    bytes_writen += pMyserial->write(&coordinateX, sizeof(coordinateX));

    coordinateY = 0xFF & Centroid[1];
    bytes_writen += pMyserial->write(&coordinateY, sizeof(coordinateX));
    coordinateY = 0xFF & (Centroid[1] >> 8);
    bytes_writen += pMyserial->write(&coordinateY, sizeof(coordinateX));

    std::cout << bytes_writen << " Bytes enviados" << std::endl;
}

void UART::arrayCallback(const std_msgs::Int32MultiArray::ConstPtr &array)
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
        if(Centroid[0] > 640)
        {
            Centroid[0] = 320;
        }
        if(Centroid[1] > 480)
        {
            Centroid[1] = 240;
        }
        sendData();
    }

    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "msp_UART");
    ros::NodeHandle n_ros;
    UART host = UART(&n_ros);
    ros::spin();
    return 0;
}