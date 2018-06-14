#define TEMP_0_PIN         13  
#include<LiquidCrystal.h>
LiquidCrystal lcd(16,17,23,25,27,29);
int x;
int zadana = 150;
double y,e,u;
double i = 0;
#define a -6.33387388255690e-07
#define b 0.00102505642227912
#define c -0.622300381120344
#define d 244.188233988838

#define HEATER0     10
#define FAN 44

void setup() 
{
  Serial.begin(57600);
  lcd.begin(20,4); 
  pinMode(FAN,OUTPUT);
  lcd.print("xdxd beka");
   digitalWrite(FAN,HIGH);
}

void loop() 
{
  
  e = zadana - y;
  u = 255/(zadana - 20) * e + i/7;
  i = i + 0.01*e;
  x = analogRead(TEMP_0_PIN);
  y = a*(double)pow(x,3) + b*(double)pow(x,2)+c*(double)x+d;
  Serial.println(x);
   lcd.setCursor(0,1);
   lcd.print("temp: ");
  lcd.print(y);
  
  if(y<zadana)
  {
    lcd.setCursor(0,2);
    analogWrite(HEATER0,u>255 ? 255 : floor(u));
    lcd.print("Heater:ON ");
    lcd.setCursor(0,3);
    lcd.print("U: ");
    lcd.print(u);
    lcd.setCursor(0,0);
    lcd.print("I:  ");
    lcd.print(i);
    
    
    }
    else
    {
      lcd.setCursor(0,2);
     digitalWrite(HEATER0,LOW);
     lcd.print("Heater:OFF");
     
    }
    delay(10);
 }
