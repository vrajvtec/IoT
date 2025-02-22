/* 
 * Pinouts Of Input Sensors, I2C LCD, Relays
 * 
 * D15 ----------> Pump Relay
 * 
 * D4 ----------> Humidity Relay
 * 
 * D13 ----------> Light Relay
 * 
 * D26 ----------> Temperature Relay
 * 
 * DHT Signal ----------> D5
 * 
 * Soil Moisture Input -------> VP (GPIO 36 also known as A0)  
 * 
 * LDR Input --------> D34 (With ADC)
 * 
 * I2C LCD (SDA) ---------> D21 dont change this
 * 
 * I2C LCD (SCL) ---------> D22 dont change this
 * 
 * 
 * Blynk Virtual Pins

 * 
 */


// including the required libararies 
#define BLYNK_PRINT Serial


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#include <LiquidCrystal_I2C.h>

int lcdColumns = 16;
int lcdRows = 2;


LiquidCrystal_I2C lcd(0x3F, lcdColumns, lcdRows);  //Ox3F is I2C address of LCD, dont change



// Wifi Connectivity
char ssid[] = " "; // input your WiFi SSID
char pass[] = " "; // input your WiFi Password
char auth[] = " "; // add your blynk code

//====dht11 part

#include "DHTesp.h"

#define DHTpin 5    //D5 of ESP32 DevKit

DHTesp dht;

//===================Relay Part============

#define relyPump 15
#define relyHumidity 4
#define relyLight 13
#define relyTemperature 26 //IN4

//============soil moisture part=======

const int moisturePin = A0; 
int moisturePercentage;    // Variable for analog Moisuture Input
int value ;



//========ldr part========

#define ldrPin 34
float light; // Variable for analog Light Input
float lightP = 0; // Variable for storing Input


void setup() {

//  
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
   pinMode(relyPump, OUTPUT);
  pinMode(relyHumidity, OUTPUT);
  pinMode(relyLight, OUTPUT);
  pinMode(relyTemperature , OUTPUT);
  dht.setup(DHTpin, DHTesp::DHT11);
    lcd.init();
  // turn on LCD backlight                      
  lcd.backlight();


  }

void loop() {
    Blynk.run();
    ldr();
    soilMoistureSensor();
    dhtSensor();
  
    
}



void ldr(){
  
  light = analogRead(ldrPin);
  
  lightP = (light/4095)*100;
  Serial.println(lightP);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Light: ");
  lcd.print(lightP);
  lcd.print(" % ");
  delay(1000);
  Blynk.virtualWrite(V4,lightP); 

  if (lightP < 20 ){
    digitalWrite(relyLight,LOW); //Turning on Light Relay
    Serial.println("Light Relay On ");
    delay(100);
  }

  else{
     digitalWrite(relyLight,HIGH);
  }
  

  

}


//=============================DHT 11 Sensor==========================

void dhtSensor(){
  delay(dht.getMinimumSamplingPeriod());

  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();

  Serial.println("humidity="+String(humidity));
  Serial.println("temp = "+ String(temperature) + " °C");
  delay(100);
  Blynk.virtualWrite(V3,temperature);
  Blynk.virtualWrite(V2,humidity);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Temp : ");
  lcd.print(temperature);
  lcd.print(" ");
  lcd.print(char(223));
  lcd.print("C");
  lcd.setCursor(0,1);
  lcd.print("Humidity :");
  lcd.print(humidity);
  delay(1000);
//==== Relay Conditions ========
  if (temperature < 27){
    digitalWrite(relyTemperature,LOW);
     Serial.println("temp Relay On ");
    delay(100);
    
  }
  else {
    digitalWrite(relyTemperature,HIGH);
  }
  delay(400);
}

void soilMoistureSensor(){
      moisturePercentage = ( 100.00 - ( (analogRead(moisturePin) / 4095.00) * 100.00 ) );
      Serial.print("Moisture : ");
      Serial.print(moisturePercentage);
      Serial.println("%");
      Serial.println(" ");
     
      lcd.setCursor(0,1);
      lcd.print("Moisture : ");
      lcd.print(moisturePercentage);
      lcd.print(" % ");
       delay(1000);
      
      Blynk.virtualWrite(V8,moisturePercentage);
     // lcd.setCursor(
      if (moisturePercentage < 35 ) {
       digitalWrite(relyPump,LOW);//
         Serial.println("Pump Relay On ");
    delay(100);
      }
      else if (moisturePercentage > 60) {
     digitalWrite(relyPump,HIGH);
          }
  
}
© 2021 GitHub, Inc.void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
