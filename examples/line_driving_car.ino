#include "MyToolsV5.h"
//Include the Tool first

BUTTON red(3);  //reg a button
BUTTON green(4);  //reg a button
BUTTON blue(5);  //reg a button
BUTTON yellow(7);  //reg a button
BUTTON back_Touch(13);  //reg a button
Motors motor;  //reg motors


void setup() {
  Serial.begin(9600);
}

void loop() {
  if(back_Touch){  //button pressed
    motor.brake();  //brake(not coast)
    while(1){
      if(green&&blue){
        break;
      }
    }
    green.waitUntilRelease();
    blue.waitUntilRelease();
  }else if(red){  //button pressed
    motor(125,125);  //motor go
  }else if(green){  //button pressed
    motor(-125,-125);  //motor go
  }else if(blue){  //button pressed
    motor(-125,125);  //motor go
  }else if(yellow){  //button pressed
    motor(125,-125);  //motor go
  }else{
    motor(0,0);  //motor coast
  }
}
