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
    void initialization();

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
    initialization();
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

    /*---------- Setting the Attributes of the serial port using termios structure --------- */

    struct termios SerialPortSettings; /* Create the structure                          */

    tcgetattr(fd, &SerialPortSettings); /* Get the current attributes of the Serial port */

    /* Setting the Baud rate */
    cfsetispeed(&SerialPortSettings, B9600); /* Set Read  Speed as 9600                       */
    cfsetospeed(&SerialPortSettings, B9600); /* Set Write Speed as 9600                       */

    /* 8N1 Mode */
    SerialPortSettings.c_cflag &= ~PARENB; /* Disables the Parity Enable bit(PARENB),So No Parity   */
    SerialPortSettings.c_cflag &= ~CSTOPB; /* CSTOPB = 2 Stop bits,here it is cleared so 1 Stop bit */
    SerialPortSettings.c_cflag &= ~CSIZE;  /* Clears the mask for setting the data size             */
    SerialPortSettings.c_cflag |= CS8;     /* Set the data bits = 8                                 */

    SerialPortSettings.c_cflag &= ~CRTSCTS;       /* No Hardware flow Control                         */
    SerialPortSettings.c_cflag |= CREAD | CLOCAL; /* Enable receiver,Ignore Modem Control lines       */

    SerialPortSettings.c_iflag &= ~(IXON | IXOFF | IXANY);         /* Disable XON/XOFF flow control both i/p and o/p */
    SerialPortSettings.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG); /* Non Cannonical mode                            */

    SerialPortSettings.c_oflag &= ~OPOST; /*No Output Processing*/

    /* Setting Time outs */
    SerialPortSettings.c_cc[VMIN] = 10;  /* Read at least 1 characters */
    SerialPortSettings.c_cc[VTIME] = 0; /* Wait indefinetly   */
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
    tcflush(fd, TCOFLUSH);
}

void transmitter::initialization()
{
    tcflush(fd, TCIFLUSH); /* Discards old data in the rx buffer            */

    int bytes_read = 0;
    char read_buffer[32];
    char write_buffer[] = "a";

    bytes_read = read(fd, &read_buffer, 32);
    while (read_buffer[0] != '@')
    {
        bytes_read = read(fd, &read_buffer, 32);

        if (bytes_read != -1)
        {
            std::cout << "Sincronizando . . ." << std::endl;
        }
    }
    std::cout << read_buffer[0] << std::endl;

    write(fd, write_buffer, sizeof(write_buffer));
    tcflush(fd, TCOFLUSH);
    std::cout << "La comunicación se ha sincronizado" << std::endl;
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
