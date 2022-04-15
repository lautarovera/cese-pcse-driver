/*
 * max6675_CIAA_port.h
 *
 *  Created on: 28 ago. 2021
 *      Author: gonza
 */

#ifndef PCSE_DRIVER_MAX6675_INC_MAX6675_CIAA_PORT_H_
#define PCSE_DRIVER_MAX6675_INC_MAX6675_CIAA_PORT_H_

#include "max6675.h"
#include "chip.h"
#include "sapi.h"



void chipSelect_CIAA_port(csState_t estado);
uint16_t spiRead_CIAA_port(void);
void delay_CIAA_port(int32_t millisecs);



#endif /* PCSE_DRIVER_MAX6675_INC_MAX6675_CIAA_PORT_H_ */
