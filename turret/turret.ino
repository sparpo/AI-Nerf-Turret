#include <Servo.h>
//Absolute maximum angles for servos
#define MAX_PITCH 170
#define MIN_PITCH 40
#define MAX_YAW   270
#define MIN_YAW   0

//Signal pins for servos
#define pitch_pin 3
#define yaw_pin   5

//motor signal pins
#define loader_motor_pin  10//loads nerf bullets from magazine to driver motors
#define driver_motor_pin  11//drives nerf bullets out of gun

#define loader_sense_pin  A0 //detects position of loader. 

//LED display pins
#define safety_led_pin  8
#define warning_led_pin 7
#define danger_led_pin  6

int driving = LOW;
int loading = LOW;

Servo pitchServo;//up-down
Servo yawServo;//left-right

int pitch = 90;//horizontal
int yaw = 135;//middle angle


char Incoming_value = 0; //Variable for storing Incoming_value

int servo270(int A) {//servo library uses angles 0-180
  int b = map(A,0,270,0,180);
  return b;
}
void addPitch(int deg) {// up
   pitch += deg; //add to pitch
    
  if(pitch > MAX_PITCH) { //if more than max val
    pitch = MAX_PITCH;
  } else if(pitch < MIN_PITCH) { //if less than min
    pitch = MIN_PITCH;
  }
  
  pitchServo.write(servo270(pitch));//write to servo
}

void addYaw(int deg){ //left
  yaw += deg;
    if(yaw > MAX_YAW) { //if more than max val
    yaw = MAX_YAW;
  } 
  if(yaw < MIN_YAW) { //if less than min
    yaw = MIN_YAW;
  }
  yawServo.write(servo270(yaw));
  
}

void setYaw(int deg){ //set yaw
  yaw = deg;
  if(yaw > MAX_YAW) { //if more than max val
    yaw = MAX_YAW;
  } else if(yaw < MIN_YAW) { //if less than min
    yaw = MIN_YAW;
  }
  yawServo.write(servo270(yaw));
}
void setPitch(int deg){ //down
  pitch = deg;

  if(pitch > MAX_PITCH) { //if more than max val
    pitch = MAX_PITCH;
  } else if(pitch < MIN_PITCH) { //if less than min
    pitch = MIN_PITCH;
  }
  pitchServo.write(servo270(pitch));
}


int getPitch() { //display ptiche
  return pitch;
}
int getYaw() { //display yaw
  return yaw;
}

void startDriver() {
  driving = HIGH;
  digitalWrite(driver_motor_pin, driving);
}
void stopDriver() { //loading must stop if drivers are stopped
  driving = LOW;
  loading = LOW;
  digitalWrite(driver_motor_pin, driving);
  digitalWrite(loader_motor_pin, loading);
}
void startLoad() {
  if(driving == HIGH){//can only load bullets if driver motors are spinning
    loading = HIGH;
    digitalWrite(loader_motor_pin, loading);
  } else {
    loading = LOW;
    digitalWrite(loader_motor_pin, loading);
  }
  
}
void stopLoad() {
    loading = LOW;
    digitalWrite(loader_motor_pin, loading);
}
void updateLeds() {
      //LEDs to display state of system
    if(driving) {
      digitalWrite(warning_led_pin, HIGH);
      digitalWrite(safety_led_pin, LOW);
    } else {
      digitalWrite(warning_led_pin, LOW);
      digitalWrite(safety_led_pin, HIGH);
    }
    if(loading) {
      digitalWrite(danger_led_pin, HIGH);
    } else {
      digitalWrite(danger_led_pin, LOW);
    }
}

void checkLoader() {
   int val = analogRead(loader_sense_pin);
   Serial.println(val);
   if(val > 700 && loading == LOW) {
      digitalWrite(loader_motor_pin, HIGH); //independant of loading variable
      delay(15);
      digitalWrite(loader_motor_pin, LOW);
      delay(5);
   }
}

void setup() {
    Serial.begin(9600);         //Sets the data rate in bits per second (baud) for serial data transmission
  pitchServo.attach(pitch_pin);
  yawServo.attach(yaw_pin);

  pinMode(loader_motor_pin, OUTPUT);
  pinMode(driver_motor_pin, OUTPUT);
  pinMode(safety_led_pin, OUTPUT);
  pinMode(warning_led_pin, OUTPUT);
  pinMode(danger_led_pin, OUTPUT);

  digitalWrite(safety_led_pin, HIGH);
  digitalWrite(warning_led_pin, LOW);
  digitalWrite(danger_led_pin, LOW);
  
  setPitch(90);
  //resetPitch();
  delay(500);
  setYaw(135);
  //resetYaw();
  delay(500);

}


void loop() {

    //LEDs to display state of system
  updateLeds();
    
  checkLoader();
  if(Serial.available() > 0)  {
    
    Incoming_value = Serial.read();      //Read the incoming data and store it into variable Incoming_value
    //Serial.println(Incoming_value);        //Print Value of Incoming_value in Serial monitor

    
    int a = 10;
    int t = 15;
    switch (Incoming_value) {
      case 'l':
        addYaw(a);
        delay(t);
        break;
      case 'r':
        addYaw(-a);
        delay(t);
        break;
      case 'u':
        addPitch(-a);
        delay(t);
        break;
      case 'd':
        addPitch(a);
        delay(t);
        break;
      case 'A':
        if(driving == LOW) {
            startDriver();
        } else {
           stopDriver();
        }
        break;
      case 'X':
        if(loading == LOW) {
          startLoad();
        } else {
          stopLoad();
        }
        break;
      default:
        // statements
        break;
        
    }

  }                            
 
}
