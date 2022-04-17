/**************************************************************************************************
 * @file   : lis2dw12_stm32f429xx_port.h
 * @brief  : LIS2DW12 Driver - I2C Port for STM32F429xx
 **************************************************************************************************
 * @author : Lautaro Vera
 **************************************************************************************************/

#ifndef LIS2DW12_STM32F429XX_PORT_H_
#define LIS2DW12_STM32F429XX_PORT_H_

/* Includes --------------------------------------------------------------------------------------*/

#include <stdint.h>

/* Public typedef --------------------------------------------------------------------------------*/

typedef enum {
  I2C_OK,
  I2C_NOK,
  I2C_BUSY
} stm32f429xx_i2c_status_t;

/* Public define ---------------------------------------------------------------------------------*/

/* Public macro ----------------------------------------------------------------------------------*/

/* Public variables ------------------------------------------------------------------------------*/

/* Functions prototypes --------------------------------------------------------------------------*/

/**************************************************************************************************
 *  @brief Funcion para set/reset del chip enable para lis2dw12
 *
 *  @details    Esta funcion es especifica para el hardware utilizado. 
 *              Notar que se basa en una capa HAL existente (LPC Open)
 *
 *  @param      estado  Determina la accion a ser tomada con el pin CS.
 *  @return     None.
 **************************************************************************************************/
void stm32f429xx_i2c_init(void);

/**************************************************************************************************
 *  @brief Funcion para set/reset del chip enable para lis2dw12
 *
 *  @details    Esta funcion es especifica para el hardware utilizado. 
 *              Notar que se basa en una capa HAL existente (LPC Open)
 *
 *  @param      estado  Determina la accion a ser tomada con el pin CS.
 *  @return     None.
 **************************************************************************************************/
stm32f429xx_i2c_status_t stm32f429xx_i2c_master_tx(uint16_t dev_address, uint8_t *p_data, uint16_t size);

/**************************************************************************************************
 *  @brief Funcion para set/reset del chip enable para lis2dw12
 *
 *  @details    Esta funcion es especifica para el hardware utilizado. 
 *              Notar que se basa en una capa HAL existente (LPC Open)
 *
 *  @param      estado  Determina la accion a ser tomada con el pin CS.
 *  @return     None.
 **************************************************************************************************/
stm32f429xx_i2c_status_t stm32f429xx_i2c_master_rx(uint16_t dev_address, uint8_t *p_data, uint16_t size);

/**************************************************************************************************
 *  @brief Funcion para set/reset del chip enable para lis2dw12
 *
 *  @details    Esta funcion es especifica para el hardware utilizado. 
 *              Notar que se basa en una capa HAL existente (LPC Open)
 *
 *  @param      estado  Determina la accion a ser tomada con el pin CS.
 *  @return     None.
 **************************************************************************************************/
stm32f429xx_i2c_status_t stm32f429xx_i2c_mem_write(uint16_t dev_address, uint16_t mem_address, uint16_t mem_address_size, uint8_t *p_data, uint16_t size);

/**************************************************************************************************
 *  @brief Funcion para set/reset del chip enable para lis2dw12
 *
 *  @details    Esta funcion es especifica para el hardware utilizado. 
 *              Notar que se basa en una capa HAL existente (LPC Open)
 *
 *  @param      estado  Determina la accion a ser tomada con el pin CS.
 *  @return     None.
 **************************************************************************************************/
stm32f429xx_i2c_status_t stm32f429xx_i2c_mem_read(uint16_t dev_address, uint16_t mem_address, uint16_t mem_address_size, uint8_t *p_data, uint16_t size);

/**************************************************************************************************
 *  @brief Funcion para set/reset del chip enable para lis2dw12
 *
 *  @details    Esta funcion es especifica para el hardware utilizado. 
 *              Notar que se basa en una capa HAL existente (LPC Open)
 *
 *  @param      estado  Determina la accion a ser tomada con el pin CS.
 *  @return     None.
 **************************************************************************************************/
stm32f429xx_i2c_status_t stm32f429xx_i2c_is_dev_ready(uint16_t dev_address, uint32_t trials);

#endif /* LIS2DW12_STM32F429XX_PORT_H_ */
