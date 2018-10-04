#include <Servo.h>

//*** DEFINES *****************************************************************
#define VERSION "PR_Test_V1, Revision: 1.0, Compile time: " __DATE__ " " __TIME__


#define ADC_CHANNEL0_PIN A0
#define GPIO_PORT0_PIN PB0
#define USB_PORT0_PIN PC1
#define TPIC6C595_LATCH_0_PIN 2

//#Servo 1 180 = Lukket, 0 = åben-
//#Servo 2 180 = Lukket, 90 = åben

Servo myservo;  // create servo object to control a servo
enum State_enum {STOP, FORWARD, ROTATE_RIGHT, ROTATE_LEFT};
enum Sensors_enum {NONE, SENSOR_RIGHT, SENSOR_LEFT, BOTH};
 
void state_machine_run(uint8_t sensors);
void motors_stop();
void motors_forward();
void motors_right();
void motors_left();
uint8_t read_IR();
 
uint8_t state = STOP;
 

void setup() {
    // put your setup code here, to run once:
  Serial.begin(9600);
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object

  pinMode(ADC_CHANNEL0_PIN, INPUT);
  
  pinMode(GPIO_PORT0_PIN, INPUT);
  
  pinMode(USB_PORT0_PIN, OUTPUT);

 
}

void loop(){
  state_machine_run(read_IR());
 
  delay(10);
}
 
void state_machine_run(uint8_t sensors) 
{
  switch(state)
  {
    case STOP:
      if(sensors == NONE){
        motors_forward();
        state = FORWARD;
      }
      else if(sensors == SENSOR_RIGHT){
        motors_left();
        state = ROTATE_LEFT;
      }
      else{
        motors_right();
        state = ROTATE_RIGHT;
      }
      break;
       
    case FORWARD:
      if(sensors != NONE){
        motors_stop();
        state = STOP;
      }
      break;
 
    case ROTATE_RIGHT:
      if(sensors == NONE || sensors == SENSOR_RIGHT){
        motors_stop();
        state = STOP;
      }
      break;
 
    case ROTATE_LEFT:
      if(sensors != SENSOR_RIGHT)
      {
        motors_stop();
        state = STOP; 
      }
      break;
  }
}
 
void motors_stop()
{
  //code for stopping motors
}
 
void motors_forward()
{
  //code for driving forward  
}
 
void motors_right()
{
  //code for turning right
}
 
void motors_left()
{
  //code for turning left
}
 
uint8_t read_IR()
{
  //code for reading both sensors
}
