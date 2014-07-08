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

}

void loop() {
   showDisplay();
}

void showDisplay(){

   display.setCursor(-10,0);
   display.print(mfc->getPresure());

   display.setCursor(CENTR_X,0);
   display.print(mfc->getTemp());

   display.setCursor(0,CENTR_Y);
   display.print(mfc->getSetPoint());

   display.setCursor(CENTR_X,CENTR_Y);
   display.print(mfc->getGasName());

   display.setCursor(CENTR_X*2-10,0);
   display.print(mfc->getVoluFlowRate());

   display.setCursor(CENTR_X*2-10,CENTR_Y);
   display.print(mfc->getMassFlowRate());

   display.display();
   delay(200);
   display.clearDisplay();   // clears the screen and buffer
}
