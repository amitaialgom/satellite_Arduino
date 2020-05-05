/*
  SDcard.cpp - Library for read and write filse from SD card.
   Created by Eitan Palmon, 31 Merch 2019.
*/

#include "Arduino.h"
#include "SDcard.h"
#include "global.h"
#include "communication.h"

const uint8_t SD_CHIP_SELECT = SS;
const int8_t DISABLE_CHIP_SELECT = -1;
SdFat SD;
communication *rfComm;



//the constrractour of the SDcard cllas
SDcard::SDcard(int CSin)
{


  Serial.print("Initializing SD card...");

  if (!SD.begin(CSin)) {
    Serial.println("initialization failed!");
    //while (1);
  }
  rfComm = new communication();
  Serial.println("initialization done.");
    
  _CSin = CSin;
  
}


/**
 * writes meesage into SD card
 */
void SDcard::writeMsg(String fileName, String message){
  File myFile = SD.open(fileName, FILE_WRITE);

 
 if (myFile) {
    if (DEBUG_ENABLED){
      Serial.print("Writing to file...");      
    }
    myFile.println (message);
    // close the file:
    myFile.close();
    if (DEBUG_ENABLED){
      Serial.println (message);
      Serial.println("done.");
    }
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening file");
  }
}

/**
 * reads all messages from SD file and prints to serial
 */
 void  SDcard::printFile(String fileName){
  // re-open the file for reading:
  File myFile = SD.open(fileName);
  if (myFile) {
    Serial.println("file:");

    // read from the file until there's nothing else in it:
    while (myFile.available() ) {
        char c = myFile.read();
        Serial.write(c);
      }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
}



/**
 * reads file data and send over RF communication
 */
 void  SDcard::sendFile(String fileName){
    
    char buff[BUFFER_SIZE];
  // re-open the file for reading:
  File myFile = SD.open(fileName);
  if (myFile) {
    Serial.println("file:");

    // read from the file until there's nothing else in it:
    while (myFile.available() ) {
      int i=0;
      while (myFile.available() && i < BUFFER_SIZE) {
        char c = myFile.read();
        Serial.write(c);
        buff[i]=c;
        i++;
        delay(1);
      }
      buff[i] = '\0';
      rfComm->sendMessage(buff);
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening file");
  }  
}








// get the amount of free sapce on the SD card in MB. returns -1 if can't open SD card device
float SDcard::getFreeSpace(){
    float fs = -1;
    if (SD.begin(SD_CHIP_SELECT, SPI_FULL_SPEED)) {
      uint32_t volFree = SD.vol()->freeClusterCount();        
      fs = 0.000512*volFree*SD.vol()->blocksPerCluster();
      if (DEBUG_ENABLED)
        Serial.println(fs);
    }

  return fs;
}
