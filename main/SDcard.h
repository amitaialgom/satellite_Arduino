/*
 SD card.h - Library for read and write filse from SD card.
  Created by Eitan Palmon, 31 Merch 2019.
*/
#ifndef SDcard_h
#define SDcard_h


#include "Arduino.h"
#include "SdFat.h"

class SDcard
{
  public:
   SDcard(int CSin);
   void writeMsg(String fileName, String message);
   void sendFile(String fileName);
   void printFile(String fileName);
   float getFreeSpace();
   
  private:
    int _CSin;
};

#endif
