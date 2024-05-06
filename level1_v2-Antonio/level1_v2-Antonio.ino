//#include <bluefruit.h>
#include <Servo.h>
#include <math.h>
//#include <KeyboardController.h>

// Instructions are of the type: 
// 1,2:90;3:180

String string;
const int N_motors = 6;
const int Pins[] = {3, 5, 6, 9, 10, 11};
bool flag[N_motors];
bool keepOnGoing = false;
Servo servo[N_motors];
int servo_angle[N_motors];
int rotation_angles[N_motors];

int max_angle = 180;
int duration = 10000;
int deltaT = duration / max_angle;

void setup() {
  Serial.begin(115200);
  for (int k=0; k<N_motors; k++) {
    servo[k].attach(Pins[k], 500, 2500);
    servo_angle[k] = 0;
    servo[k].write(servo_angle[k]);    
    } 
  resetFlags();
}

void loop() {

  if (Serial.available() > 0) {
    keepOnGoing = true;
    string = Serial.readString();
    string = string.substring(0,string.length()-1);
    Serial.println("----------------------------"); Serial.print("Received data: "); Serial.println(string);

    int n = countPieces(string, ';');

    String pieces[n];
    for (int i=0; i<n; i++) {
      pieces[i] = splitString(string, ';', n, i);
    }

    String motors_str[n];
    String angles_str[n];
    int n_i[n];
    int angles[n];
    
    for (int i=0; i<n; i++) {
      motors_str[i] = pieces[i].substring(0,pieces[i].indexOf(':'));
      angles_str[i] = pieces[i].substring(pieces[i].indexOf(':')+1);
      angles[i] = angles_str[i].toInt();
      if (abs(angles[i])!=90 && abs(angles[i])!=180) {
        Serial.print("!!!!!!!!!! "); Serial.print("ERROR in angle value"); Serial.println(" !!!!!!!!!!");
        keepOnGoing = false;
      }
      n_i[i] = countPieces(motors_str[i], ',');   
    }

    if (keepOnGoing == true) {
      //int motors[n][max_int(n_i, n)];
      int motor_idx;
      
      for (int i=0; i<n; i++) {
        motors_str[i] = pieces[i].substring(0,pieces[i].indexOf(':'));
        Serial.print("==="); Serial.print("ANGLE "); Serial.println(angles[i]);
        
        for (int j=0; j<n_i[i]; j++) {                
          motor_idx = splitString(motors_str[i], ',', n_i[i], j).toInt();
          flag[motor_idx] = true;
          rotation_angles[motor_idx] = angles[i];
          Serial.print("MOTOR "); Serial.println(motor_idx);    
        } 
      }
  
      //int max_angle = max_int(angles, n);
      //Serial.print("Max angle = "); Serial.println(max_angle);
      
      //rotateMotors();
      
      int temp_angle[N_motors];
      for (int i=0; i<N_motors; i++) {
        temp_angle[i] = servo_angle[i];
      }
      
      for (int step_ = 0; step_ <= 180; step_++) {
        for (int i=0; i<N_motors; i++) {
          if (flag[i]==true){
            if (servo_angle[i]==0) {
              temp_angle[i] = step_;
              servo[i].write(temp_angle[i]);
              if (temp_angle[i] == rotation_angles[i]) {
                servo_angle[i] = temp_angle[i];
                flag[i] = false;
                Serial.print("A. Motor "); Serial.print(i); Serial.print(" moved to angle "); Serial.println(servo_angle[i]);
              }
            }
            else if (servo_angle[i]==180) {
              temp_angle[i] = rotation_angles[i]-step_;
              servo[i].write(temp_angle[i]);
              if (temp_angle[i] == 180-rotation_angles[i]) {
                servo_angle[i] = temp_angle[i];
                flag[i] = false;
                Serial.print("B. Motor "); Serial.print(i); Serial.print(" moved to angle "); Serial.println(servo_angle[i]);
              }           
            }
            else if (servo_angle[i]==90) {
              if (rotation_angles[i]<0) {
                temp_angle[i] = servo_angle[i]-step_;
              }
              else {
                temp_angle[i] = servo_angle[i]+step_;
              }            
              servo[i].write(temp_angle[i]);
              if (temp_angle[i] == 0 || temp_angle[i] == 180) {
                servo_angle[i] = temp_angle[i];
                flag[i] = false;
                Serial.print("C. Motor "); Serial.print(i); Serial.print(" moved to angle "); Serial.println(servo_angle[i]);
              }           
            }
          }
        }
        delay(deltaT);    
      }
      Serial.println(">> Ready");
    }     
    }
}

void rotateMotors(int motors, int angles) {
  
}

void parseData() {
    
}

int max_int(int array_[], int siz) {
  int max_val = array_[0];
  for (int i=1; i<siz; i++) {
    if (array_[i]>max_val) {
      max_val = array_[i];
    }
  }
  return max_val;
}

int countPieces(String string, char delimiter) {
  int n = 1;
    for (byte i=0; i < string.length(); i++) {
    if (string[i] == delimiter) {
      n++;
      }
    }
   return n;
}

void resetFlags() {
  for (int i=0; i<N_motors; i++) {
      flag[i]=false;
  }
}

String splitString(String string, char delimiter, int n, int j) {
    int counter = 1;
    int index_array[n+1];
    String pieces[n];
    index_array[0] = 0;
    for (byte i=0; i < string.length(); i++) {
    if (string[i] == delimiter) {
      index_array[counter] = i+1;
      counter++;
      }
    }
    index_array[counter] = string.length();
    
    if (j<n-1) {
      pieces[j] = string.substring(index_array[j],index_array[j+1]-1);
    }
    else {
      pieces[j] = string.substring(index_array[j],index_array[j+1]);
    }

    return pieces[j];
}
