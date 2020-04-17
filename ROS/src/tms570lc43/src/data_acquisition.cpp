#include "ros/ros.h"
#include "serial/serial.h"
#include <sstream>

int main(int argc, char **argv)
{
    ros::init(argc, argv, "tms570lc43_serial");
    std::string port;
    int baudrate;
    uint8_t buffer[1];
    std::string received_line = "";
    uint16_t data_received;
    float angle;

    ros::param::get("/MRS900_serial/port", port);
    ros::param::get("/MRS900_serial/baudrate", baudrate);

    serial::Serial my_serial("/dev/ttyACM0", 115200);
    serial::Serial *pMySerial = &my_serial;
    my_serial.close();
    my_serial.open();

    while (1)
    {
        if (pMySerial->waitReadable())
        {
            received_line = pMySerial->readline();
            // data_received = std::stoul(received_line, nullptr, 16);
            // angle = (float)(data_received * 360.0 / 8192.0);
            // std::cout << angle << std::endl;
            std::cout << received_line << std::endl;

        }
    }

    return 0;
}