#include <ros.h>
#include <std_msgs/String.h>
#include <std_msgs/MultiArrayLayout.h>
#include <std_msgs/MultiArrayDimension.h>
#include <std_msgs/Int32MultiArray.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Float32.h>

//! Arduino stuff
#include <Servo.h>

#define referenceX 320
#define referenceY 240

//declarar errores
int last_error[2] = {0,0};
int error[2] = {0,0};
int sum_error[2] = {0,0};

//declarar variables de ganancias
double KP[2] = {0.0,0.0};
double KD[2] = {0.0 , 0.0};
double KI[2] = {0.0 , 0.0};

//declarar variable de control
double CTRL[2] = {0.0,0.0};

int PWM[2] = {0,0};

//variables para calcular tiempo de muestreo
unsigned long now;
double timeDelta;
unsigned long lastTime;


ros::NodeHandle nh; //Creamos el nodo de ROS

//* creamos objeto de clase servo
Servo myServo1;
Servo myServo2;

//Prototipo de funciones
void callBack(const std_msgs::Int32MultiArray &array);
void control(uint16_t inX, uint16_t inY);

//ros subscriber
ros::Subscriber<std_msgs::Int32MultiArray> sub("coordinates", callBack);

//Variables publicadoras 
std_msgs::Int32 ErrorX;
std_msgs::Int32 ErrorY;
ros::Publisher pub_ex("control/ErrorX", &ErrorX);
ros::Publisher pub_ey("control/ErrorY", &ErrorY);

void setup()
{
  nh.initNode();
  nh.subscribe(sub);
  nh.advertise(pub_ex);
  nh.advertise(pub_ey);

  //Motor 1 Pin 9
  //Motor 2 Pin 10

  myServo1.attach(9);
  myServo2.attach(10);
  myServo1.writeMicroseconds(1500); //iniciar el motor (no movimiento)
  myServo2.writeMicroseconds(1500); //iniciar el motor (no movimiento)
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
    control(X,Y);
}
void control(uint16_t inX, uint16_t inY)
{
  //obtener errores
  last_error[0] = error[0];
  last_error[1] = error[1];
  error[0] = referenceX - inX;
  error[1] = referenceY - inY;
  sum_error[0] += error[0];
  sum_error[1] += error[1];

  

  ErrorX.data = error[0];
  pub_ex.publish(&ErrorX); //Publicar error en coordenada X
  ErrorY.data = error[1];
  pub_ey.publish(&ErrorY); //Publicar error en coordenada Y

  //Establecemos ganancias
  KP[0]= (double)(analogRead(A0) * 0.2 / 1023.0);
  KP[1] = (double)(analogRead(A2) * 0.3 / 1023.0);
  KD[0] = 0.0;
  KD[1] = 0.0;
  KI[0] = (double)(analogRead(A1) * 0.00002 / 1023.0);
  KI[1] = (double)(analogRead(A3) * 0.00002 / 1023.0);
  

  //guardar el tiempo anterior
  lastTime = now;
  //obtener el tiempo actual
  now = millis();
  //calcular diferencia entre cada llamada
  timeDelta = (double)(now - lastTime);

  CTRL[0] = (KP[0] * error[0]) + (KD[0] * (error[0]- last_error[0])/timeDelta) + (KI[0]*sum_error[0]*timeDelta);
  CTRL[1] = (KP[1] * error[1]) + (KD[1] * (error[1]- last_error[1])/timeDelta) + (KI[1]*sum_error[1]*timeDelta);

  //Establecer entrada de control [0]
  if(CTRL[0] > 0)
  {
    PWM[0] = 1446 - (int)CTRL[0];
  }
  if(CTRL[0] < 0 )
  {
    PWM[0] = 1553 + (int)CTRL[0] * -1;
  }
  if(error[0] == 0 || CTRL[0] == 0)
  {
    PWM[0] = 1500;
  }

  //Establecer entrada de control [1]
  if(CTRL[1] > 0)
  {
    PWM[1] = 1543 + (int)CTRL[1];
  }
  if(CTRL[1] < 0 )
  {
    PWM[1] = 1439 + (int)CTRL[1];
  }
  if(error[1] == 0 || CTRL[1] == 0)
  {
    PWM[1] = 1500;
  }

  //Limites de PWM[1]
  if(PWM[1] < 1280)
  {
    PWM[1] = 1280;
  }
  if(PWM[1] > 1700)
  {
    PWM[1] = 1700;
  }

  //Limites de PWM[0]
  if(PWM[0] < 1294)
  {
    PWM[0] = 1294;
  }
  if(PWM[0] > 1705)
  {
    PWM[0] = 1705;
  }

  myServo2.writeMicroseconds(PWM[0]);
  myServo1.writeMicroseconds(PWM[1]);
  
}
