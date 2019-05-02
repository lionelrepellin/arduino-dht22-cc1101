#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <ELECHOUSE_CC1101.h>

#define DHTPIN 4
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

const int n = 61;
byte buffer[n] = "";

void setup() {
  Serial.begin(9600);
  dht.begin();
  ELECHOUSE_cc1101.Init(F_433); // set frequency - F_433, F_868, F_965 MHz
}

union cvt {
  float value;
  byte b[4];
} u;

void loop() {
  delay(2000);

  float t = dht.readTemperature(); // celsius
  float f = dht.readTemperature(true); // farenheit
  float h = dht.readHumidity();

  if(isnan(t) || isnan(h) || isnan(f)) {
    return;
  }

  float hi = dht.computeHeatIndex(f, h);
  float feel = dht.convertFtoC(hi);
  
  Serial.print("Temp: ");
  Serial.println(t);
  Serial.print("Hum: ");
  Serial.println(h);
  Serial.print("Feel temp: ");
  Serial.println(feel);
  Serial.println("----");
  
  encodeTemperature(buffer, t);
  encodeHumidity(buffer, h);
  encodeFeelTemp(buffer, feel);
   
  int len = sizeof(buffer);
  buffer[len] = '\0';
    
  ELECHOUSE_cc1101.SendData(buffer, len);
}

void encodeTemperature(byte buffer[], float temp) {
  u.value = temp;
  buffer[0] = u.b[0];
  buffer[1] = u.b[1];
  buffer[2] = u.b[2];
  buffer[3] = u.b[3];
}

void encodeHumidity(byte buffer[], float hum) {
  u.value = hum;
  buffer[4] = u.b[0];
  buffer[5] = u.b[1];
  buffer[6] = u.b[2];
  buffer[7] = u.b[3];
}

void encodeFeelTemp(byte buffer[], float feel) {
  u.value = feel;
  buffer[8] = u.b[0];
  buffer[9] = u.b[1];
  buffer[10] = u.b[2];
  buffer[11] = u.b[3];
}
