#include <LiquidCrystal.h>
#include <pins_arduino.h>
#include <Arduino.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int sensorPin = A0;
int motorPinKalt = A5;
int motorPinHeiss = A4;

int LED_ROT = 10;
int LED_GRUEN = 9;
int LED_BLAU = 8;
int LED_ORANGE = 7;
int valueOld = 0;
int s = 0;
int m = 0;
int h = 0;


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


    int sensorWert = analogRead(A0);
    float voltage = sensorWert * 5.0;
    voltage /= 1024.0;
    float temp = (voltage - 0.5) * 100 ;

    temp = roundTemp(temp, 0);

    lcd.setCursor(0, 1);
    lcd.print("T:");

    lcd.setCursor(3, 1);
    lcd.print(temp);

    valueOld = temp;



    if(checkTime){
        //turn on led for respective case
        if(temp <= 24 && temp >= 22){
            setActors(0,0,1,0,0,0);

        }

        else if(temp > 24){
            setActors(0,1000,0,0,1,0);
        }

        else if(temp < 22){
            setActors(1000,0,0,1,0,0);
        }
    }
    delay (1000);
    s++;

    boolean reset = false;
    if(s == 60){
        reset = true;
        m++;
        s = 0;
    }
    if(m == 60){
        lcd.clear();
        h++;
        m = 0;
    }
    if(h == 24){
        lcd.clear();
        h = 0;
    }

    lcd.setCursor(0,0);



    lcd.print("Zeit: " );
    if( h >= 1){
        lcd.print(h);
    }

    else if(h == 0){
        lcd.print("00");
    }

    lcd.print(":");
    if(m >= 1){
        lcd.print(m);
    }

    else if(m == 0){
        lcd.print("00");
    }

    lcd.print(":");
    if(s >= 1){
        lcd.print(s);
    }
    else if(h == 0){
        lcd.print("00");
    }
    if(reset){
        lcd.clear();
    }
    reset = false;

    if(!checkTime){
        setActors(0,0,0,0,0,1);
    }



}

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


bool checkTime(){
    if((h >= 8 && m >= 0) && (h <= 17 && m <= 59)){
        return false;
        Serial.print("true");
    }
    Serial.print("false");
    return true;

}

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