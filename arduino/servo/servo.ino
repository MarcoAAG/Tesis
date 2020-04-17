#include <Servo.h> 

Servo myservo;

void setup() 
{ 
  myservo.attach(9);
  myservo.writeMicroseconds(1575);  // set servo to mid-point
} 

void loop() {} 
