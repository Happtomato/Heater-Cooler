#include <LiquidCrystal.h>
 
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int sensorPin = A0;
int motorPinKalt = A5;
int motorPinHeiss = A4;
 
int LED_ROT = 10;
int LED_GRUEN = 9;
int LED_BLAU = 8;
int LED_ORANGE = 7;
int valueOld = 0;
int s = 50;
int m = 59;
int h = 17;
 
 
void setup()
{
    pinMode(LED_ROT, OUTPUT);
    pinMode(LED_GRUEN, OUTPUT);
    pinMode(LED_BLAU, OUTPUT);
    pinMode(LED_ORANGE, OUTPUT);
    Serial.begin(9600);
 
    lcd.begin(16, 2);
}
 
void loop() {
 
//Calculate Temperature and round it and display temperature 
    int sensorWert = analogRead(A0);
    float voltage = sensorWert * 5.0;
    voltage /= 1024.0;
    float temp = (voltage - 0.5) * 100 ;
 
    temp = roundTemp(temp, 0);
 
    lcd.setCursor(0, 1);
    lcd.print("T:");
 
    lcd.setCursor(2, 1);
    lcd.print(temp);
 
    valueOld = temp;
 
 
lcd.setCursor(9, 1);
  	lcd.print("Stat:");
  
  //set actors and print coresponding action
    if(checkTime() == true){
        //turn on led for respective case
        if(temp <= 24 && temp >= 22){
            setActors(0,0,1,0,0,0);
lcd.print("I.");
        }
 
        else if(temp > 24){
            setActors(0,1000,0,0,1,0);
          	lcd.print("C.");
        }
 
        else if(temp < 22){
            setActors(1000,0,0,1,0,0);
lcd.print("H.");
        }
    }
    else{
        setActors(0,0,0,0,0,1);
      	lcd.print("S.");
    }
 
    delay (1000);
    s++;
 
 
    if(s == 60){
        m++;
        s = 0;
    }
    if(m == 60){
        
        h++;
        m = 0;
    }
    if(h == 24){
        
        h = 0;
    }
  //construct clock
    lcd.setCursor(0,0);
    lcd.print("Time ");
    if(h<10)lcd.print("0");
    lcd.print(h);
    lcd.print(":");
    if(m<10)lcd.print("0");
    lcd.print(m);
    lcd.print(":");
    if(s<10)lcd.print("0");
    lcd.print(s);
}
 
//round temperature using given value and decimal
float oldTemp;
int oldX;
float roundTemp(float val, byte dec) {
 
    if(val != oldTemp){
 
        float x = val * pow(10, dec);
        float y = round(x);
        float z = x - y;
        if ((int) z == 5) {
            y++;
        } else {
        }
 
        oldTemp = val;
 
 
        x = y / pow(10, dec);
 
        oldX = x;

        return x;
    }
    return oldX;
}
 
//check the current "time" and return true or false
bool checkTime(){
    if(h < 18){
        if(h < 8){
            Serial.print("true");
            return true;
        }
        Serial.print("false");
        return false;
    }
    else{
        Serial.print("true");
        return true;
    }
}
//controls the actors using given values
void setActors(int speedH, int speedC, int ledGruen, int ledRot,  int ledBlau, int ledOrange){
    int geschwindigkeitH = speedH;
    int geschwindigkeitK = speedC;
    analogWrite(motorPinHeiss, geschwindigkeitH);
    analogWrite(motorPinKalt, geschwindigkeitK);
    digitalWrite(LED_GRUEN,ledGruen);
    digitalWrite(LED_ROT, ledRot);
    digitalWrite(LED_BLAU, ledBlau);
    digitalWrite(LED_ORANGE, ledOrange);
}
