#define X_MIN_PIN           3  // obsluga krancowki od PSA X
#ifndef X_MAX_PIN
#define X_MAX_PIN         2
#endif
#define Y_MIN_PIN          14  // obsluga krancowki od PSA Y
#define Y_MAX_PIN          15
#define Z_MIN_PIN          18  // obsluga krancowki od PSA Z
#define Z_MAX_PIN          19


#define X_STEP_PIN         54
#define X_DIR_PIN          55
#define X_ENABLE_PIN       38
#define X_CS_PIN           53

#define Y_STEP_PIN         60
#define Y_DIR_PIN          61
#define Y_ENABLE_PIN       56
#define Y_CS_PIN           49

#define Z_STEP_PIN         46
#define Z_DIR_PIN          48
#define Z_ENABLE_PIN       62
#define Z_CS_PIN           40

/////////////////////////     TEMP  /////////////////////////////////////
#define TEMP_0_PIN         13
#include<LiquidCrystal.h>
LiquidCrystal lcd(16, 17, 23, 25, 27, 29);
#define a -6.33387388255690e-07
#define b 0.00102505642227912
#define c -0.622300381120344
#define d 244.188233988838
#define HEATER0     10
#define FAN0 44

int x_t;
double y_t = -1;
double e_t = 0;
double u_t = 0;
double i_t = 0;

bool stop = false;
bool fan = false;
bool heater = false;
int temp = -1;
int x;
int y;
int z;
int krokx = 0;
int kroky = 0;
int krokz = 0;
int tmp;
bool wyslane = true;

String s;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);

  lcd.begin(20, 4);
  pinMode(FAN0, OUTPUT);

  pinMode(X_MAX_PIN, INPUT_PULLUP);
  pinMode(Y_MAX_PIN, INPUT_PULLUP);
  pinMode(Z_MAX_PIN, INPUT_PULLUP);

  pinMode(X_ENABLE_PIN, OUTPUT);
  digitalWrite(X_ENABLE_PIN, LOW);
  pinMode(X_DIR_PIN, OUTPUT);
  digitalWrite(X_DIR_PIN, LOW);
  pinMode(X_STEP_PIN, OUTPUT);

  pinMode(Y_ENABLE_PIN, OUTPUT);
  digitalWrite(Y_ENABLE_PIN, LOW);
  pinMode(Y_DIR_PIN, OUTPUT);
  digitalWrite(Y_DIR_PIN, LOW);
  pinMode(Y_STEP_PIN, OUTPUT);

  pinMode(Z_ENABLE_PIN, OUTPUT);
  digitalWrite(Z_ENABLE_PIN, LOW);
  pinMode(Z_DIR_PIN, OUTPUT);
  digitalWrite(Z_DIR_PIN, LOW);
  pinMode(Z_STEP_PIN, OUTPUT);

  pinMode(X_MIN_PIN, INPUT);
}

void loop() {

  
    lcd.setCursor(0,0);
    lcd.print("TEMP ");
    lcd.print(y_t);
    lcd.print("/");
    lcd.print(temp);
    lcd.print("     ");
    lcd.setCursor(0,1);
    lcd.print("HEATER ");
    lcd.print(heater);
    lcd.print("     ");
    lcd.setCursor(0,2);
    lcd.print("FAN ");
    lcd.print(fan);
    lcd.print("     ");
    lcd.setCursor(0,3);
    lcd.print("STOP ");
    lcd.print(stop);
    lcd.print("     "); 

  x =  digitalRead(X_MAX_PIN);
  y =  digitalRead(Y_MAX_PIN);
  z =  digitalRead(Z_MAX_PIN);
  //  Serial.print(x);
  //  Serial.print(y);
  //  Serial.println(z);

  if ( Serial.available() > 0)
  {
    s = Serial.readStringUntil('\n');
    if (s.indexOf("STOP") >= 0)                              
      sscanf(s.c_str(), "STOP %d", &stop);                   
    else if (s.indexOf("TEMP") >= 0)                            
      sscanf(s.c_str(), "TEMP %d", &temp);
    else if (s.indexOf("FAN") >= 0)                             
      sscanf(s.c_str(), "FAN %d", &fan);
    else if (s.indexOf("HEATER") >= 0)                    
      sscanf(s.c_str(), "HEATER %d", &heater);              
    else
    {
      sscanf(s.c_str(), "X%d Y%d Z%d", &krokx, &kroky, &krokz);
      wyslane = false;

      if (krokx < 0)
      {
        digitalWrite(X_DIR_PIN, HIGH);
        krokx = -krokx;

      } else {
        digitalWrite(X_DIR_PIN, LOW);
      }

      if (kroky < 0)
      {
        digitalWrite(Y_DIR_PIN, HIGH);
        kroky = -kroky;

      } else {
        digitalWrite(Y_DIR_PIN, LOW);
      }

      if (krokz < 0)
      {
        digitalWrite(Z_DIR_PIN, HIGH);
        krokz = -krokz;

      } else {
        digitalWrite(Z_DIR_PIN, LOW);
      }
    }
	
	if (stop)
	{
		fan = false;
		temp = -1;
		heater = false;
	}

    if (fan)
    {
      digitalWrite(FAN0, HIGH);
    }
    else
    {
      digitalWrite(FAN0, LOW);
    }

	if(temp == -1)
	{
		if (heater)
		  digitalWrite(HEATER0, HIGH);
		 else 
		  digitalWrite(HEATER0, LOW);
	}

    if (temp != -1)
    {
      e_t = temp - y_t;
      u_t = 255 / (temp - 20) * e_t + i_t / 7;
      i_t = i_t + 0.01 * e_t;
      x_t = analogRead(TEMP_0_PIN);
      y_t = a * (double)pow(x_t, 3) + b * (double)pow(x_t, 2) + c * (double)x_t + d;

      if (y_t < temp)
      {
        analogWrite(HEATER0, u_t > 255 ? 255 : floor(u_t));
        heater = true;
      }
      else
      {
        digitalWrite(HEATER0, LOW);
        heater = false;
      }
    }

  }

  if (x == 0 && krokx > 0)
  {
    digitalWrite(X_STEP_PIN, HIGH);
    krokx --;
  }
  if (y == 0 && kroky > 0)
  {
    digitalWrite(Y_STEP_PIN, HIGH);
    kroky--;
  }
  if (z == 0 && krokz > 0)
  {
    digitalWrite(Z_STEP_PIN, HIGH);
    krokz--;
  }
  delayMicroseconds(300);
  digitalWrite(X_STEP_PIN, LOW);
  digitalWrite(Y_STEP_PIN, LOW);
  digitalWrite(Z_STEP_PIN, LOW);
  delayMicroseconds(300);

  if (krokx == 0 && kroky == 0 && krokz == 0 && wyslane == false)
  {
    Serial.println("OK");
    wyslane = true;
  }
  if (digitalRead(X_MIN_PIN) == 0) 
  {
    digitalWrite(X_ENABLE_PIN, HIGH);
    digitalWrite(Y_ENABLE_PIN, HIGH); 
    digitalWrite(Z_ENABLE_PIN, HIGH);
  }
  else
  {
    digitalWrite(X_ENABLE_PIN, LOW);
    digitalWrite(Y_ENABLE_PIN, LOW); 
    digitalWrite(Z_ENABLE_PIN, LOW);
  }
}
