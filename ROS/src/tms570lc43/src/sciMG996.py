#!/usr/bin/env python
import rospy
from std_msgs.msg import Int32MultiArray
import serial
import io

data = ''
XY = ''

# mySerial = serial.Serial('/dev/ttyACM0',115200)
mySerial = serial.Serial()


def callback(data):
    # print(data.data[0])
    XY = str(data.data[0]) + str(data.data[1])
    mySerial.write(XY)
    data = mySerial.readline()
    print(data)

    # print("Sending")


def listener():

    rospy.init_node('listener', anonymous=True)

    rospy.Subscriber("coordinates", Int32MultiArray, callback)

    # spin() simply keeps python from exiting until this node is stopped
    rospy.spin()


if __name__ == '__main__':

    mySerial.port = "/dev/ttyACM0"
    mySerial.baudrate = 115200
    mySerial.bytesize = serial.EIGHTBITS
    mySerial.parity = serial.PARITY_NONE
    mySerial.stopbits = serial.STOPBITS_TWO
    mySerial.timeout = 0.01
    mySerial.xonxoff = False
    mySerial.rtscts = False
    mySerial.dsrdtr = False
    mySerial.writeTimeout = 0.01

    try:
        mySerial.open()
    except Exception, e:
        print "error open serial port: " + str(e)
        exit()

    mySerial.flushInput()
    mySerial.flushOutput()
    listener()

    # mySerial.open()
    # mySerial.write('@')
    # data = mySerial.readline()
    # print(data)
    # listener()
