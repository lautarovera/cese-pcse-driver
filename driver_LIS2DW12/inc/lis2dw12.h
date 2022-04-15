/*
 * max6675.h
 *
 *  Created on: 28 ago. 2021
 *      Author: gonza
 */

#ifndef PCSE_DRIVER_MAX6675_INC_MAX6675_H_
#define PCSE_DRIVER_MAX6675_INC_MAX6675_H_


#include <stdbool.h>
#include <stdint.h>

#define MAX_TEMP 				1023.75		// salida si hay un dato de 0x0FFF (maximo valor en 12-bits)
#define MIN_TEMP 				0
#define MAX_TEMP_DIGITAL_VALUE	0x0FFF
#define PROBE_MASK				0x0004		//Mascara para determinar si la sonda esta presente


enum _csState {CS_ENABLE=0,CS_DISABLE};
typedef enum _csState csState_t;

/**
 * typedefs para simplificar la lectura, puntero a funcion
 */
typedef void (*csFunction_t)(csState_t);
typedef uint16_t (*spiRead_t)(void);
typedef void (*delay1ms_t)(int32_t);


/**
 * Estructura simple con punteros a funciones para separar la capa mas baja del driver
 */

struct _max6675  {
	csFunction_t chip_select_ctrl;
	spiRead_t spi_read_fnc;
	delay1ms_t delay_1ms_func;

};

typedef struct _max6675 max6675_t;


/**
 * prototipos de funciones
 */
void max6675_InitDriver(max6675_t config);
float max6675_GetTemp(void);
bool max6675_isProbeOpen(void);

#endif /* PCSE_DRIVER_MAX6675_INC_MAX6675_H_ */
