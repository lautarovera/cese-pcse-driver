/*
 * max6675_CIAA_port.c
 *
 *  Created on: 28 ago. 2021
 *      Author: gonza
 */

#include "max6675_CIAA_port.h"

/*************************************************************************************************
	 *  @brief Funcion para set/reset del chip enable para MAX6675
     *
     *  @details
     *   	Esta funcion es especifica para el hardware utilizado. Notar que se basa en una capa
     *   	HAL existente (LPC Open)
     *
	 *  @param		estado	Determina la accion a ser tomada con el pin CS.
	 *  @return     None.
***************************************************************************************************/
void chipSelect_CIAA_port(csState_t estado)  {

	switch(estado)  {

	case CS_ENABLE:
		Chip_GPIO_SetPinOutLow(LPC_GPIO_PORT,3,0);
		break;

	case CS_DISABLE:
		Chip_GPIO_SetPinOutHigh(LPC_GPIO_PORT,3,0);
		break;

	default:
		;
	}
}

/*************************************************************************************************
	 *  @brief Funcion para leer un dato mediante SPI del chip enable para MAX6675
     *
     *  @details
     *   	Esta funcion es especifica para el hardware utilizado. Notar que se basa en una capa
     *   	HAL existente (LPC Open)
     *
	 *  @param		None.
	 *  @return     El dato leido desde el chip MAX6675 en crudo, ver hoja de datos.
***************************************************************************************************/
uint16_t spiRead_CIAA_port(void)  {
	Chip_SSP_DATA_SETUP_T xferConfig;
	uint8_t buffer[2];
	uint16_t ret;
	bool res;

	xferConfig.tx_data = NULL;
	xferConfig.tx_cnt  = 0;
	xferConfig.rx_data = buffer;
	xferConfig.rx_cnt  = 0;
	xferConfig.length  = 2;


	/**
	 * Se utiliza una funcion de lpc open que permite leer un byte
	 * Mediante el parametro lengh se indica que seran 2 bytes a leer
	 */
	res = Chip_SSP_RWFrames_Blocking(LPC_SSP1,  &xferConfig);

	/**
	 * Se debe armar el dato de 16bits entendiendo que el primer byte recibido es
	 * el mas significativo
	 */
	ret = buffer[0] << 8;
	ret |= buffer[1];

	return ret;
}

/*************************************************************************************************
	 *  @brief Funcion que implementa un delay simple de 1 milisegundo
     *
     *  @details
     *   	Esta funcion es especifica para el hardware utilizado. Notar que se basa en una capa
     *   	de software existente (sAPI)
     *
	 *  @param		millisecs	Los milisegundos a efectuar el delay.
	 *  @return     None.
	 *  @see		delay (implementacion de sAPI).
***************************************************************************************************/
void delay_CIAA_port(int32_t millisecs)  {
	delay((tick_t)millisecs);
}
