/**
   SmartBin Gateway

    Created on: 20-03-2018
    By: ioCare, Pune
*/

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#define USE_SERIAL Serial
const char* ssid = "iocareap";
const char* password = "iocare2018";

const byte numChars = 100;
char receivedChars[numChars]; // an array to store the received data

char url[100];
char datas[100];
char allVals[7];
boolean newData = false;
void setup() {

  USE_SERIAL.begin(115200);
  // USE_SERIAL.setDebugOutput(true);

  USE_SERIAL.println();
  USE_SERIAL.println();
  USE_SERIAL.println();

  USE_SERIAL.print("connecting to ");
  USE_SERIAL.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    USE_SERIAL.print(".");
  }
  USE_SERIAL.println("");
  USE_SERIAL.println("WiFi connected");
  USE_SERIAL.println("IP address: ");
  USE_SERIAL.println(WiFi.localIP());
  dataPoster("started");
}

void loop() {

  recvWithEndMarker();
  showNewData();
  ///delay(10000);
  delay(10);
}

void recvWithEndMarker() {
  static byte ndx = 0;
  char endMarker = '\n';
  char rc;

  while (USE_SERIAL.available() > 0 && newData == false) {
    rc = USE_SERIAL.read();

    if (rc != endMarker) {
      receivedChars[ndx] = rc;
      ndx++;
      if (ndx >= numChars) {
        ndx = numChars - 1;
      }
    }
    else {
      receivedChars[ndx] = '\0'; // terminate the string
      ndx = 0;
      newData = true;
    }
  }
}

void showNewData() {
  if (newData == true) {
    dataPoster(receivedChars);
    newData = false;
  }
}
float stringToDouble(String & str)  {
  return atof( str.c_str() );
}
void dataPoster(char rc[])
{
  int lvl;
  double wgt;
  String lats;
  String lngs;
  String s = String(rc);
  //11;3;1;0;49;26.881996;75.813400;400
  //1-level
  //2-wt
  int commaIndex = s.indexOf(';');
  //  Search for the next comma just after the first
  int secondCommaIndex = s.indexOf(';', commaIndex + 1);
  String firstValue = s.substring(0, commaIndex);//11

  int thirdCommaIndex = s.indexOf(';', secondCommaIndex + 1);
  int secondValue = s.substring(commaIndex + 1, secondCommaIndex).toInt();//3
  String thirdValue = s.substring(secondCommaIndex + 1, thirdCommaIndex); ///1

  int fourthCommaIndex = s.indexOf(';', thirdCommaIndex + 1);
  String fourtValue = s.substring(thirdCommaIndex + 1, fourthCommaIndex); ///1

  int fifthCommaIndex = s.indexOf(';', fourthCommaIndex + 1);
  int fifthValue = s.substring(fourthCommaIndex + 1, fifthCommaIndex).toInt(); ///1

  int sixthCommaIndex = s.indexOf(';', fifthCommaIndex + 1);
  int sixthValue = s.substring(fifthCommaIndex + 1, sixthCommaIndex).toInt(); ///1
  String url;
  //url = "http://scare.iocare.in/input/post.json?";
  url = "http://www.iocare.in/hack/post-bins/";
  url += "1,";
  /* if (secondValue == 1) {
     lvl = sixthValue;
     USE_SERIAL.print("level\n");
     USE_SERIAL.println(lvl);
     url += "lvl:";
     url += lvl;
    } else {
     if (secondValue == 2) {
       wgt = sixthValue;
       USE_SERIAL.print("weight\n");
       USE_SERIAL.println(wgt);
       url += "wgt:";
       url += wgt;
     } else {
       String ts;
       int sixthCommaIndex = s.indexOf(';', fifthCommaIndex + 1);
       ts = s.substring(fifthCommaIndex + 1, sixthCommaIndex); ///1
       lats = stringToDouble(ts);
       url += "lat:";
       url += lats;
       USE_SERIAL.println(lats);
       int seventhCommaIndex = s.indexOf(';', sixthCommaIndex + 1);
       ts = s.substring(sixthCommaIndex + 1, seventhCommaIndex); ///1
       lngs = stringToDouble(ts);
       url += ",lng:";
       url += lngs;
       USE_SERIAL.println(lngs);
     }
    }*/

    if (secondValue == 1) {
      lvl = sixthValue;
      USE_SERIAL.print("level\n");
      USE_SERIAL.println(lvl);
      url += "lvl,";
      url += lvl;
    } else {
      if (secondValue == 2) {
        wgt = sixthValue;
        USE_SERIAL.print("weight\n");
        USE_SERIAL.println(wgt);
        url += "wgt,";
        url += wgt;
      } else {
        String ts;
        int sixthCommaIndex = s.indexOf(';', fifthCommaIndex + 1);
        lats = s.substring(fifthCommaIndex + 1, sixthCommaIndex); ///1
        USE_SERIAL.println(lats);
        //lats = stringToDouble(lats);
        url += "pos,";
        url += lats;
        
        int seventhCommaIndex = s.indexOf(';', sixthCommaIndex + 1);
        lngs = s.substring(sixthCommaIndex + 1, seventhCommaIndex); ///1
        USE_SERIAL.println(lngs);
        //lngs = stringToDouble(ts);
        url += ",";
        url += lngs;
        
      }
    }

    /*
      if (secondValue == 3) {
       lvl = sixthValue;
       USE_SERIAL.print("level\n");
       USE_SERIAL.println(lvl);

       url += lvl;
       url += ",";
       wgt = sixthValue;
       USE_SERIAL.print("weight\n");
       USE_SERIAL.println(wgt);

       url += wgt;
       url += ",";
       String ts;
       int sixthCommaIndex = s.indexOf(';', fifthCommaIndex + 1);
       ts = s.substring(fifthCommaIndex + 1, sixthCommaIndex); ///1
       lats = stringToDouble(ts);
       url += lats;
       url += ",";
       USE_SERIAL.println(lats);
       int seventhCommaIndex = s.indexOf(';', sixthCommaIndex + 1);
       ts = s.substring(sixthCommaIndex + 1, seventhCommaIndex); ///1
       lngs = stringToDouble(ts);
       url += lngs;
       USE_SERIAL.println(lngs);
      }
    */


    //url += ",data:";
    //url += rc;
    url += "";

    USE_SERIAL.print("[HTTP] begin...\n");
    // configure traged server and url{"power1":100,"power2":200,"power3":300}
    //http.begin("http://172.26.58.53/hack/post.php?data=%s", "7a 9c f4 db 40 d3 62 5a 6e 21 bc 5c cc 66 c8 3e a1 45 59 38"); //HTTPS
    HTTPClient http;
    //sprintf(url, 'http://scare.iocare.in/input/post?apikey=9c5b81163fe6882a8c23fe1a1a8c4b1f&id=0&data={"power1":"%s"}', rc);
    http.begin(url); //HTTP
    USE_SERIAL.printf("[HTTP] GET... url: %s\n", url.c_str());
    USE_SERIAL.print("[HTTP] GET...\n");
    // start connection and send HTTP header
    int httpCode = http.GET();

    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        USE_SERIAL.println(payload);
      }
    } else {
      USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();

  }

