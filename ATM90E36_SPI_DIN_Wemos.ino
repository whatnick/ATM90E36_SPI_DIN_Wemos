/* ATM90E36 Energy Monitor Demo Application

   The MIT License (MIT)

  Copyright (c) 2016 whatnick and Ryzee

  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include <SPI.h>
#include <ATM90E36.h>     //SPI Metering Chip - https://github.com/whatnick/ATM90E36_Arduino
#include <U8g2lib.h>      //OLED Driver
#include <Wire.h>

//Setup OLED
U8G2_SSD1306_64X48_ER_F_HW_I2C u8g2(U8G2_R0); // EastRising 0.66" OLED breakout board, Uno: A4=SDA, A5=SCL, 5V powered
long curMillis, prevMillis;

ATM90E36 eic1(D3);
ATM90E36 eic2(D8);

void setup() {
  /* Initialize the serial port to host */
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }
  u8g2.begin();
  u8g2.setFont(u8g2_font_5x8_tr);
  
  Serial.println("Start ATM90E36");
  /*Initialise the ATM90E36 + SPI port */
  eic1.begin();
  delay(1000);
  eic2.begin();
  delay(1000);
}



void loop() {
  
  /*Repeatedly fetch some values from the ATM90E36 */
  curMillis = millis();
  yield();
  if ((curMillis - prevMillis) > 500)
  {
    u8g2.clearDisplay();
  }
  if ((curMillis - prevMillis) > 1000)
  {
    readMeterCurrentDisplay();
    delay(500);
    readMeterVoltageDisplay();
    delay(500);
  }
}

void readMeterVoltageDisplay()
{
  double voltageA1,voltageB1,voltageC1;
  double voltageA2,voltageB2,voltageC2;
  const double scale = 0.5149;
  voltageA1=eic1.GetLineVoltageA()*scale;
  voltageB1=eic1.GetLineVoltageB()*scale;
  voltageC1=eic1.GetLineVoltageC()*scale;

  voltageA2=eic2.GetLineVoltageA()*scale;
  voltageB2=eic2.GetLineVoltageB()*scale;
  voltageC2=eic2.GetLineVoltageC()*scale;

  u8g2.clearDisplay();
  u8g2.firstPage();
  do {
    u8g2.drawStr(0, 9, "VA:");
    u8g2.setCursor(14, 9);
    u8g2.print(voltageA1, 0);
    u8g2.setCursor(40, 9);
    u8g2.print(voltageA2, 0);

    u8g2.drawStr(0, 18, "VB:");
    u8g2.setCursor(14, 18);
    u8g2.print(voltageB1, 0);
    u8g2.setCursor(40, 18);
    u8g2.print(voltageB2, 0);

    u8g2.drawStr(0, 27, "VC:");
    u8g2.setCursor(14, 27);
    u8g2.print(voltageC1, 0);
    u8g2.setCursor(40, 27);
    u8g2.print(voltageC2, 0);

  } while ( u8g2.nextPage() );
  prevMillis = curMillis;
}

void readMeterCurrentDisplay()
{
  double currentA1,currentB1,currentC1;
  double currentA2,currentB2,currentC2;
  const double scale = 2.5;
  currentA1=eic1.GetLineCurrentA()*scale;
  currentB1=eic1.GetLineCurrentB()*scale;
  currentC1=eic1.GetLineCurrentC()*scale;

  currentA2=eic2.GetLineCurrentA()*scale;
  currentB2=eic2.GetLineCurrentB()*scale;
  currentC2=eic2.GetLineCurrentC()*scale;

  u8g2.clearDisplay();
  u8g2.firstPage();
  do {
    u8g2.drawStr(0, 9, "IA:");
    u8g2.setCursor(14, 9);
    u8g2.print(currentA1, 2);
    u8g2.setCursor(40, 9);
    u8g2.print(currentA2, 2);

    u8g2.drawStr(0, 18, "IB:");
    u8g2.setCursor(14, 18);
    u8g2.print(currentB1, 2);
    u8g2.setCursor(40, 18);
    u8g2.print(currentB2, 2);

    u8g2.drawStr(0, 27, "IC:");
    u8g2.setCursor(14, 27);
    u8g2.print(currentC1, 2);
    u8g2.setCursor(40, 27);
    u8g2.print(currentC2, 2);

  } while ( u8g2.nextPage() );
}
