// ROS stuff
#include <ros.h>
#include <std_msgs/String.h>
#include <std_msgs/MultiArrayLayout.h>
#include <std_msgs/MultiArrayDimension.h>
#include <std_msgs/Int32MultiArray.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Float32.h>

//Arduino stuff
#include <Servo.h>

//init Node
ros::NodeHandle nh;

//Init Servo
Servo myServo1;
Servo myServo2;

//define reference
#define referenceX 320
#define referenceY 240

//Init variables
int16_t error[2] = {0.0};
int16_t lasterror[2] = {0.0};
int16_t sumerror[2] = {0.0};

//Init gains
float Kp[2] = {0.0, 0.0};
float Ki[2] = {0.0, 0.0};
float Kd[2] = {0.0, 0.0};

//Init PID
int16_t PID[2] = {0, 0};

void callBack(const std_msgs::Int32MultiArray &array);
void control(uint16_t _X, uint16_t _Y);
bool getTurn(int16_t in);
void sendPWM(bool dir, int16_t PID, bool ID);

ros::Subscriber<std_msgs::Int32MultiArray> sub("coordinates", callBack);
std_msgs::Int32 ErrorX;
std_msgs::Int32 ErrorY;
ros::Publisher pubEX("PID/ErrorX", &ErrorX);
ros::Publisher pubEY("PID/ErrorY", &ErrorY);
// std_msgs::Int32 u;
// ros::Publisher pub_u("PID/u", &u);
// std_msgs::Float32 PID_Kp;
// ros::Publisher pub_Kp("PID/Kp", &PID_Kp);

unsigned long now;
double timeChange;
unsigned long lastTime;

void setup()
{
    nh.initNode();
    nh.subscribe(sub);
    nh.advertise(pubEX);
    nh.advertise(pubEY);
    // myServo1.writeMicroseconds(1537);

    // nh.advertise(pub_Kp);

    //setting Servo
    /*
  *MOTOR 1 PIN 9
  *MOTOR 2 PIN 10
  */
    myServo1.attach(9);
    myServo2.attach(10);
    myServo1.writeMicroseconds(1450);
    myServo2.writeMicroseconds(1450);
}

void loop()
{
    nh.spinOnce();
    delay(1);
}

void callBack(const std_msgs::Int32MultiArray &array)
{
    nh.loginfo("recibiendo datos");
    uint16_t X;
    uint16_t Y;
    X = array.data[0];
    Y = array.data[1];
    control(X, Y);
}

void control(uint16_t _X, uint16_t _Y)
{
    bool sign1;
    bool sign2;

    //save last time
    lastTime = now;
    //get current time
    now = millis();
    //get time difference
    timeChange = (double)(now - lastTime);

    //save last error
    lasterror[0] = error[0];
    lasterror[1] = error[1];

    //get new error
    error[0] = referenceX - _X;
    error[1] = referenceY - _Y;

    ErrorX.data = error[0];
    pubEX.publish(&ErrorX);
    ErrorY.data = error[1];
    pubEY.publish(&ErrorY);

    //sum all errors
    sumerror[0] += error[0];
    sumerror[1] += error[1];

    sign1 = getTurn(error[0]);
    sign2 = getTurn(error[1]);

    //Kp[0] = 0.0;
    // Kp[1] = 1.0;
    Ki[0] = 0.0;
    Ki[1] = 0.0;
    //Kd[0] = 0.0;
    // Kd[1] = 0.0;

    Kp[1] = (float)(analogRead(A0) * 1.2 / 1023.0);
    //Ki[1] = (float)(analogRead(A1) * 0.1 / 1023.0);
    Kd[1] = (float)(analogRead(A2) * 50.0 / 1023.0);

    Kp[0] = (float)(analogRead(A3) * 1.0 / 1023.0);
    //Ki[0] = (float)(analogRead(A4) * 0.1 / 1023.0);
    Kd[0] = (float)(analogRead(A5) * 50.0 / 1023.0);

    // PID_Kp.data = Kd[1];
    // pub_Kp.publish(&PID_Kp);

    PID[0] = Kp[0] * error[0] + (Kd[0] * (error[0] - lasterror[0]) / timeChange) + (Ki[0] * sumerror[0] * timeChange);
    PID[1] = Kp[1] * error[1] + (Kd[1] * (error[1] - lasterror[1]) / timeChange) + (Ki[1] * sumerror[1] * timeChange);

    sendPWM(sign1, PID[0], 1);
    sendPWM(sign2, PID[1], 0);
}

bool getTurn(int16_t in)
{
    bool ret;
    ret = (in > 0) ? 1 : 0;
    return ret;
}
void sendPWM(bool dir, int16_t PID, bool ID)
{
    /*
    *ID 0 -> MOTOR 1
    *ID 1 -> MOTOR 2
    *dir 0 -> TURN LEFT -> NEGATIVE ERROR
    *dir 1 -> TURN  RIGHT -> POSITIVE ERROR
    */

    //u.data = dir;

    if (ID == 0)
    {
        // u.data = PID;
        // pub_u.publish(&u);

        if (dir == 0)
        {
            if (PID == 0)
            {
                PID = 1500;
            }
            else
            {
                PID = (PID * -1) + 1542;
                PID = (PID > (1542 + referenceY)) ? (1542 + referenceY) : PID;
                PID = (PID < 1542) ? 1540 : PID;
            }
        }
        else
        {
            if (PID == 0)
            {
                PID = 1500;
            }
            else
            {
                PID = 1428 - PID;
                PID = (PID > 1428) ? 1428 : PID;
                PID = (PID < (1428 - referenceY)) ? (1428 - referenceY) : PID;
            }
        }

        //u.data = PID;
        //pub_u.publish(&u);
        myServo1.writeMicroseconds((uint32_t)PID);
    }

    if (ID == 1)
    {
        // u.data = PID;
        // pub_u.publish(&u);

        if (dir == 0)
        {
            if (PID == 0)
            {
                PID = 1500;
            }
            else
            {
                PID = (PID * -1) + 1549;
                PID = (PID > (1549 + referenceX)) ? (1549 + referenceX) : PID;
                PID = (PID < 1549) ? 1540 : PID;
            }
        }
        else
        {
            if (PID == 0)
            {
                PID = 1500;
            }
            else
            {
                PID = 1440 - PID;
                PID = (PID > 1440) ? 1441 : PID;
                PID = (PID < (1439 - referenceX)) ? (1439 - referenceX) : PID;
            }
        }

        //u.data = PID;
        //pub_u.publish(&u);
        myServo2.writeMicroseconds((uint32_t)PID);
    }
}
