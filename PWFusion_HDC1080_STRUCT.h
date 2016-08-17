/***************************************************************************
* File Name: PWFusion_HDC1080.h
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

#ifndef PWF_HDC1080_STRUCT_H
#define PWF_HDC1080_STRUCT_H

#include "avr/pgmspace.h"


struct var_HDC1080{
	uint8_t		IC_verified;	// 0=not found, 1=IC found
    uint16_t	relhum_raw;		// relative humidity, 1/2^16 RH (%) per bit
    uint16_t  	temp_raw;		// temp of chip, T = 1/2^14 *165C - 40C
    uint8_t		batt_stat;		// battery status - valid/invalid + fault reason
};


#endif /* PWF_HDC1080_STRUCT_H */
