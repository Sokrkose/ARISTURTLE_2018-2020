#include <DallasTemperature.h>
#include <OneWire.h>
#define ONE_WIRE_BUS A4
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);

#define target 50

#define period 10

float temp=0.0;
float peak=0.0;
short int temp100=0; 
bool relay=HIGH;  //OFF
bool phase2peakflag=false;
bool cycleflag=true;
bool bake=false;
int inByte=200;

unsigned long startmil=millis();
unsigned long stopmil=millis();
int phase=0;
int dutycycle=50;

  /*
  phase0 = sensor error
  phase1 = preheat to 76
  phase2 = let it reach 80
  phase3 = 10sec period, duty cycle 50% (variable)
  */

void setup() {
  Serial.begin(115200);
  Serial.print("Carbon Oven initialization\n");
  pinMode(12, OUTPUT);
  digitalWrite(12, relay);
  DS18B20.begin();
  DS18B20.requestTemperatures();
  delay(10);
  temp=DS18B20.getTempCByIndex(0);
  temp100=temp*100;
  if (temp<0 || temp>120) {
    phase=0;
  } else if (temp<target-5) {
    phase=1;
  } else {
    phase=3;
  }
  delay(1000);
  startmil=millis();
}

void loop() {



// se periptosh pou xasei kapoia metrhsh to thermistor kai steilei -127, ginetai ksana reset!
   if(temp < 0){
      pinMode(12, OUTPUT);
      digitalWrite(12, relay);
      DS18B20.begin();
      DS18B20.requestTemperatures();
      delay(10);
      temp=DS18B20.getTempCByIndex(0);
      temp100=temp*100;
      if (temp<0 || temp>120) {
        phase=0;
      } else if (temp<target-5) {
        phase=1;
      } else {
        phase=3;
      }
      delay(1000);
      startmil=millis();
   }




  if (Serial.available() > 0) {
    inByte=Serial.read();
    if (inByte==1) {
      phase=1;
    } else if (inByte==2){
      phase=2;
    } else if (inByte==3){
      phase=3;
    } else if (inByte==4){
      phase=4;
 //   } else if (inByte>7 && inByte<20){
 //     period=inByte;
 //   } else if (inByte==40 || inByte==60 || inByte==80 || inByte==90 || inByte==110){
 //     target=inByte;
    } else {
      Serial.println("Command not found");
    }
  }
/*  
  if (bake==false) {
    relay=HIGH;
    digitalWrite(2,relay);
  } else {
*/  
  DS18B20.requestTemperatures();
  delay(10);
  temp=DS18B20.getTempCByIndex(0);
  temp100=100*temp;

  //----- phase detect
  if (temp<0 || temp>120) {
    phase=0;
  } else if (phase==4){
    phase=4; //do nothing
  } else if (temp<target-10) {
    phase=1;
  }

  if (phase==0) {
    relay=LOW;
    digitalWrite(12,relay);
  } else if (phase==1){
    if (temp<target-5) {
      relay=HIGH;  //ON
    } else {
      relay=LOW; //OFF
      phase=2;
      phase2peakflag=false;
      cycleflag=true;
    }
  } else if (phase==2){
    /*relay=HIGH; //OFF
    if (temp>peak) {
      peak=temp;
    } else if (temp<peak-0.1){
      dutycycle=40*(target-temp)/3;
      if (dutycycle>40) {
        dutycycle=40;
      } else if (dutycycle<-40){
        dutycycle=-40;
      }
      dutycycle=dutycycle+50;
      */
      dutycycle=70*(target-temp)/5;
      if (dutycycle>70) {
        dutycycle=70;
      } else if (dutycycle<-70){
        dutycycle=-70;
      }
      dutycycle=dutycycle+20;
      if (cycleflag){
        relay=HIGH;  //ON
        if (millis()-startmil>period*10*dutycycle){
          relay=LOW;  //OFF
          cycleflag=false; //toggle dutycycle half period
          startmil=millis();  // start counting for the new half period
        }
      } else {
        relay=LOW;  //OFF
        if (millis()-startmil>period*10*(100-dutycycle)){
          relay=HIGH;  //ON
          cycleflag=true; //toggle dutycycle half period
          startmil=millis();  // start counting for the new half period
        }
      }
      if (temp>target-1){
           startmil=millis();
        phase=3;
        cycleflag=true;
        peak=0.0;
      }
    //}
  } else if (phase==3){
    dutycycle=40*(target-temp)/0.7;
    if (dutycycle>40) {
      dutycycle=40;
    } else if (dutycycle<-40){
      dutycycle=-40;
    }
    dutycycle=dutycycle+50;
    if (cycleflag){
      relay=HIGH;  //ON
      if (millis()-startmil>period*10*dutycycle){
        relay=LOW;  //OFF
        cycleflag=false; //toggle dutycycle half period
        startmil=millis();  // start counting for the new half period
      }
    } else {
      relay=LOW;  //OFF
      if (millis()-startmil>period*10*(100-dutycycle)){
        relay=HIGH;  //ON
        cycleflag=true; //toggle dutycycle half period
        startmil=millis();  // start counting for the new half period
      }
    }
  } else if(phase==4){
    relay=LOW; //OFF
    //do nothing
  }

//  }
  
  digitalWrite(12, relay);
  Serial.write("Temp:");
  Serial.write(temp100);
  Serial.write(temp100>>8);
  Serial.write(relay);
  Serial.write(relay>>8);
  Serial.write(phase);
  Serial.write(phase>>8);
//  Serial.write(inByte);
//  Serial.write(inByte>>8);
//  Serial.write(target);
//  Serial.write(target>>8);
//  Serial.write(period);
//  Serial.write(period>>8);
  Serial.write(0x00);
  Serial.print("\t");
  Serial.println(temp);
  if (relay) {
    Serial.println("Resistors are \tON");
  } else {
    Serial.println("Resistors are \tOFF");
  }
  delay(100);
}
