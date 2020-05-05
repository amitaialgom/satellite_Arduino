
#include "communication.h"
#include "SDcard.h"
#include "GPS.h"
#include "MemoryFree.h"
#include "global.h"

/**********************************************
 This is the main/starting point code for the YCubeSat 
 
**********************************************/



char rtxBuffer[BUFFER_SIZE]=""; // buffer for rx messages
char imALive[BUFFER_SIZE]="YcubeSat starting\n";
char beacon[BUFFER_SIZE]="YcubeSat BEACON\n";

// pointers to all objects used in the main code
communication *common;
SDcard *sd;
GPS *theGps;

unsigned long lastBeaconTransmit = -1000; // when was the last time we sent beacon - in miliseconds 
unsigned long lastTelematicsCollect = -1000; // when was the last time we collected telematics info from all sensors - in miliseconds



void setup() {

  intilize();

}

void loop() {
      
  sendBeacon();
  collectTelematics(0);
  ReceiveTasks();

  delay(MAIN_DELAY);
}


/**
 * transmits becaon meesage
 */
void sendBeacon(){
  if (millis()-lastBeaconTransmit > BEACON_INTEVAL){
    if (DEBUG_ENABLED)
      Serial.println("Sending beacon...");

    common->sendMessage(beacon);  
    lastBeaconTransmit= millis();
  }
}

/**
 * initilize the cube sat components. 
 * sneds i'm a live meesage to ground station
 */
void intilize(){
  // put your setup code here, to run only once on boot:
  if (DEBUG_ENABLED)
    Serial.begin(9600);             // Serial port to computer
  
  // create new instance for all objects we need
  common = new communication();
  sd = new SDcard(53);
  theGps = new GPS();

  // send initial i'm a live message
  common->sendMessage(imALive);
  
}

/**
 * collect telmatics data from all sensors and store in SD as CSV format
 * sendOverRF - it 0than just store on SD if 1 than also send over RF to gounf station
 * each line in the file includes: 
 * millis from reboot, current date (YYYYMMDD), current time (hh:mm:ss), lat, long, speed, free RAM, free SD space in MB
 */

void collectTelematics(int sendOverRF){

  // check if we need to collect telematics, if not return, if we need to sendOverRF than alway process the request
  if (millis()-lastTelematicsCollect<TELEMATICS_INTEVAL && sendOverRF==0) return;

  lastTelematicsCollect = millis();
  /**
   * collect data from GPS
   */
  GPSData data = theGps->getGPSData();

  // generate telematics file name based on current day
  String fileName = "";
  String dateTime ="";

  if (data.day>10)
    dateTime.concat(data.day);
  else{
    dateTime.concat("0");
    dateTime.concat(data.day);    
  }

  if (data.month>10)
    dateTime.concat(data.month);
  else{
    dateTime.concat("0");
    dateTime.concat(data.month);    
  }
    dateTime.concat(data.year);

        
  fileName.concat(dateTime);
  fileName.concat(".csv");

        

  String msg = "";
  msg.concat(millis());
  msg=msg+",";
  msg.concat(dateTime);
  char time[32];
  sprintf(time, "%02d:%02d:%02d ", data.hour,data.minute,data.second);
  msg = msg +"," + time;

      Serial.println("6");

  // add the location (convert double to string)
  char str[32]={0};
  dtostrf(data.lat, 2, 6, &str[strlen(str)]);
  strcat(str, ",");
  dtostrf(data.lan, 2, 6, &str[strlen(str)]);
  
  msg = msg +"," + str;
  msg = msg + "," + data.speed;
  
  /**
   * collect data from arduino
   */
   
  msg = msg + "," + freeMemory();


  
  /**
   * collect data from sun sensors... TBD
   */


  /**
   * collect free sapce on SD card...note that this opeation can take a few seconds !
   */
  //msg = msg + "," + sd->getFreeSpace();
  msg = msg + "," + "1500"; //  sd->getFreeSpace() takes too much time, for now just return something...



  /**
   * write all telematics data to SD...
   */
  if (DEBUG_ENABLED){
    Serial.print("file name:");
    Serial.println(fileName);
    Serial.print("message:");
    Serial.println(msg);  
  }
  sd->writeMsg(fileName,msg);

  // send msg over RF - if requested
  if (sendOverRF ==1){
    while (msg.length()>0){
      msg.toCharArray(rtxBuffer, BUFFER_SIZE-1);
      rtxBuffer[BUFFER_SIZE]="\0";
      common->sendMessage(rtxBuffer);        
      Serial.println(rtxBuffer);
      msg.remove(0,BUFFER_SIZE-2);
    }
  }
}

void(* resetFunc) (void) = 0;//declare reset function at address 0

/**
 * recevie task messages from land station
 * protocol: 
 * 0-2: command id (001=send current telematics, 002=send telematics for date, 003=reboot OBC)
 * 3-30: command parametres()
 * 
 * example for getting telematics for date: 00215012019
 */
void ReceiveTasks(){

    int i = common->getMessage(rtxBuffer,true);
    if (i<=0) return;
      
    char cmd[3];
    memcpy( cmd, &rtxBuffer[0], 3 );
    int cmdID = atoi(cmd);

    char ddate[12];
    memcpy( ddate, &rtxBuffer[3], 8 );
    ddate[8] = '\0';
    String theDate = ddate;
    theDate.concat(".csv");



 switch(cmdID)
 
 {
  // send current telematics over RF
  case 001:
      collectTelematics(1);  
      break;
  // send telematics for date
  case 002:
      Serial.print("date:");  
      Serial.println(theDate);
        sd->sendFile(theDate);
      break;
  // restart OBC
  case 003:
      Serial.println("3");
      resetFunc();
      //asm volatile ("  jmp 0");  
      break;
  default: 
      Serial.print("invalid coomand");  
 }
   
  
}
