
#define MY_DEBUG
#define MY_RADIO_NRF24
#define GPS_SEND_INTERVAL 10000

#define SKETCH_VERSION "v1.5"
#define ID_S_DISTANCE 1
#define ID_S_WEIGHT 2
#define CHILD_ID_GPS 3
#define ID_S_INFO 4
static const int GPS_PIN = A0;
static const uint32_t GPSBaud = 9600;
// Offset hours adjustment from gps time (UTC)
const int offset = 1;

#include <SPI.h>
#include <TimeLib.h>
#include <MySensors.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
//distance
#include <NewPing.h>
#define TRIGGER_PIN  4  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     5  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 400 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
//distance

//Load Cell
/*
  #include "HX711.h"
  #define DOUT  6
  #define CLK  7
  HX711 scale(DOUT, CLK);
  float calibration_factor = 260.40;//obtain it from caliberation program
  //Load Cell*/
  #define COIN_DISP_WEIGHT 20
  long lastWT = 0;

// TinyGPS++ is used for parsing serial gps data
TinyGPSPlus gps;
SoftwareSerial ss(GPS_PIN, A1);
unsigned long lastGPSSent = 0;

// Some buffers
char latBuf[11];
char lngBuf[11];
char altBuf[6];
char payload[30];
char sz[64];
String info;
MyMessage msg_GPS(CHILD_ID_GPS, V_POSITION);
MyMessage msg_S_DISTANCE(ID_S_DISTANCE, V_DISTANCE);
MyMessage msg_S_WEIGHT(ID_S_WEIGHT, V_WEIGHT);
MyMessage msg_S_INFO(ID_S_INFO, V_TEXT);
void setup() {
  ss.begin(GPSBaud);
  //scale.tare();//resest to zero
  Serial.println("Enabling GPS..");
  ss.begin(115200);
  ss.println("AT+GPS=1");
  wait(100);
  ss.begin(GPSBaud);
}


void presentation() {
  //Send the sensor node sketch version information to the gateway
  Serial.println("Presenting Sensors");
  present(ID_S_DISTANCE, S_DISTANCE, "Distance Sensor");
  present(CHILD_ID_GPS, S_GPS, "GPS");
  present(ID_S_WEIGHT, S_WEIGHT, "WEIGHT");
  present(ID_S_INFO, S_INFO, "Bin Info");
}

void loop()
{

  unsigned long currentTime = millis();

  // Evaluate if it is time to send a new position
  bool timeToSend = currentTime - lastGPSSent > GPS_SEND_INTERVAL;

  // Read gps data
  while (ss.available())
    gps.encode(ss.read());

  if (timeToSend) {
    // Sync gps time with Arduino
    updateTime();

    // Send current gps location
    if (gps.location.isValid() && gps.altitude.isValid()) {
      // Build position and altitude string to send
      dtostrf(gps.location.lat(), 1, 6, latBuf);
      dtostrf(gps.location.lng(), 1, 6, lngBuf);
      dtostrf(gps.altitude.meters(), 1, 0, altBuf);
      sprintf(payload, "%s;%s;%s", latBuf, lngBuf, altBuf);

      Serial.print(F("Position: "));
      Serial.println(payload);

      send(msg_GPS.set(payload));

      Serial.print(F("GPS Time: "));
      sprintf(sz, "%02d/%02d/%02d %02d:%02d:%02d", month(), day(), year(), hour(), minute(), second());
      Serial.println(sz);
      info = "";
      getWeight();
      getLevel();

    } else {
      if (millis() > 5000 && gps.charsProcessed() < 10)
        Serial.println(F("No GPS data received: check wiring"));
      else
        Serial.println(F("No GPS data received yet..."));
    }
    lastGPSSent = currentTime;
  }
}

void updateTime()
{
  TinyGPSDate d = gps.date;
  TinyGPSTime t = gps.time;
  if (d.isValid() && t.isValid()) {
    // set the Time to the latest GPS reading if less then 0.2 seconds old
    setTime(t.hour(), t.minute(), t.second(), d.day(), d.month(), d.year());
    adjustTime(offset * SECS_PER_HOUR);
    return;
  }
  Serial.println(F("Unable to adjust time from GPS"));
}
void getWeight()
{

  Serial.print("\tWeight:\t");
  long w =  12;//scale.get_units(10);

  long currentWT;
  currentWT = w;
  bool do_dispense = currentWT - lastWT > COIN_DISP_WEIGHT;

  lastWT = currentWT;
  Serial.println(w, 1);//get average of 10 readings

  if (w > 82)
  {
    Serial.print("Garbage Added ");
    info += "Garbage Added ";
  } else
  {
    //do nothing
  }

  if (do_dispense)
  {
    Serial.print("Coin Dispensed ");
    info += "Coin Dispensed ";
    ///MoveForward();
    //wait(600);
    //StopMotor();
  } else
  {
    //do nothing
  }
  send(msg_S_WEIGHT.set(w));//weight

}

void getLevel()
{
  Serial.println("Level is: " );
  long d = sonar.ping_cm();
  Serial.println(d);
  if ((d < 21) && (d > 0))
  {
    Serial.print("Bin full ");
    info += "Bin Full ";
  } else
  {
    //do nothing
  }
  //Send All Info to GateWay
  send(msg_S_DISTANCE.set(d));//distance
}
