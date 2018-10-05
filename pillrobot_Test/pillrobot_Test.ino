#include <Servo.h>

//*** DEFINES *****************************************************************
#define VERSION "PR_Test_V1, Revision: 1.0, Compile time: " __DATE__ " " __TIME__


#define ADC_CHANNEL0_PIN A0
#define GPIO_PORT0_PIN PB0
#define USB_PORT0_PIN PC1
#define TPIC6C595_LATCH_0_PIN 2

//#Servo 1 180 = Lukket, 0 = åben-
//#Servo 2 180 = Lukket, 90 = åben

Servo Serv_PillRelase;  // create servo object to control a servo
Servo Serv_Pillfeed;  // create servo object to control a servo

enum State_enum {STOP, PILLFEED, PILLRELASE, VIBRATE};
enum Sensors_enum {NONE, PILLFEED, SENSOR_LEFT, BOTH};

 
void state_machine_run(uint8_t sensors);
void motors_stop();
void onepillfeed();
void motors_right();
void motors_left();
uint8_t read_IR();
 
uint8_t state = STOP;

uint16_t PillOK_Count[9] = 0; //Pill release after 0 - 8 vibration 

 

void setup() {
    // put your setup code here, to run once:
  Serial.begin(9600);
  Serv_PillRelase.attach(9);  // attaches the servo on pin 9 to the servo object
  Serv_Pillfeed.attach(8);  // attaches the servo on pin 9 to the servo object
  
  
  //pinMode(ADC_CHANNEL0_PIN, INPUT);
  //pinMode(GPIO_PORT0_PIN, INPUT);
  //pinMode(USB_PORT0_PIN, OUTPUT);

 
}

void loop(){

  if(Serial.available()> 0)
  {
    process_serial(Serial.read(),&STATE,TEXT_BUEFFER,&TEXT_BUEFFER_CNT);
  }
 
  state_machine_run(read_sensor());
 
  delay(10);
}

 
void state_machine_run(uint8_t sensors) 
{
  switch(state)
  {
    case STOP:
      if(sensors == NONE){
       state = STOP;
      }
      else if(sensors == PILLFEED){
        onepillfeed();
        state = PILLFEED;
      }
      else{
        motors_right();
        state = ROTATE_RIGHT;
      }
      break;
       
    case PILLFEED:
      if(sensors != PILLDEEDED){
        vibrate();
        state = VIBRATE;
      }
      break;
    case PILLREALSE:
      if(sensors != PILLDEEDED){
        vibrate();
        state = VIBRATE;
      }
      break;
    case VIBRATE:
      if(sensors != SENSOR_RIGHT)
      {
        motors_stop();
        state = STOP; 
      }
      break;
  }
}
 

void onepillfeed()
{
  //code for driving forward  
}

 
uint8_t read_IR()
{
  //code for reading both sensors
}
