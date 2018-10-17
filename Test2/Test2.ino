#include "Arduino.h"
#include <Servo.h>
#include <EEPROM.h>

#define RELDETPIN 2
#define BUTTON_PIN 3
#define VIBRATE_PIN 4
#define RELSERVOPIN 9
#define CUPSERVOPIN 8
#define CUPDETTPIN 3

#define DELAY 30000 //milliseconds

#define PASSCNTSIZ 9

static unsigned long time;
static unsigned int state;
static unsigned int button;
static unsigned int delay_1;
int incomingByte = 0;
int addr = 0;
bool Automode = false;
const byte releaseinterrupt = 2;
volatile byte interrupt_state = LOW;
unsigned int RetryCount = 0;

int  Passcount[PASSCNTSIZ] = {0,0,0,0,0,0,0,0,0};



enum State_enum {STOP, START, PILLRELASE_END, PILLRELASE, DETEKT_PILLRELEASE, VIBRATE, ALERT};
Servo Serv_PillRelase;  // create servo object to control a servo
Servo Serv_PillCupRelase;

//Prototypes
void onepillrelease();
void vibrate();
bool pillreleasedet();
void SerialInHandler(char);
void fotoReleaseDet();
void PrintResult();
void cuprelease(bool state);


void setup()
{
    time = 0;
    state = STOP;

    pinMode(VIBRATE_PIN, OUTPUT);           
    digitalWrite(VIBRATE_PIN, LOW);        
    pinMode(RELDETPIN, INPUT_PULLUP);
    // put your setup code here, to run once:
    Serial.begin(9600);
    while (!Serial);// wait for serial port to connect. Needed for native USB port only
   /* for (int i = 0; i < PASSCNTSIZ; i++)
    {
      EEPROM.get( i, Passcount[i] );
    }
*/
    
    

    Serial.println("PillRobot Test start up complete");
    attachInterrupt(digitalPinToInterrupt(RELDETPIN), fotoReleaseDet, CHANGE);
    
}

void loop()
{
bool s = false;
  if (Serial.available() > 0) 
  {
    incomingByte = Serial.read(); // read the incoming byte:
    SerialInHandler(incomingByte);
    Serial.println(state);
  }  
    delay(1000);
    switch (state)
    {
    case START:
      RetryCount = 0;
      state = PILLRELASE;
      break;
    
    case STOP:
      delay(1000);
      state = STOP;
      break;
            
    case PILLRELASE:
      Serial.println("PILL RELEASE");
      onepillrelease();
      state = DETEKT_PILLRELEASE;
      break;
      
    case PILLRELASE_END:
      PrintResult();
      delay(3000);
      if (Automode)
      {
        state = PILLRELASE;
      }
      else
      {
        state = STOP;
      }
      break;  
      
    case DETEKT_PILLRELEASE:
        s = pillreleasedet();
        Serial.print("PILL DET ");
        if (s){
            Serial.println("True");
            state = PILLRELASE_END;
            Passcount[RetryCount]++;
            RetryCount = 0;
          }
          else
          {
            Serial.println("False");
            if (RetryCount <= 8) 
              {state = VIBRATE;}
            else
              {state = ALERT;}  
          }  
         
        break;      
         
    case VIBRATE:
        RetryCount++;
        vibrate();
        Serial.print("Vibrate count ");
        Serial.println(RetryCount);
         
        state=PILLRELASE;
        break;
        
    case ALERT:
        Serial.println("PillRobot In alert mode"); 
        delay(100);
        state = STOP;
        break;
        
    }
}

void PrintResult()
{
  Serial.println("STATE PILLRELEASE END ");
  for (uint8_t i = 0; i < PASSCNTSIZ; i++) {
  Serial.print("Vibrate cnt: ");
  Serial.print(i);
  Serial.print(" ");
  Serial.println(Passcount[i]);
  }
}

void SerialInHandler(char incomingByte)
{
  Serial.println(char(incomingByte));
  switch (incomingByte)
   {
    case 'H':
      Serial.println("Hello World");
      break;
    
    case 'r':
      state = START;
      break;
      
    case 'e':
      Automode = false;
      state = STOP;
      break;
      
    case 'a':
      Automode = true;
      state = START;
      break;
      
    case 'p':
      PrintResult ();
      break;
    
    case 'v':
      vibrate ();
      break;

    case '1':
      onepillrelease();
      break;
    default:
    break;  
    
    }
  }


void cuprelease(bool state)
{
  Serv_PillCupRelase.attach(CUPSERVOPIN);  // attaches the servo on pin 9 to the servo object
  if (state)
  {
    Serv_PillCupRelase.write(0);
  }
  else
  {
    Serv_PillCupRelase.write(100);
  }
  delay(400);

  Serv_PillCupRelase.detach();
}

void onepillrelease()
{
  int pos = 0;
  cuprelease(false);
  Serv_PillRelase.attach(RELSERVOPIN);  // attaches the servo on pin 9 to the servo object
 
    for (pos = 100; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    Serv_PillRelase.write(pos);              // tell servo to go to position in variable 'pos'
    delay(1);                       // waits 15ms for the servo to reach the position
  }  
  delay(500);
  
   for (pos = 0; pos <= 100; pos += 1) { // goes from 0 degrees to 180 degrees
   // in steps of 1 degree
    Serv_PillRelase.write(pos);              // tell servo to go to position in variable 'pos'
    delay(1);                       // waits 15ms for the servo to reach the position
  }
  delay(500);
  Serv_PillRelase.detach();  // deattaches the servo on pin 9 to the servo object
  cuprelease(true);
 
}

void vibrate()
{
  digitalWrite(VIBRATE_PIN, HIGH);       // sets the digital pin 13 on
  delay(5000);                  // waits for a second
  digitalWrite(VIBRATE_PIN, LOW);        // sets the digital pin 13 off
  delay(1000); 
}

void UpdateEEprom()
{
  for (int i = 0; i < PASSCNTSIZ; i++) {
    EEPROM.update(i, Passcount[i]);
  }
}

void fotoReleaseDet()
{
  Serial.println("Pill Det Intterupt");
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
