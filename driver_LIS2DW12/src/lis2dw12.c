/*
 * max6675.c
 *
 *  Created on: 28 ago. 2021
 *      Author: gonza
 */

#include "max6675.h"



#define convert_raw2temp(rawTemp)	(rawTemp * (MAX_TEMP / MAX_TEMP_DIGITAL_VALUE))

#define isProbeOpen(rawData)	((rawData & PROBE_MASK) ? true : false)

/**
 * Se define una estructura interna al driver que no es visible al usuario.
 * Se utiliza para cargar las funciones definidas especificamente como port para
 * el hardware especifico.
 */
static max6675_t max6675_control;


/**
 * Funcion de inicializacion del driver, toma una estructura externa cargada con los
 * punteros a funciones definidos en el archivo port correspondiente.
 */

/*************************************************************************************************
	 *  @brief Inicializacion del driver MAX6675
     *
     *  @details
     *   	Se copian los punteros a funciones pasados por argumentos a la estructura interna
     *   	del driver, la cual no puede ser accedida por el resto del programa
     *
	 *  @param		config	Estructura de configuracion para el driver.
	 *  @return     None.
	 *  @warning	En esta version no hay asser alguno sobre si los punteros tienen un valor
	 *  			distinto de NULL.
***************************************************************************************************/
void max6675_InitDriver(max6675_t config)  {
	max6675_control.chip_select_ctrl = config.chip_select_ctrl;
	max6675_control.spi_read_fnc = config.spi_read_fnc;
	max6675_control.delay_1ms_func = config.delay_1ms_func;

	/**
	 * La primer llamada a chip select control hace que el dispositivo descarte cualquier
	 * dato que tuviese y lo mantiene en hold
	 */
	max6675_control.chip_select_ctrl(CS_DISABLE);
	max6675_control.delay_1ms_func(2);
}


/*************************************************************************************************
	 *  @brief Obtencion de temperatura actual
     *
     *  @details
     *   	Se obtiene la temperatura actual de la sonda leyendo los datos del IC MAX6675
     *   	por medio del puerto SPI
     *
	 *  @param		None.
	 *  @return     El valor de la temperatura en ºC.
	 *  @warning	No puede llamarse esta funcion sin antes inicializar el driver.
	 *  @see 		max6675_InitDriver
***************************************************************************************************/
float max6675_GetTemp(void)  {
	uint16_t raw_data;
	float temp;

	/**
	 * Inicio de una nueva lectura, ponemos CS en LOW (ver hoja de datos)
	 */
	max6675_control.chip_select_ctrl(CS_ENABLE);
	max6675_control.delay_1ms_func(2);
	/**
	 * Leemos el dato crudo desde el dispositivo y terminamos la transaccion
	 */
	raw_data = max6675_control.spi_read_fnc();

	max6675_control.delay_1ms_func(2);
	max6675_control.chip_select_ctrl(CS_DISABLE);

	/**
	 * Se hace un bit shift hacia la derecha segun la hoja de datos
	 */
	raw_data = raw_data >> 3;

	/**
	 * Aplicamos la conversion correspondiente segun la hoja de datos y retornamos
	 */
	temp = convert_raw2temp(raw_data);

	return temp;
}

/*************************************************************************************************
	 *  @brief Determina si la sonda esta desconectada o funcionando de forma inapropiada
     *
     *  @details
     *   	Haciendo uso del bit D2 en la lectura de los datos crudos desde el MAX6675 se
     *   	puede determinar si la sonda esta presente o abierta.
     *
	 *  @param		None.
	 *  @return     true si la sonda esta abierta, false en caso contrario.
	 *  @warning	No puede llamarse esta funcion sin antes inicializar el driver.
	 *  @see 		max6675_InitDriver
***************************************************************************************************/
bool max6675_isProbeOpen(void)  {
	bool ret;
	uint16_t raw_data;

	/**
	 * Inicio de una nueva lectura, ponemos CS en LOW (ver hoja de datos)
	 */
	max6675_control.chip_select_ctrl(CS_ENABLE);

	max6675_control.delay_1ms_func(2);

	/**
	 * Leemos el dato crudo desde el dispositivo y terminamos la transaccion
	 */
	raw_data = max6675_control.spi_read_fnc();

	max6675_control.delay_1ms_func(2);
	max6675_control.chip_select_ctrl(CS_DISABLE);


	ret = isProbeOpen(raw_data);

	return ret;
}
