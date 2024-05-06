//#include <bluefruit.h>
#include <Servo.h>
#include <math.h>
//#include <KeyboardController.h>

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;
bool rotate1 = false;
bool rotate2 = false;
bool rotate3 = false;
bool rotate4 = false;
bool rotate5 = false;
int servoAngle1 = 0;
int servoAngle2 = 0;
int servoAngle3 = 0;
int servoAngle4 = 0;
int servoAngle5 = 0;
int targetAngle = 180;
int deltaT = 10000 / targetAngle;
String vals;

//USBHost usb;
//
//// Attach Keyboard controller to USB
//KeyboardController keyboard(usb);

void setup() {
  // put your setup code here, to run once:
  servo1.attach(3, 500, 2500);
  servo2.attach(5, 500, 2500);
  servo3.attach(6, 500, 2500);
  servo4.attach(9, 500, 2500);
  servo5.attach(10, 500, 2500);

  Serial.begin(115200);

  servo1.write(0);
  servo2.write(0);
  servo3.write(0);
  servo4.write(0);
  servo5.write(0);
  
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  //  test1();
  //  test2();
  //  test3();
  //  test4();
  //  test5();
  //  usb.Task();

  if (Serial.available() > 0) {
    vals = Serial.readString();
    Serial.print("Received data: ");
    Serial.println(vals);
    
    if (vals.indexOf('1')!=-1) {
      rotate1 = true;
    }
    if (vals.indexOf('2')!=-1) {
      rotate2 = true;
    }
    if (vals.indexOf('3')!=-1) {
      rotate3 = true;
    }
    if (vals.indexOf('4')!=-1) {
      rotate4 = true;
    }
    if (vals.indexOf('5')!=-1) {
      rotate5 = true;
    }
    //rotateMotor1();
    rotateMotors();
  }

}

void rotateMotors() {

    int angle1 = servoAngle1;
    int angle2 = servoAngle2;
    int angle3 = servoAngle3;
    int angle4 = servoAngle4;
    int angle5 = servoAngle5;    
  
    for (int step = 0; step <= targetAngle; step++)
    {
    if (rotate1==true) {
      if (servoAngle1==0) {
        angle1 = step;
        servo1.write(angle1);
      }
      else {
        angle1 = targetAngle-step;
        servo1.write(angle1);
        }      
    }

    if (rotate2==true) {
      if (servoAngle2==0) {
        angle2 = step;
        servo2.write(angle2);
      }
      else {
        angle2 = targetAngle-step;
        servo2.write(angle2);
        }      
    }

    if (rotate3==true) {
      if (servoAngle3==0) {
        angle3 = step;
        servo3.write(angle3);
      }
      else {
        angle3 = targetAngle-step;
        servo3.write(angle3);
        }      
    }

    if (rotate4==true) {
      if (servoAngle4==0) {
        angle4 = step;
        servo4.write(angle4);
      }
      else {
        angle4 = targetAngle-step;
        servo4.write(angle4);
        }      
    }
    if (rotate5==true) {
      if (servoAngle5==0) {
        angle5 = step;
        servo5.write(angle5);
      }
      else {
        angle5 = targetAngle-step;
        servo5.write(angle5);
        }      
    }
    delay(deltaT);
    }
    
    servoAngle1 = angle1;
    servoAngle2 = angle2;
    servoAngle3 = angle3;
    servoAngle4 = angle4;
    servoAngle5 = angle5;
    Serial.print("Angle 1: ");
    Serial.println(servoAngle1);
    Serial.print("Angle 2: ");
    Serial.println(servoAngle2);
    Serial.print("Angle 3: ");
    Serial.println(servoAngle3);
    Serial.print("Angle 4: ");
    Serial.println(servoAngle4);
    Serial.print("Angle 5: ");
    Serial.println(servoAngle5);
       
    rotate1 = false;
    rotate2 = false;
    rotate3 = false;
    rotate4 = false;
    rotate5 = false;
}

void rotateMotor1() {
  if (rotate1 == true) {
    Serial.println("Entered rotate 1 function");
    if (servoAngle1==0) {
      for (int angle = 0; angle <= 180; angle++) {
        servo1.write(angle);
        delay(deltaT);
      servoAngle1 = angle;
      }
    } else if (servoAngle1==180) {
      for (int angle = 180; angle >= 0; angle--) {
        servo1.write(angle);
        delay(deltaT);
      servoAngle1 = angle;
      }
    }
    rotate1 = false;
  }
}
