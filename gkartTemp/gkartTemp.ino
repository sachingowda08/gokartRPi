#include <SoftwareSerial.h>
#include <OneWire.h>
#include <TinyGPS.h>
#define ldrPin A0
#define ldrThresh 500
#define headLightPin 7
#define tempSensorPin  10
OneWire  ds(tempSensorPin);
TinyGPS gps;
SoftwareSerial ss(4, 3);
float celsius, fahrenheit;
String stringResult; 
float timeElapsed=0;
float timeForTempElapsed=millis();
int timeForTemp=5000;
volatile float count=0;
float rpm;
void readTemp(){
  byte i;
  byte present = 0;
  byte type_s;
  byte data[12];
  byte addr[8];
  if ( !ds.search(addr)) {
    ds.reset_search();
    long int prev=millis();
    while(millis()-prev<=250){
      }
      return;
      }
      if (OneWire::crc8(addr, 7) != addr[7]) {
        //Serial.println("CRC is not valid!");
      return;
      } 
  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);       // start conversion, with parasite power on at the end
      // maybe 750ms is enough, maybe not

  long int prev=millis();
  while(millis()-prev<=1000){
    }
  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // Read Scratchpad
  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
  }
  int16_t raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3; // 9 bit resolution default
    if (data[7] == 0x10) {
      // "count remain" gives full 12 bit resolution
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } else {
    byte cfg = (data[4] & 0x60);
    // at lower res, the low bits are undefined, so let's zero them
    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
    //// default is 12 bit resolution, 750 ms conversion time
  }
  celsius = (float)raw / 16.0;
  //fahrenheit = celsius * 1.8 + 32.0;
  //Serial.print("  Temperature = ");
  //Serial.print(celsius);
  //Serial.print(" Celsius, ");
  }
void startPos() {
  count++;
}

void setup()
{
  Serial.begin(115200);
  ss.begin(9600);
  attachInterrupt(0, startPos, FALLING);
  timeElapsed = 0;
  timeForTempElapsed=millis();
}


void loop()
{
  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;
  if(analogRead(ldrPin)>ldrThresh){
    digitalWrite(headLightPin,HIGH);
    }
    else{
      digitalWrite(headLightPin,LOW);
      }
  detachInterrupt(0);
  timeElapsed=millis()-timeElapsed;
  rpm=count*60000/timeElapsed;
  timeElapsed=millis();
  attachInterrupt(0, startPos, FALLING);
  count=0;
  if( (millis()-timeForTempElapsed) > timeForTemp  ){
  readTemp();
  timeForTempElapsed=millis();
  }
    // For one second we parse GPS data and report some key values
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (ss.available())
    {
      char c = ss.read();
     //  Serial.write(c); // uncomment this line if you want to see the GPS data flowing
      if (gps.encode(c)) // Did a new valid sentence come in?
        newData = true;
    }
  }
float flat=0, flon=0 ,fspeed=0;
  if (newData)
  {
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);

   flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6;
//print_float(gps.f_speed_kmph(), TinyGPS::GPS_INVALID_F_SPEED, 6, 2);
fspeed=gps.f_speed_kmph();
   flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6;
  }
  
  //gps.stats(&chars, &sentences, &failed);
  String stringCelsius=String(celsius);
  String stringLAT=String(flat,6);
  String stringLONG=String(flon,6);
  String stringSPEED=String(fspeed,6);
  String stringRPM=String(rpm,2);
  stringResult="TEMP="+stringCelsius+",LAT="+stringLAT+",LONG="+stringLONG+",SPEED="+stringSPEED+",RPM="+stringRPM ;
  Serial.println(stringResult);
 // if (chars == 0)
  //  Serial.println("** No characters received from GPS: check wiring **");
}
