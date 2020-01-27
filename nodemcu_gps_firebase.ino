#include "TinyGPS++.h"
#include "SoftwareSerial.h"
#include <FirebaseArduino.h>
#include <ESP8266WiFi.h>

// D1 --> RX
// D2 --> TX

#define GPS_UART_RX_PIN 4
#define GPS_UART_TX_PIN 5

#define FIREBASE_HOST "hasystem-b6df4.firebaseio.com"
#define FIREBASE_AUTH "rmKKMaGyuC5Xx7qPjylKaX751ke3YNONSsWzKy2y"
#define WIFI_SSID "SLT-ADSL-296A1" 
#define WIFI_PASSWORD "adsl1234" 

SoftwareSerial serial_connection(GPS_UART_RX_PIN,GPS_UART_TX_PIN);
TinyGPSPlus gps;

void setup() {
  Serial.begin(9600);   
  serial_connection.begin(9600);            
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD); 
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP());
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.setString("vehicle_tracking/train_details/ruhunukumari/wifi_local", "HelloWorld");
}

void loop()
{
  while(serial_connection.available()){
    gps.encode(serial_connection.read());
  }
  if(gps.location.isUpdated()){
    //-----------------------------
    Firebase.setString("vehicle_tracking/train_details/ruhunukumari/sat_count", String(gps.satellites.value()));
    Firebase.setString("vehicle_tracking/train_details/ruhunukumari/latitude", String((gps.location.lat(), 6)));
    Firebase.setString("vehicle_tracking/train_details/ruhunukumari/longitude", String((gps.location.lng(), 6)));
    Firebase.setString("vehicle_tracking/train_details/ruhunukumari/speed", String(gps.speed.kmph()));
    Firebase.setString("vehicle_tracking/train_details/ruhunukumari/train_name", "Ruhunu Kumari");
    Firebase.setString("vehicle_tracking/train_details/ruhunukumari/altitude", String(gps.altitude.meters()));
    delay(5000);
  }
  Serial.println("Looping...");
}

/* 
Serial.println(gps.location.lat(), 6); // Latitude in degrees (double)
Serial.println(gps.location.lng(), 6); // Longitude in degrees (double)
Serial.print(gps.location.rawLat().negative ? "-" : "+");
Serial.println(gps.location.rawLat().deg); // Raw latitude in whole degrees
Serial.println(gps.location.rawLat().billionths);// ... and billionths (u16/u32)
Serial.print(gps.location.rawLng().negative ? "-" : "+");
Serial.println(gps.location.rawLng().deg); // Raw longitude in whole degrees
Serial.println(gps.location.rawLng().billionths);// ... and billionths (u16/u32)
Serial.println(gps.date.value()); // Raw date in DDMMYY format (u32)
Serial.println(gps.date.year()); // Year (2000+) (u16)
Serial.println(gps.date.month()); // Month (1-12) (u8)
Serial.println(gps.date.day()); // Day (1-31) (u8)
Serial.println(gps.time.value()); // Raw time in HHMMSSCC format (u32)
Serial.println(gps.time.hour()); // Hour (0-23) (u8)
Serial.println(gps.time.minute()); // Minute (0-59) (u8)
Serial.println(gps.time.second()); // Second (0-59) (u8)
Serial.println(gps.time.centisecond()); // 100ths of a second (0-99) (u8)
Serial.println(gps.speed.value()); // Raw speed in 100ths of a knot (i32)
Serial.println(gps.speed.knots()); // Speed in knots (double)
Serial.println(gps.speed.mph()); // Speed in miles per hour (double)
Serial.println(gps.speed.mps()); // Speed in meters per second (double)
Serial.println(gps.speed.kmph()); // Speed in kilometers per hour (double)
Serial.println(gps.course.value()); // Raw course in 100ths of a degree (i32)
Serial.println(gps.course.deg()); // Course in degrees (double)
Serial.println(gps.altitude.value()); // Raw altitude in centimeters (i32)
Serial.println(gps.altitude.meters()); // Altitude in meters (double)
Serial.println(gps.altitude.miles()); // Altitude in miles (double)
Serial.println(gps.altitude.kilometers()); // Altitude in kilometers (double)
Serial.println(gps.altitude.feet()); // Altitude in feet (double)
Serial.println(gps.satellites.value()); // Number of satellites in use (u32)
Serial.println(gps.hdop.value()); // Horizontal Dim. of Precision (100ths-i32)
*/
