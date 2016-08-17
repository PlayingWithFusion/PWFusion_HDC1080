/***************************************************************************
* File Name: PWFusion_HDC1080.cpp
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
* Author		Date		Comments
* J. Steinlage	2016Feb		Original version
* 
* Playing With Fusion, Inc. invests time and resources developing open-source
* code. Please support Playing With Fusion and continued open-source 
* development by buying products from Playing With Fusion!
*
* **************************************************************************
* ADDITIONAL NOTES:
* This file contains functions to interface with the HDC1080 Humidity and
* temperature sensor from Texas Instruments. Tested on the Arduino Uno R3.
***************************************************************************/
#include "PWFusion_HDC1080.h"

PWF_HDC1080::PWF_HDC1080(uint8_t DevAdd)
{
	// address is hard coded to x40 for the HDC1080 (same as 1050)
	_devadd = 0x40;

}

void PWF_HDC1080::HDC1080_Config(uint8_t temp_res, uint8_t hum_res, uint8_t heater_en, struct var_HDC1080 *humsns_ptr)
{
	uint16_t dev_ser_high = 0;
	uint32_t dev_ser_low = 0;
	uint16_t temp_0, temp_1;
	uint8_t data_array[4] = {0,0,0,0};
	uint8_t write_array[2] = {0,0};
	
	// start by verifying it is, indeed, an HDC1080
	I2c.read(_devadd, 0xFF, 2, data_array); // read 0xFF (device ID), store result in data_array
	temp_0 = (data_array[0] << 8) + data_array[1]; // temp_0 is the device ID
	I2c.read(_devadd, 0xFE, 2, data_array); // read 0xFE (manuf ID), store result in data_array
	temp_1 = (data_array[0] << 8) + data_array[1]; // temp_1 is Manuf ID
	
	if(((uint16_t)0x1050 == temp_0) && ((uint16_t)0x5449 == temp_1))
	{
		humsns_ptr->IC_verified = 1;
		Serial.println("HDC1080 found, beginning setup...");
		
		// read device serial number. Result must be stored in 2 vars as
		// print can't handle 64-bit numbers
		// read bits 40:25
		I2c.read(_devadd, 0xFB, 2, data_array);
		// save high 9 bits in high serial number variable
		dev_ser_high = (data_array[0]<<8) + data_array[1];
		dev_ser_high>>=7;                  // get rid of lower 7 (stored in low)
		// start packing the rest of the serial number into the lower SN variable
		dev_ser_low = data_array[1] & 0x7F; // keep only the lower 7 bits
		dev_ser_low<<=16; // prepare to load next 16 bits in the variable
		// read bits 24:9
		I2c.read(_devadd, 0xFC, 2, data_array);
		dev_ser_low = dev_ser_low + ((uint16_t)data_array[0]<<8) + data_array[1];
		// read bits 8:0
		I2c.read(_devadd, 0xFD, 2, data_array);
		dev_ser_low<<=9;
		temp_0 = (uint16_t)(data_array[0]<<8) + data_array[1];
		temp_0>>=7;
		dev_ser_low |= temp_0;
		// print serial number to UART
		Serial.print("Device Serial Number = x");
		Serial.print(dev_ser_high, HEX);
		Serial.println(dev_ser_low, HEX);
		
		// read config reg, determine battery status
		I2c.read(_devadd, 0x02, 2, data_array);
		humsns_ptr->batt_stat = (data_array[0] & 0x08)>>3;
		if(0 == humsns_ptr->batt_stat){Serial.println("System voltage OK");}
		else{Serial.println("System voltage low");}
		
		// write configuration, done as two bytes to a single register (0x02)
		write_array[0] = 0x10 + (0x20 & heater_en) + (0x04 & temp_res) + (0x03 & hum_res);
		write_array[1] = 0x00;
		// now write two bytes to config register
		I2c.write(_devadd, 0x02, write_array, 2);
		Serial.println("HDC1080 config successful!");
	}
	else
	{
		Serial.println("HDC1080 not found. Check wiring");
		return;
	}
}

void PWF_HDC1080::HDC1080_Read(uint8_t heater_en, struct var_HDC1080 *humsns_ptr)
{
	uint8_t data_array[4] = {0,0,0,0};
	uint8_t write_array[2] = {0,0};	
	uint8_t I2C_read_fail = 0;
	
	// start by reading read config reg, determine battery status
	I2c.read(_devadd, 0x02, 2, data_array);
	
	// handle battery status
	humsns_ptr->batt_stat = (data_array[0] & 0x08)>>3;
	
	// change heater bit if needed, start with enable check
	if((HEATER_EN == heater_en) && (0x00 == (data_array[0] & 0x20))) // heater disabled, want it enabled
	{
		write_array[0] = (data_array[0] & 0xDF) + HEATER_EN;
		write_array[1] = 0x00;
		// now write two bytes to config register
		I2c.write(_devadd, 0x02, write_array, 2);
	}
	else if((HEATER_DIS == heater_en) && (0x20 == (data_array[0] & 0x20))) // heater enabled, want it disabled
	{
		write_array[0] = (data_array[0] & 0xDF);
		write_array[1] = 0x00;
		// now write two bytes to config register
		I2c.write(_devadd, 0x02, write_array, 2);
	}
	
	// start conversion, wait 7ms then read temp/hum regs
	I2c.write((uint8_t)_devadd, (uint8_t)0x00); 	// pointer write to HDC1080
	delay(40);
	// now read the temp and hum registers
	I2C_read_fail = I2c.read(_devadd, 4, data_array);
	if(0 != I2C_read_fail){Serial.println("I2C read failure...may need to increase delay between trigger and read");}
	humsns_ptr->temp_raw = ((uint16_t)data_array[0] << 8) | data_array[1];
	humsns_ptr->relhum_raw = ((uint16_t)data_array[2] << 8) | data_array[3];
	return;
	
	// raw data conversion equations: 
	// humidity conv: raw register / 2^16 * 100% RH
	// temperature conv: raw reg / 2^16 *165C - 40C
}
