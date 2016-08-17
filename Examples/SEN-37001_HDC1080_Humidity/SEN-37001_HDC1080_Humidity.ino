/***************************************************************************
* File Name: SEN-37001_HDC1080.ino
* Processor/Platform: Arduino Uno R3 (tested)
* Development Environment: Arduino 1.6.5
*
* Designed for use with with Playing With Fusion HDC1080 Humidity
* and Temperature Sensor Breakout: SEN-37001
*
*   SEN-37001 (universal applications)
*   ---> http://www.playingwithfusion.com/productview.php?pdid=79
*
* Copyright © 2016 Playing With Fusion, Inc.
* SOFTWARE LICENSE AGREEMENT: This code is released under the MIT License.
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
* DEALINGS IN THE SOFTWARE.
* **************************************************************************
* REVISION HISTORY:
* Author    Date    Comments
* J. Steinlage  2016Feb   Original version
* 
* Playing With Fusion, Inc. invests time and resources developing open-source
* code. Please support Playing With Fusion and continued open-source 
* development by buying products from Playing With Fusion!
*
* **************************************************************************
* APPLICATION SPECIFIC NOTES (READ THIS!!!):
* - This code configures an Arduino to interface with an HDC1080 from Texas
*   Instruments.
*    - Configure Arduino (I2C, UART, etc)
*    - Perform initial verification and configuration of the HDC1080
*    - The HDC1080 is an I2C-only device
*    - read status/info from sensor
*    - Write formatted information to serial port
* - I2C specific note: This example uses the I2C interface via the I2C lib, not
*   the 'Wire' lib included with the Arduino IDE.
* 
* Circuit:
*    Arduino Uno   Arduino Mega  -->  SEN-37001: HDC1080 Breakout
*    SDA:    SDA        SDA      -->  SDA
*    SCL:    SCL        SCL      -->  SCL
*    GND:    GND        ''       -->  GND
*    5V:     5V         ''       -->  VDD 
*    (note: VDD should match voltage of IO, and can be between 3.3 and 5V)
**************************************************************************/
// The HDC10x0 communicates via I2C. 
// This example uses the I2C interface via the I2C lib, not Wire lib
#include "I2C.h"
// include Playing With Fusion HDC1080 libraries
#include <PWFusion_HDC1080.h>
#include <PWFusion_HDC1080_STRUCT.h>

// declare sensor object (see PWFusion_HDC1080.h file for definitions)
PWF_HDC1080 humsns0(HDC1080_ADD);

// declare sensor structure
struct var_HDC1080 hum_snsr_0;

void setup()
{
  var_HDC1080 *humsns_ptr;
  humsns_ptr = &hum_snsr_0;
  
  Serial.begin(115200);
  Serial.println("Playing With Fusion: HDC1080 Humidity Sensor, SEN-37001");
  
  // setup for the the I2C library: (enable pullups, set speed to 400kHz)
  I2c.begin();
  I2c.pullup(true);
  I2c.setSpeed(1); 

  // give the Arduino time to start up
  delay(1000); 
  
  // setup for the humidity sensor
  // see PWFusion_HDC1080.h file for definitions
  // temperature res / humidity res / enable heater / data structure
  humsns0.HDC1080_Config(TMP_RES_14_BIT, HUM_RES_14_BIT, HEATER_DIS, humsns_ptr);
  

}

void loop()
{
  float temp;
  var_HDC1080 *humsns_ptr;
  humsns_ptr = &hum_snsr_0;
  
  delay(1000);

  humsns0.HDC1080_Read(HEATER_DIS, humsns_ptr);
  Serial.println("HDC1080 CH 0... Temp (C), RelHum (%)");
  Serial.print("               ");
  temp = (float)humsns_ptr->temp_raw;
  Serial.print(temp * 165.0 / 65536.0 - 40.0);
  Serial.print("      ");
  temp = (float)humsns_ptr->relhum_raw;
  Serial.println(temp * 100.0 / 65536.0);

}

