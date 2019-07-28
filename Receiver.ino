#include <LiquidCrystal_I2C.h>
#include <ELECHOUSE_CC1101.h>

const int n = 61;
//const int n = 16;

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup()
{
  Serial.begin(9600);
  Serial.println("Rx");
  ELECHOUSE_cc1101.Init(F_433);  // set frequency - F_433, F_868, F_965 MHz
  ELECHOUSE_cc1101.SetReceive();

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Ready!");
}

byte buffer[61] = {0};
//byte buffer[n] = {0};

float temp;
float hum;
float felt;

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

    if(readChecksum(buffer)) 
    {      
      temp = decodeTemperature(buffer);    
      displayTemperature(temp);
    
      Serial.print("Temp: ");
      Serial.println(temp);

      hum = decodeHumidity(buffer);
      displayHumidity(hum);
    
      Serial.print("Hum: ");
      Serial.println(hum);

      felt = decodeFeltTemp(buffer);
      displayFeltTemp(felt);
    
      Serial.print("Felt: ");
      Serial.println(felt);
    
      Serial.println("----");
    
      ELECHOUSE_cc1101.SetReceive();
    }
    else 
    {
      lcd.setCursor(0, 0);
      lcd.print("Error");
    }
    
  }
}

bool readChecksum(byte buffer[]) 
{
  if(buffer[0] == buffer[12] && buffer[4] == buffer[13] && buffer[8] == buffer[14]) {
    Serial.print("CRC ok !");
    return true;
  }
  else { 
    Serial.print("Wrong CRC!");
    return false;
  }
}

void displayTemperature(float temp)
{
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print((int)temp);
  lcd.print("C");
}

void displayHumidity(float hum) 
{
  lcd.setCursor(0, 1);
  lcd.print("Hum:  ");
  lcd.print((int)hum);
  lcd.print("%");
}

void displayFeltTemp(float felt) 
{
  lcd.setCursor(11, 0);
  lcd.print("Felt:");
  lcd.setCursor(12, 1);
  lcd.print((int)felt);
  lcd.print("C");
}

float decodeTemperature(byte buffer[])
{
  u.b[0] = buffer[0];
  u.b[1] = buffer[1];
  u.b[2] = buffer[2];
  u.b[3] = buffer[3];
  return u.value;
}

float decodeHumidity(byte buffer[])
{
  u.b[0] = buffer[4];
  u.b[1] = buffer[5];
  u.b[2] = buffer[6];
  u.b[3] = buffer[7];
  return u.value;
}

float decodeFeltTemp(byte buffer[])
{
  u.b[0] = buffer[8];
  u.b[1] = buffer[9];
  u.b[2] = buffer[10];
  u.b[3] = buffer[11];
  return u.value;
}
