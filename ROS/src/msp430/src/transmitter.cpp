//! LINUX stuff
#include <fcntl.h>   /* File Control Definitions           */
#include <termios.h> /* POSIX Terminal Control Definitions */
#include <unistd.h>  /* UNIX Standard Definitions 	   */
#include <errno.h>   /* ERROR Number Definitions           */

//! c++ stuff
#include <iostream>
#include "stdint.h"

//! ROS stuff
#include <ros/ros.h>
#include <std_msgs/MultiArrayLayout.h>
#include <std_msgs/MultiArrayDimension.h>
#include <std_msgs/Int32MultiArray.h>
// #include "serial/serial.h"

class transmitter
{
public:
    uint16_t Centroid[2];

    transmitter(ros::NodeHandle *n);
    ~transmitter();
    void arrayCallback(const std_msgs::Int32MultiArray::ConstPtr &array);

private:
    ros::Subscriber sub;
    int fd; //* File descriptor
    int flag_error_sender;
    int bytes_writen;
    uint8_t coordinateX;
    uint8_t coordinateY;

    void serial_Init();
    void sendData();
};

transmitter::transmitter(ros::NodeHandle *n)
{
    serial_Init();
    sub = n->subscribe("coordinates", 100, &transmitter::arrayCallback, this);
}

transmitter::~transmitter()
{
    close(fd);
}

void transmitter::serial_Init()
{
    const char port[] = "/dev/ttyUSB0";
    std::cout << "Comunicación serial comenzando" << std::endl;
    fd = open(port, O_RDWR | O_NOCTTY | O_NDELAY); //* ttyUSB0 is the FT232 based USB2SERIAL Converter
                                                   //* O_RDWR   - Read/Write access to serial port
                                                   //* O_NOCTTY - No terminal will control the process
                                                   //* O_NDELAY -Non Blocking Mode,Does not care about-
                                                   //* -the status of DCD line,Open() returns immediatly
    if (fd == -1)
    {
        ROS_INFO("Error al abrir el puerto\nVerifique conexion");
    }
    else
    {
        ROS_INFO("Conectado al puerto con exito");
    }
}

void transmitter::sendData()
{
    bytes_writen = 0;
    coordinateX = 0xFF & Centroid[0];
    bytes_writen += write(fd, (unsigned char *)&coordinateX, sizeof(coordinateX));
    coordinateX = 0xFF & (Centroid[0] >> 8);
    bytes_writen += write(fd, (unsigned char *)&coordinateX, sizeof(coordinateX));

    coordinateY = 0xFF & Centroid[1];
    bytes_writen += write(fd, (unsigned char *)&coordinateY, sizeof(coordinateY));
    coordinateY = 0xFF & (Centroid[1] >> 8);
    bytes_writen += write(fd, (unsigned char *)&coordinateY, sizeof(coordinateY));

    std::cout << bytes_writen << " Bytes enviados" << std::endl;
}

void transmitter::arrayCallback(const std_msgs::Int32MultiArray::ConstPtr &array)
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
        sendData();
    }

    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}
int main(int argc, char **argv)
{
    ros::init(argc, argv, "transmitter_serial");
    ros::NodeHandle n_ros;
    transmitter host = transmitter(&n_ros);
    ros::spin();
    return 0;
}
