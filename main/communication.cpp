/*
  communication.cpp - Library for send and get messages by using RF.
  Created by Imri Yosef and Nir Ben kohal, march 31st, 2019.
  Released into the teve"l project.

  using arduino MEGA pin 18 & 19 for RX-TX
*/

#include "Arduino.h"
#include "communication.h"
#include <stdio.h>
#include "global.h"


//the constractor of the communication class
communication::communication()
{
    Serial1.begin(9600);               
}

/**
 * send message over RF
 */
void communication::sendMessage(char msg[]){
  if (DEBUG_ENABLED){
    Serial.print("Sending over RF:");
    Serial.println(msg);    
  }
  Serial1.write(msg);
  Serial1.flush();
}

/**
 * check if we got any message in RF, if yes fills buff with up to BUFFER_SIZE bytes from the message 
 * if printMessage==true prints the message to serial
 * 
 * returns the amount of bytes in the message recived, if therfe is no messaged than returns 0
 */
int communication::getMessage(char buff[], bool printMessage){
  int c=0;
  while (Serial1.available() && c < BUFFER_SIZE) {        // If HC-12 has data
    char character = Serial1.read();
    //Serial.print(character);
    buff[c]=character;
    c++;
    delay(1); // without this delay it look like we get split into a new meesage every few bytes
  }
  
  buff[c]= '\0'; // terminate the string

  // print what we got to the serial monitor...
  if (printMessage && c>0){
    Serial.print("Got: ");
    Serial.print(c);
    Serial.print(" chars. message=");
    Serial.println(buff);
  }
  
  return c;
}
