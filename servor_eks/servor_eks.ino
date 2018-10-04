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

void setup() {
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
}

void loop() {
  // for (pos = 0; pos <= 180; pos += 1) { // goes from 180 degrees to 0 degrees
    myservo.write(0);              // tell servo to go to position in variable 'pos'
    delay(500);
    myservo.write(180);              // tell servo to go to position in variable 'pos'
    delay(500);
    myservo.write(0);              // tell servo to go to position in variable 'pos'
   
   //  }
  exit(0);
}
