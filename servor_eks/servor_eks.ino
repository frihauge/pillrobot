/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/

#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position
int nul = 13;
void setup() {
  myservo.attach(8);  // attaches the servo on pin 9 to the servo object
  //myservo.writeMicroseconds(1500);  // set servo to mid-point
}
 
void loop() {

 myservo.write(-10);
 delay(2000); 
 myservo.write(75);
 delay(2000);
 myservo.detach(); 

//
//exit(0);
 
}
