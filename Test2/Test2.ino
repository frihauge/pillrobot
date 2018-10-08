#include "Arduino.h"
#include <Servo.h>
#define BUTTON_PIN 3
#define RELDETPIN 3
#define RELSERVOPIN 8
#define DELAY 30000 //milliseconds
#define VIBRATE_PIN 1

static unsigned long time;
static unsigned int state;
static unsigned int button;
static unsigned int delay_1;
int incomingByte = 0;
const byte releaseinterrupt = 2;
volatile byte interrupt_state = LOW;
static unsigned int RetryCount = 0;
uint16_t Passcount[9] = {0};



enum State_enum {STOP, START, PILLRELASE_END, PILLRELASE, DETEKT_PILLRELEASE, VIBRATE, ALERT};
Servo Serv_PillRelase;  // create servo object to control a servo


void onepillrelease();
void vibrate();
bool pillreleasedet();


void setup()
{
    time = 0;
    state = STOP;

    pinMode(VIBRATE_PIN, OUTPUT);          // sets the digital pin 1 as output
    pinMode(RELDETPIN, INPUT_PULLUP);
    // put your setup code here, to run once:
    Serial.begin(9600);
    while (!Serial);// wait for serial port to connect. Needed for native USB port only
    Serv_PillRelase.attach(RELSERVOPIN);  // attaches the servo on pin 9 to the servo object
    Serial.print("PillRobot Test start up complete");
    attachInterrupt(digitalPinToInterrupt(RELDETPIN), fotoReleaseDet, CHANGE);
}

void loop()
{
bool s = false;
  if (Serial.available() > 0) 
  {
    incomingByte = Serial.read(); // read the incoming byte:
    Serial.println(incomingByte);
    if (incomingByte == 's')
      state = START;
    if (incomingByte == 'e')
      state = STOP;
  }

    switch (state)
    {
    case START:
      state = PILLRELASE;
      break;
    
    case STOP:
      delay(1000);
      state = STOP;
      break;
            
    case PILLRELASE:
      onepillrelease();
      state = DETEKT_PILLRELEASE;
      break;
      
    case PILLRELASE_END:
      delay(3000);
      state = PILLRELASE;
      break;  
      
    case DETEKT_PILLRELEASE:
        s = pillreleasedet();
        if (s){
            state = PILLRELASE_END;
            Passcount[RetryCount]++;
            RetryCount = 0;
          }
          else
          {
            RetryCount ++;
            delay(1000);
            if (RetryCount <= 8) 
              {state = VIBRATE;}
            else
              {state = ALERT;}  
          }  
         
        break;      
         
    case VIBRATE:
        RetryCount++;
        vibrate();
        state=PILLRELASE;
        break;
        
    case ALERT:
        Serial.print("PillRobot In alert mode"); 
        delay(100);
        state = STOP;
        break;
        
    }
}

void onepillrelease()
{
  int pos = 0;
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
   // in steps of 1 degree
    Serv_PillRelase.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    Serv_PillRelase.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }

}

void vibrate()
{
  digitalWrite(VIBRATE_PIN, HIGH);       // sets the digital pin 13 on
  delay(2000);                  // waits for a second
  digitalWrite(VIBRATE_PIN, LOW);        // sets the digital pin 13 off
  delay(1000); 
}

void fotoReleaseDet()
{
  interrupt_state = true;
}

bool pillreleasedet()
{
 for (int i = 0; i < 100; i++)
 {
  if (interrupt_state)
  {
    interrupt_state = false;
    return true;
  }

  else
    {
     delay (5);  
    } 
 }
 return false;
}
