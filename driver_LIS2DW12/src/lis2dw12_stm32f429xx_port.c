/**************************************************************************************************
 * @file   : lis2dw12_stm32f429xx_port.c
 * @brief  : LIS2DW12 Driver - I2C Port for STM32F429xx
 **************************************************************************************************
 * @author : Lautaro Vera
 **************************************************************************************************/

/* Includes --------------------------------------------------------------------------------------*/

#include "stm32f4xx_hal.h"

/* Private includes ------------------------------------------------------------------------------*/

#include "lis2dw12_stm32f429xx_port.h"

/* Private typedef -------------------------------------------------------------------------------*/

/* Private define --------------------------------------------------------------------------------*/

#define I2C_TIMEOUT   10u

/* Private macro ---------------------------------------------------------------------------------*/

/* Private variables -----------------------------------------------------------------------------*/

I2C_HandleTypeDef hi2c3;

/* Private prototypes ----------------------------------------------------------------------------*/

/* Functions definitions -------------------------------------------------------------------------*/

void stm32f429xx_i2c_init(void)
{
  hi2c3.Instance = I2C3;
  hi2c3.Init.ClockSpeed = 100000u;
  hi2c3.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c3.Init.OwnAddress1 = 0u;
  hi2c3.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c3.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c3.Init.OwnAddress2 = 0u;
  hi2c3.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c3.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c3) != HAL_OK)
  {
    
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c3, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c3, 0) != HAL_OK)
  {
    
  }
}

stm32f429xx_i2c_status_t stm32f429xx_i2c_master_tx(uint16_t dev_address, uint8_t *p_data, uint16_t size)
{
  HAL_StatusTypeDef hal_res = HAL_ERROR;
  stm32f429xx_i2c_status_t i2c_res = I2C_NOK;
  
  hal_res = HAL_I2C_Master_Transmit(&hi2c3, dev_address, p_data, size, I2C_TIMEOUT);

  if (HAL_OK == hal_res) {
    i2c_res = I2C_OK;
  }
  else if (HAL_BUSY == hal_res) {
    i2c_res = I2C_BUSY;
  }
  else {
    /* HAL error */
  }

  return i2c_res;
}

stm32f429xx_i2c_status_t stm32f429xx_i2c_master_rx(uint16_t dev_address, uint8_t *p_data, uint16_t size)
{
  HAL_StatusTypeDef hal_res = HAL_ERROR;
  stm32f429xx_i2c_status_t i2c_res = I2C_NOK;

  hal_res = HAL_I2C_Master_Receive(&hi2c3, dev_address, p_data, size, I2C_TIMEOUT);

  if (HAL_OK == hal_res) {
  i2c_res = I2C_OK;
  }
  else if (HAL_BUSY == hal_res) {
    i2c_res = I2C_BUSY;
  }
  else {
    /* HAL error */
  }

  return i2c_res;
}

stm32f429xx_i2c_status_t stm32f429xx_i2c_mem_write(uint16_t dev_address, uint16_t mem_address, uint16_t mem_address_size, uint8_t *p_data, uint16_t size)
{
  HAL_StatusTypeDef hal_res = HAL_ERROR;
  stm32f429xx_i2c_status_t i2c_res = I2C_NOK;

  hal_res = HAL_I2C_Mem_Write(&hi2c3, dev_address, mem_address, mem_address_size, p_data, size, I2C_TIMEOUT);

  if (HAL_OK == hal_res) {
    i2c_res = I2C_OK;
  }
  else if (HAL_BUSY == hal_res) {
    i2c_res = I2C_BUSY;
  }
  else {
    /* HAL error */
  }

  return i2c_res;
}

stm32f429xx_i2c_status_t stm32f429xx_i2c_mem_read(uint16_t dev_address, uint16_t mem_address, uint16_t mem_address_size, uint8_t *p_data, uint16_t size)
{
  HAL_StatusTypeDef hal_res = HAL_ERROR;
  stm32f429xx_i2c_status_t i2c_res = I2C_NOK;

  hal_res = HAL_I2C_Mem_Read(&hi2c3, dev_address, mem_address, mem_address_size, p_data, size, I2C_TIMEOUT);

  if (HAL_OK == hal_res) {
    i2c_res = I2C_OK;
  }
  else if (HAL_BUSY == hal_res) {
    i2c_res = I2C_BUSY;
  }
  else {
    /* HAL error or timeout */
  }

  return i2c_res;
}

stm32f429xx_i2c_status_t stm32f429xx_i2c_is_dev_ready(uint16_t dev_address, uint32_t trials)
{
  HAL_StatusTypeDef hal_res = HAL_ERROR;
  stm32f429xx_i2c_status_t i2c_res = I2C_NOK;

  hal_res = HAL_I2C_IsDeviceReady(&hi2c3, dev_address, trials, I2C_TIMEOUT);

  if (HAL_OK == hal_res) {
    i2c_res = I2C_OK;
  }
  else if (HAL_BUSY == hal_res) {
    i2c_res = I2C_BUSY;
  }
  else {
    /* HAL error or timeout */
  }

  return i2c_res;
}
