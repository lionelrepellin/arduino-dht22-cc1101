#include <ELECHOUSE_CC1101.h>

const int n = 61;

void setup()
{
  Serial.begin(9600);
  Serial.println("Rx");
  ELECHOUSE_cc1101.Init(F_433);  // set frequency - F_433, F_868, F_965 MHz
  ELECHOUSE_cc1101.SetReceive();
}

byte buffer[61] = {0};

float temp;
float hum;
float feel;

union cvt {
   byte b[4];
   float value;
} u;


void loop()
{
  if (ELECHOUSE_cc1101.CheckReceiveFlag())
  {
    int len = ELECHOUSE_cc1101.ReceiveData(buffer);
    buffer[len] = '\0';

    Serial.print("Raw value: ");
    Serial.println((char *) buffer);
    Serial.println("----");

    temp = decodeTemperature(buffer);    
    
    Serial.print("Temp: ");
    Serial.println(temp);

    hum = decodeHumidity(buffer);
    
    Serial.print("Hum: ");
    Serial.println(hum);

    feel = decodeFeelTemp(buffer);
    
    Serial.print("Feel: ");
    Serial.println(feel);
    
    Serial.println("----");
    
    ELECHOUSE_cc1101.SetReceive();
  }
}

float decodeTemperature(byte buffer[]){
  u.b[0] = buffer[0];
  u.b[1] = buffer[1];
  u.b[2] = buffer[2];
  u.b[3] = buffer[3];
  return u.value;
}

float decodeHumidity(byte buffer[]){
  u.b[0] = buffer[4];
  u.b[1] = buffer[5];
  u.b[2] = buffer[6];
  u.b[3] = buffer[7];
  return u.value;
}

float decodeFeelTemp(byte buffer[]){
  u.b[0] = buffer[8];
  u.b[1] = buffer[9];
  u.b[2] = buffer[10];
  u.b[3] = buffer[11];
  return u.value;
}
