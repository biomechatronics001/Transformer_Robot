//#include <bluefruit.h>
#include <Servo.h>
#include <math.h>
//#include <KeyboardController.h>

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
bool rotate = true;
int servoAngle1 = 0;
int servoAngle2 = 0;
int servoAngle3 = 0;
int servoAngle4 = 0;
int deltaT = 8000 / 180;
char val;

//USBHost usb;
//
//// Attach Keyboard controller to USB
//KeyboardController keyboard(usb);


void setup() {
  // put your setup code here, to run once:
  servo1.attach(A2, 500, 2500);
  servo2.attach(5, 500, 2500);
  servo3.attach(6, 500, 2500);
  servo4.attach(9, 500, 2500);
  pinMode(2, INPUT);
  pinMode(4, INPUT);
  pinMode(7, INPUT);
  pinMode(8, INPUT);
  pinMode(10, INPUT);
  pinMode(11, INPUT);
  pinMode(12, INPUT);
  pinMode(13, INPUT);
  Serial.begin(115200);
  digitalWrite(2, LOW);
  digitalWrite(4, LOW);

  servo1.write(0);
  servo2.write(0);
  servo3.write(0);
  servo4.write(0);
  delay(3000);
}

void loop() {
  // put your main code here, to run repeatedly:
  //  test1();
  //  test2();
    test3();
  //  test4();
  //  usb.Task();
  //  test5();
  
  rotateMotor(0, 180);
//  rotate = true;
  if (Serial.available() > 0) {
    val = Serial.read();
    if (val == '1') {
      rotate = true;
    }
  }
  rotateMotor(180, 0);


}

void test5() {
  //  Serial.print("Pressed:  ");
  //  Serial.print(keyboard.getKey());
  //  Serial.println();
  if (Serial.available() > 0) {
    val = Serial.read();
    if (val == '1') {
      if (servoAngle1 > 180) {
        // do nothing
      }
      else {
        servoAngle1 += 5;
        if (servoAngle1 > 180) {
          servoAngle1 = 180;
        }
        servo1.write(servoAngle1);
        delay(deltaT);
      }
    }

  }

}

void rotateMotor(int initialAngle, int targetAngle) {
  if (rotate == true && initialAngle == servoAngle1) {
    Serial.println("Motor is rotating");
//    servo1.write(initialAngle);
    if (initialAngle < targetAngle) {
      for (int angle = initialAngle; angle <= targetAngle; angle++) {
        servo1.write(angle);
        servoAngle1 = angle;
        delay(6000 / 180);
      }
    } else if (initialAngle > targetAngle) {
      for (int angle = initialAngle; angle >= targetAngle; angle--) {
        servo1.write(angle);
        servoAngle1 = angle;
        delay(6000 / 180);
      }
    }
    rotate = false;
  }
}

void test4() {
  if (digitalRead(2) == HIGH) {
    Serial.print("Pin2 = HIGH, angle = ");
    Serial.println(servoAngle1);
    if (servoAngle1 > 180) {
      // do nothing
    }
    else {
      servoAngle1++;
      if (servoAngle1 > 180) {
        servoAngle1 = 180;
      }
      servo1.write(servoAngle1);
      delay(deltaT);
    }

  } else if (digitalRead(4) == HIGH) {
    Serial.print("Pin4 = HIGH, angle = ");
    Serial.println(servoAngle1);
    if (servoAngle1 < 0) {
      // do nothing
    }
    else {
      servoAngle1--;
      if (servoAngle1 < 0) {
        servoAngle1 = 0;
      }
      servo1.write(servoAngle1);
      delay(deltaT);
    }

  }
}

void test1() {
  if (digitalRead(2) == HIGH) {
    if (servoAngle1 > 180) {
      // do nothing
    }
    else {
      for (int angle = servoAngle1; angle <= 180; angle++) {
        if (digitalRead(2) != HIGH) {
          break;
        }
        servo1.write(angle);
        servoAngle1 = angle;
        delay(deltaT);
      }
    }

  } else if (digitalRead(4) == HIGH) {
    if (servoAngle1 < 0) {
      // do nothing
    }
    else {
      for (int angle = servoAngle1; angle >= 0; angle--) {
        if (digitalRead(4) != HIGH) {
          break;
        }
        servo1.write(angle);
        servoAngle1 = angle;
        delay(deltaT);
      }
    }

  }
}

void test2() {
  int a = 1;
  for (int b = a; b <= 5; b++) {
    a = b;
    Serial.println(a);
    delay(1000);
  }
}

void test3() {
  for (int angle = 0; angle <= 180; angle++) {
    servo1.write(angle);
    delay(6000 / 180);
  }
  delay(1000);
  for (int angle = 180; angle >= 0; angle--) {
    servo1.write(angle);
    delay(6000 / 180);
  }
  delay(3000);
}
