#include <LiquidCrystal.h>
#include <SD.h>
#include <SPI.h>
#include <DS1307.h>
#include <SoftwareSerial.h>

int rs=7;
int en=8;
int d4=9;
int d5=6;
int d6=A0;
int d7=A1;
int flamePin=A3;
int buzzPin=5;
int switchPin=3;
int switchState;
int flameVal;
int ledPin=2;
int cs=4;

File mySensorData;
SoftwareSerial sim(A1,2);
LiquidCrystal lcd(rs,en,d4,d5,d6,d7);
String number = " "; //Phone number


DS1307 rtc(SDA,SCL);
void setup() {
  Serial.begin(9600);
  sim.begin(9600);
  SD.begin(cs);
  rtc.begin();
  rtc.halt(false);
  //rtc.setDOW(TUESDAY);
  //rtc.setTime(21,53,0);
  //rtc.setDate(28,04,2020);
  rtc.setSQWRate(SQW_RATE_1);
  rtc.enableSQW(true);
  lcd.begin(16,2);
  pinMode(flamePin,INPUT);
  pinMode(ledPin,OUTPUT);
  pinMode(switchPin,INPUT);
  pinMode(buzzPin,OUTPUT);
  pinMode(10,OUTPUT);
  digitalWrite(switchPin,HIGH);
  digitalWrite(ledPin,LOW);

}

void loop() {
  switchState=digitalRead(switchPin);
  flameVal=digitalRead(flamePin);

  delay(2);
  
  if(switchState==1&&flameVal==0){  
  digitalWrite(ledPin,HIGH);
  digitalWrite(buzzPin,HIGH);
  lcd.setCursor(0,0);
  lcd.print("Sending Message...");
  sim.println("AT+CMGF=1");
  delay(1000);
  sim.println("AT+CMGS=\"" + number + "\"\r");
  delay(1000);
  String SMS = "Fire detetced at your house";
  sim.println(SMS);
  delay(100);
  sim.println((char)26);
  delay(1000);
   
    while(digitalRead(switchPin)==1){
      lcd.setCursor(0,0);
    lcd.print("Fire Detected at");
    lcd.setCursor(0,1);
    lcd.print(rtc.getTimeStr());
    delay(2000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Saving Data to");
    lcd.setCursor(0,1);
    lcd.print("SD Card...");
    mySensorData=SD.open("Flame.txt",FILE_WRITE);
     
     if(mySensorData){
      Serial.println("SD Active");
      mySensorData.print("Time: ");
      mySensorData.print(rtc.getTimeStr());
      mySensorData.print(",");
      mySensorData.print("Date: ");
      mySensorData.println(rtc.getDateStr());
      mySensorData.close();
      
    }
    
    digitalWrite(ledPin,HIGH);
    digitalWrite(buzzPin,HIGH);
    delay(1000);
    }
 
 }
 
 
   if(switchState==0){
   lcd.setCursor(5,0);
   lcd.print("Alarm");
   lcd.setCursor(2,1);
   lcd.print("Deactivated");
   delay(1000);
   digitalWrite(ledPin,LOW);
   digitalWrite(buzzPin,LOW); 
   flameVal=1;
  }
   
    
   
  if(flameVal==1 && switchState==1){
  lcd.setCursor(0,0);
  lcd.print("Alarm Off");
  lcd.setCursor(11,0);
  lcd.print(rtc.getTimeStr());
  lcd.setCursor(0,1);
  lcd.print(rtc.getDateStr());
  delay(1000);
  lcd.clear(); 
    }
        
if(switchState==0 && flameVal==0){
   lcd.setCursor(0,0);
   lcd.print("Alarm Deactivated");
   delay(1000);
   digitalWrite(ledPin,LOW);
   digitalWrite(buzzPin,LOW); 
  }
 
    lcd.clear();
}
