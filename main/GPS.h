/*
  GPS.h - Library for getting GPS info including current ti,me and date and location (LAT + LONG).
  Created by Elad, Mar 31, 2019.
*/
#ifndef GPS_h
#define GPS_h
#include <TinyGPS++.h>

#include "Arduino.h"

struct GPS_DATA{
  //location
  double lat;
  double lan;
  //time
  uint16_t year;
  uint8_t month;
  uint8_t day;
  uint8_t minute;
  uint8_t hour;
  uint8_t second;

  double speed;
}typedef GPSData;


class GPS
{
  public:
    GPSData getGPSData();
    void printGPSData();
    void printFloat(float val, bool valid, int len, int prec);
    void printInt(unsigned long val, bool valid, int len);
    void printDateTime(TinyGPSDate &d, TinyGPSTime &t);
    void printStr(const char *str, int len);
    GPS();
  private:
    void smartDelay(unsigned long ms);
    int _rxPin;
    int _txPin;
};

#endif
