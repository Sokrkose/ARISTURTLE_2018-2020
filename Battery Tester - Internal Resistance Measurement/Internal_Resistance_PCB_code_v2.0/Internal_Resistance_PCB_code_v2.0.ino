#include <Arduino.h>
#include <SPI.h>
#include <LiquidCrystal.h>
#include "LTC2630.h"
#include "ADCCONVERT.h"

LTC2630 dac;
ADCCONVERT adc;

uint8_t V_control = 61;          //choose DAC Digital Input 0-255

int V_ctrl = (int ) V_control;   //1 byte to 2 byte conversion
float a = (float ) V_ctrl/255;   
float V = a*4.97;                //V is the DAC output voltage
float Id = V/0.150;              //Id is the Current drawn by the battery
                                 //when a resistor of 150mOhm is used

//uint16_t V_batt;                 //Battery Voltage
uint16_t adcOutput;
uint16_t V_load;                 //Load Voltage
uint16_t V_OC;                   //Open Circuit Battery Voltage
uint16_t V_batt_table[50];       //Battery samples
int CS2 = 44;

int button = 29;

const int rs = 35, en = 33, d4 = 32, d5 = 31, d6 = 30, d7 = 15;  // LCD Pinout
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  Serial.begin(9600);
  SPI.begin();
  
  pinMode(53, OUTPUT);           //DAC Chip Select (CS)
  pinMode(44, OUTPUT);           //ADC Chip Select (CS)
  pinMode(29, INPUT);            //Button Input
  
  //dac power down and then set Vref = Vcc = 5V
  dac.init();

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
}

void loop() {

/*if(digitalRead(button) == 0){

    lcd.setCursor(0, 0);
    lcd.print("Button Pressed");
    delay(1000);
    lcd.clear();

    adcOutput = adc.adcConvert(1, CS2);
    V_OC = ((float ) adcOutput / 4096) * 5;
    Serial.print("V open circuit = ");
    Serial.println(V_OC);
    
    delay(3000);
    
    dac.backlight_set(V_control);            //Write and Update Command and then set Vout = DAC_voltage
    lcd.setCursor(0, 0);
    lcd.print("V=");
    lcd.setCursor(2, 0);
    lcd.print(V);
    lcd.setCursor(7, 0);
    lcd.print("I=");
    lcd.setCursor(9, 0);
    lcd.print(Id);
    lcd.setCursor(0, 1);
    lcd.print("V_OC=");
    lcd.print(5, 1);
    lcd.print(V_OC);
    

    adcOutput = adc.adcConvert(2, CS2);
    //V_load = ((float ) adcOutput / 4096) * 5;
    //if(V_load == V){
      for(int i=0; i<50; i++){
        adcOutput = adc.adcConvert(1, CS2);
        V_batt_table[i] = ((float ) adcOutput / 4096) * 5;
        Serial.println(V_batt_table[i]);
        delay(1);
      }
      lcd.setCursor(9, 1);
      lcd.print("V_b=");
      lcd.setCursor(13, 1);
      lcd.print(V_batt_table[49]);
    //}

}
*/  
    dac.backlight_set(V_control);

    delay(3000);
    

    //dac.backlight_set(BACKLIGHT_FULL_OFF);    //Vout = 0 (maybe not needed)
    //dac.cmd_send(DAC_POWER_DOWN, 0);          //DAC Power Off


    //delay(10000);
}
