#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include "massflowcontroller.h"

// Display pins:
// pin 9 - LED ground (PWM)
#define DISP_LED 9
// pin 3 - Power on
#define DISP_VCC 3
// Software SPI (slower updates, more flexible pin options):
// pin 4 - Serial clock out (SCLK)
#define DISP_CLS 4

// pin 5 - Serial data out (DIN)
#define DISP_DIN 5

// pin 6 - Data/Command select (D/C)
#define DISP_DC 6

// pin 7 - LCD chip select (CS) CE
#define DISP_CE 7

// pin 8 - LCD reset (RST)
#define DISP_RST 8

Adafruit_PCD8544 display = Adafruit_PCD8544(
DISP_CLS, DISP_DIN, DISP_DC, DISP_CE, DISP_RST);

int const CENTR_X = (display.width() / 2);
int const CENTR_Y = (display.height() / 2);

// pin A3 - Valve POS
#define VALVE_POS A3

// pin 13 - Buildin LED
#define LED 13

MassFlowController * mfc;

void setup()   {  
  Serial.begin(9600);  
  pinMode(DISP_VCC, OUTPUT);
  digitalWrite(DISP_VCC, HIGH);
  pinMode(DISP_LED, OUTPUT);
  analogWrite(DISP_LED, 250);
  delay(22);
  
  display.begin();
  display.setContrast(25);
  display.display(); // show splashscreen
  delay(222);
  display.clearDisplay();   // clears the screen and buffer
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0,0);
  
  mfc = new MassFlowController;
  mfc->setDeviceId('C');
  mfc->setGasNumber(5);
  mfc->setPresure("14.21", 6);
  mfc->setSetPoint("3.04",5);
  mfc->setTemp("30.02", 5);
}

void loop() {
   readInput();
   showDisplay();
   showOutput();
}

void showDisplay(){

   display.setCursor(0,0);
   display.print("Pres ");
   display.print(mfc->getPresure());
   display.print(" ");
   display.print(mfc->getDeviceId());

   display.setCursor(0,10);
   display.print("SP ");
   display.print(mfc->getSetPoint());

   display.setCursor(CENTR_X+15,10);
   display.print(mfc->getGasName());

   display.setCursor(0,20);
   display.print("VFR ");
   display.print(mfc->getVoluFlowRate());

   display.setCursor(0,30);
   display.print("MFR ");
   display.print(mfc->getMassFlowRate());

   display.setCursor(0,40);
   display.print("Temerat");
   display.print(mfc->getTemp());
   display.display();
   delay(200);
   display.clearDisplay();   // clears the screen and buffer
}

void readInput(){
   int rawInputValue;
   rawInputValue = analogRead(VALVE_POS);
   unsigned int valvePos = map(rawInputValue, 0, 1023, 0, 100);
   mfc->setValvePos(valvePos);
}

void showOutput(){
   digitalWrite(LED, LOW);
   char setPoint = mfc->getSetPoint()[0];
   char volumePoint = mfc->getVoluFlowRate()[2];

   if (setPoint == volumePoint){
      digitalWrite(LED, HIGH);
   }

}
