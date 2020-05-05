/*
  communication.h - Library for send and get messages by using RF.
  Created by Imri Yosef and Nir Ben kohal, march 31st, 2019.
  Released into the teve"l project.



  TODO:

  1. get messasge is limted by 32 chars - need to think how to process longer messages!
  
*/
#ifndef communication_h
#define communication_h

#include "Arduino.h"

class communication
{
  public:
    void sendMessage(char msg[]);
    int getMessage(char buff[], bool printMessage);
    communication();
};
#endif
