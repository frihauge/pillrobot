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

const byte ledPin = 13;
const byte interruptPin = 2;
volatile byte PillFeed = LOW;

enum State_enum {STOP, FAIL, PILLFEED, PILLREALSE, VIBRATE};
enum Sensors_enum {NONE, PILLFEED_DET, SENSOR_LEFT, BOTH};

 
void state_machine_run(uint8_t sensors);
bool onepillfeed();
//void vibrate();


uint8_t state = STOP;
 
uint8_t VibrateCount = 0;
uint16_t PillOK_Count[9] = {0}; //Pill release after 0 - 8 vibration 

 

void setup() {
    // put your setup code here, to run once:
  Serial.begin(9600);
  Serv_PillRelase.attach(9);  // attaches the servo on pin 9 to the servo object
  Serv_Pillfeed.attach(8);  // attaches the servo on pin 9 to the servo object
  pinMode(ledPin, OUTPUT);
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), PillFeedDet, CHANGE); 
  
  //pinMode(ADC_CHANNEL0_PIN, INPUT);
  //pinMode(GPIO_PORT0_PIN, INPUT);
  //pinMode(USB_PORT0_PIN, OUTPUT);

 
}

void loop(){

  if(Serial.available()> 0)
  {
  }
 
  state_machine_run(PILLFEED);
 
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
       
        state = PILLFEED;
      }
      break;
    case FAIL:
      break;   
    case PILLFEED:
      bool stat = onepillfeed();
      
      if(!stat){
        state = VIBRATE;
      }
      break;
    case PILLREALSE:
      if(sensors != PILLFEED_DET){
        state = VIBRATE;
      }
      break;
    case VIBRATE:
      vibrate();
      VibrateCount++;
      if (VibrateCount <= 8)
        state = PILLFEED;
      else
        state = FAIL;
      break;
  }
}
 

bool onepillfeed()
{
 uint8_t pos = 0;
  bool stat = false;
  //code for feedig one pill
    for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    Serv_Pillfeed.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }  

  for (pos = 180; pos >= 0; pos -= 1) { 
    Serv_Pillfeed.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  delay(100);
  stat = PillFeed; 
  if(PillFeed == true)
  { 
    PillFeed = false;
  }
    
  return  stat; 
}

void PillFeedDet() {
  PillFeed = true;
}
uint8_t vibrate()
{
 
}
