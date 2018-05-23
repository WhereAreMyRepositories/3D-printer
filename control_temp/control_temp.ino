#define TEMP_0_PIN         13  
#include<LiquidCrystal.h>
LiquidCrystal lcd(16,17,23,25,27,29);
int x;
double y;
#define a -6.33387388255690e-07
#define b 0.00102505642227912
#define c -0.622300381120344
#define d 244.188233988838

void setup() 
{
  Serial.begin(57600);
  lcd.begin(20,4); 
 
  lcd.print("xdxd beka");
}

void loop() 
{
  x = analogRead(TEMP_0_PIN);

  y = a*(double)pow(x,3) + b*(double)pow(x,2)+c*(double)x+d;
  Serial.println(x);
   lcd.setCursor(0,1);
  lcd.print(y);
}
