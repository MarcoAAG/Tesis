//! LINUX stuff
#include <fcntl.h>   /* File Control Definitions           */
#include <termios.h> /* POSIX Terminal Control Definitions */
#include <unistd.h>  /* UNIX Standard Definitions 	   */
#include <errno.h>   /* ERROR Number Definitions           */

//! c++ stuff
#include <iostream>
#include "stdint.h"

int main(int argc, char const *argv[])
{
    std::cout << "Comunicacion Serial" << std::endl;
    int fd; /*File Descriptor*/

    fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY); /* ttyUSB0 is the FT232 based USB2SERIAL Converter   */
                                                             /* O_RDWR   - Read/Write access to serial port       */
                                                             /* O_NOCTTY - No terminal will control the process   */
                                                             /* O_NDELAY -Non Blocking Mode,Does not care about-  */
                                                             /* -the status of DCD line,Open() returns immediatly */

    if (fd == -1) /* Error Checking */
        std::cout << "Error al abrir el puerto USB0" << std::endl;
    else
        std::cout << "El puerto USB0 se abrio satisfactoriamente" << std::endl;

    //*---------- Setting the Attributes of the serial port using termios structure ---------
    struct termios SerialPortSettings; /* Create the structure                          */

    tcgetattr(fd, &SerialPortSettings); /* Get the current attributes of the Serial port */

    cfsetispeed(&SerialPortSettings, B9600); /* Set Read  Speed as 9600                       */
    cfsetospeed(&SerialPortSettings, B9600); /* Set Write Speed as 9600                       */

    SerialPortSettings.c_cflag &= ~PARENB; /* Disables the Parity Enable bit(PARENB),So No Parity   */
    SerialPortSettings.c_cflag &= ~CSTOPB; /* CSTOPB = 2 Stop bits,here it is cleared so 1 Stop bit */
    SerialPortSettings.c_cflag &= ~CSIZE;  /* Clears the mask for setting the data size             */
    SerialPortSettings.c_cflag |= CS8;     /* Set the data bits = 8                                 */

    SerialPortSettings.c_cflag &= ~CRTSCTS;       /* No Hardware flow Control                         */
    SerialPortSettings.c_cflag |= CREAD | CLOCAL; /* Enable receiver,Ignore Modem Control lines       */

    SerialPortSettings.c_iflag &= ~(IXON | IXOFF | IXANY);         /* Disable XON/XOFF flow control both i/p and o/p */
    SerialPortSettings.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG); /* Non Cannonical mode                            */

    SerialPortSettings.c_oflag &= ~OPOST; /*No Output Processing*/

    if ((tcsetattr(fd, TCSANOW, &SerialPortSettings)) != 0) /* Set the attributes to the termios structure*/
        std::cout << "Error al configurar puerto" << std::endl;
    else
        std::cout << " BaudRate = 9600 \n StopBits = 1 \n Parity = none" << std::endl;

    //* ------------------------------ Write data to serial port ---------------------------

    // char write_buffer[] = "B"; /* Buffer containing characters to write into port	     */
    uint8_t data_send = 255;    
    int bytes_written = 0;     /* Value for storing the number of bytes written to the port */

    bytes_written = write(fd, (unsigned char *)&data_send, sizeof(data_send)); /* use write() to send data to port                                            */
                                                                   /* "fd"                   - file descriptor pointing to the opened serial port */
                                                                   /*	"write_buffer"         - address of the buffer containing data	            */
                                                                   /* "sizeof(write_buffer)" - No of bytes to write  */

    std::cout << bytes_written << " Bytes writen to USB0" << std::endl;
    close(fd); /* Close the serial port */

    return 0;
}
