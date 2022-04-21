/**************************************************************************************************
 * @file
 * @brief  LIS2DW12 Driver - I2C Port for STM32F429xx
 **************************************************************************************************
 * @author Lautaro Vera
 **************************************************************************************************/

#ifndef LIS2DW12_STM32F429XX_PORT_H_
#define LIS2DW12_STM32F429XX_PORT_H_

/* Includes --------------------------------------------------------------------------------------*/

#include <stdint.h>

/* Public typedef --------------------------------------------------------------------------------*/

typedef enum {
  I2C_NOK,
  I2C_OK,
} i2c_status_t;

/* Public define ---------------------------------------------------------------------------------*/

/* Public macro ----------------------------------------------------------------------------------*/

/* Public variables ------------------------------------------------------------------------------*/

/* Functions prototypes --------------------------------------------------------------------------*/
/**************************************************************************************************
 *  @brief Funcion para la inicializacion de la HAL I2C de la plataforma STM32F429XX.
 *
 *  @details    Esta funcion es especifica para el hardware utilizado. 
 *              Notar que se basa en una capa HAL existente.
 *              El modo I2C (Standard o Fast) debe definirse en lis2dw12_cfg.h (en /inc).
 *              El handler de la HAL I2C (I2C1, I2C2, I2C3) debe definirse en lis2dw12_cfg.h (en /inc).
 *
 *  @see        lis2dw12_cfg.h
 *  @return     None.
 **************************************************************************************************/
void stm32f429xx_i2c_init(void);

/**************************************************************************************************
 *  @brief Funcion para la transmision del master I2C.
 *
 *  @details    Esta funcion es especifica para el hardware utilizado. 
 *              Notar que se basa en una capa HAL existente.
 *
 *  @param[in]  dev_address   Direccion del esclavo (SAD).
 *  @param[in]  p_data        Puntero al bufer de datos que se quiere transmitir.
 *  @param[in]  size          Tamaño del bufer de datos.
 *  @return     I2C_OK si la transferencia fue exitosa, de lo contrario I2C_NOK.
 **************************************************************************************************/
i2c_status_t stm32f429xx_i2c_master_tx(uint16_t dev_address, uint8_t *p_data, uint16_t size);

/**************************************************************************************************
 *  @brief Funcion para la recepcion del master I2C.
 *
 *  @details    Esta funcion es especifica para el hardware utilizado. 
 *              Notar que se basa en una capa HAL existente.
 *
 *  @param[in]  dev_address   Direccion del esclavo (SAD).
 *  @param[out] p_data        Puntero al bufer de datos que alojara los datos a recibir.
 *  @param[in]  size          Tamaño del bufer de datos, igual al numero de bytes a recibir.
 *  @return     I2C_OK si la transferencia fue exitosa, de lo contrario I2C_NOK.
 **************************************************************************************************/
i2c_status_t stm32f429xx_i2c_master_rx(uint16_t dev_address, uint8_t *p_data, uint16_t size);

/**************************************************************************************************
 *  @brief Funcion para la escritura de una sub-direccion del esclavo I2C.
 *
 *  @details    Esta funcion es especifica para el hardware utilizado. 
 *              Notar que se basa en una capa HAL existente.
 *
 *  @param[in]  dev_address       Direccion del esclavo (SAD).
 *  @param[in]  mem_address       Sub-direccion del esclavo (SUB).
 *  @param[in]  mem_address_size  Tamaño de la sub-direccion en bytes.
 *  @param[in]  p_data            Puntero al bufer de datos que se quiere escribir.
 *  @param[in]  size              Tamaño del bufer de datos, igual al numero de bytes a escribir.
 *  @return     I2C_OK si la transferencia fue exitosa, de lo contrario I2C_NOK.
 **************************************************************************************************/
i2c_status_t stm32f429xx_i2c_mem_write(uint16_t dev_address, uint16_t mem_address, uint16_t mem_address_size, uint8_t *p_data, uint16_t size);

/**************************************************************************************************
 *  @brief Funcion para la lectura de una sub-direccion del esclavo I2C.
 *
 *  @details    Esta funcion es especifica para el hardware utilizado. 
 *              Notar que se basa en una capa HAL existente.
 *
 *  @param[in]  dev_address       Direccion del esclavo (SAD).
 *  @param[in]  mem_address       Sub-direccion del esclavo (SUB).
 *  @param[in]  mem_address_size  Tamaño de la sub-direccion en bytes.
 *  @param[out] p_data            Puntero al bufer de datos que alojara los datos leidos.
 *  @param[in]  size              Tamaño del bufer de datos, igual al numero de bytes a leer.
 *  @return     I2C_OK si la transferencia fue exitosa, de lo contrario I2C_NOK.
 **************************************************************************************************/
i2c_status_t stm32f429xx_i2c_mem_read(uint16_t dev_address, uint16_t mem_address, uint16_t mem_address_size, uint8_t *p_data, uint16_t size);

/**************************************************************************************************
 *  @brief Funcion para la encuesta si un dispositivo esta listo en el bus I2C.
 *
 *  @details    Esta funcion es especifica para el hardware utilizado. 
 *              Notar que se basa en una capa HAL existente.
 *
 *  @param[in]  dev_address   Direccion del esclavo (SAD).
 *  @param[in]  trials        Numero de intentos.
 *  @return     I2C_OK si la transferencia fue exitosa, de lo contrario I2C_NOK.
 **************************************************************************************************/
i2c_status_t stm32f429xx_i2c_is_dev_ready(uint16_t dev_address, uint32_t trials);

#endif /* LIS2DW12_STM32F429XX_PORT_H_ */
