/**************************************************************************************************
 * @file
 * @brief  LIS2DW12 Driver - I2C Port for STM32F429xx
 **************************************************************************************************
 * @author Lautaro Vera
 **************************************************************************************************/

/* Includes --------------------------------------------------------------------------------------*/

#include "stm32f4xx_hal.h"
#include "lis2dw12_cfg.h"

/* Private includes ------------------------------------------------------------------------------*/

#include "lis2dw12_stm32f429xx_port.h"
#include <stdio.h>

/* Private typedef -------------------------------------------------------------------------------*/

/* Private define --------------------------------------------------------------------------------*/

/* Private macro ---------------------------------------------------------------------------------*/

/* Private variables -----------------------------------------------------------------------------*/
I2C_HandleTypeDef hi2c;

/* Private prototypes ----------------------------------------------------------------------------*/
static void error_handler(void);

/* Functions definitions -------------------------------------------------------------------------*/

static void error_handler(void)
{
  for(;;);
}

void stm32f429xx_i2c_init(void)
{
	hi2c.Instance = I2C_INSTANCE;
	hi2c.Init.ClockSpeed = I2C_MODE;
	hi2c.Init.DutyCycle = I2C_DUTYCYCLE_2;
	hi2c.Init.OwnAddress1 = 0;
	hi2c.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c.Init.OwnAddress2 = 0;
	hi2c.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(&hi2c) != HAL_OK)
	{
		error_handler();
	}

	/** Configure Analogue filter
	*/
	if (HAL_I2CEx_ConfigAnalogFilter(&hi2c, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
	{
		error_handler();
	}

	/** Configure Digital filter
	*/
	if (HAL_I2CEx_ConfigDigitalFilter(&hi2c, 0) != HAL_OK)
	{
		error_handler();
	}
}

i2c_status_t stm32f429xx_i2c_master_tx(uint16_t dev_address, uint8_t *p_data, uint16_t size)
{
  HAL_StatusTypeDef hal_res = HAL_ERROR;
  i2c_status_t i2c_res = I2C_OK;

  hal_res = HAL_I2C_Master_Transmit(&hi2c, dev_address, p_data, size, I2C_TIMEOUT);

  if (HAL_OK != hal_res) {
    i2c_res = I2C_NOK;
  }

  return i2c_res;
}

i2c_status_t stm32f429xx_i2c_master_rx(uint16_t dev_address, uint8_t *p_data, uint16_t size)
{
  HAL_StatusTypeDef hal_res = HAL_ERROR;
  i2c_status_t i2c_res = I2C_OK;

  hal_res = HAL_I2C_Master_Receive(&hi2c, dev_address, p_data, size, I2C_TIMEOUT);

  if (HAL_OK != hal_res) {
	  i2c_res = I2C_NOK;
  }

  return i2c_res;
}

i2c_status_t stm32f429xx_i2c_mem_write(uint16_t dev_address, uint16_t mem_address, uint16_t mem_address_size, uint8_t *p_data, uint16_t size)
{
  HAL_StatusTypeDef hal_res = HAL_ERROR;
  i2c_status_t i2c_res = I2C_OK;

  hal_res = HAL_I2C_Mem_Write(&hi2c, dev_address, mem_address, mem_address_size, p_data, size, I2C_TIMEOUT);

  if (HAL_OK != hal_res) {
    i2c_res = I2C_NOK;
  }

  return i2c_res;
}

i2c_status_t stm32f429xx_i2c_mem_read(uint16_t dev_address, uint16_t mem_address, uint16_t mem_address_size, uint8_t *p_data, uint16_t size)
{
  HAL_StatusTypeDef hal_res = HAL_ERROR;
  i2c_status_t i2c_res = I2C_OK;

  hal_res = HAL_I2C_Mem_Read(&hi2c, dev_address, mem_address, mem_address_size, p_data, size, I2C_TIMEOUT);

  if (HAL_OK != hal_res) {
    i2c_res = I2C_NOK;
  }

  return i2c_res;
}

i2c_status_t stm32f429xx_i2c_is_dev_ready(uint16_t dev_address, uint32_t trials)
{
  HAL_StatusTypeDef hal_res = HAL_ERROR;
  i2c_status_t i2c_res = I2C_OK;

  hal_res = HAL_I2C_IsDeviceReady(&hi2c, dev_address, trials, I2C_TIMEOUT);

  if (HAL_OK == hal_res) {
    i2c_res = I2C_NOK;
  }

  return i2c_res;
}
