/**************************************************************************************************
 * @file   : lis2dw12_cfg.h
 * @brief  : LIS2DW12 Driver Configuration
 **************************************************************************************************
 * @author : Lautaro Vera
 **************************************************************************************************/

#ifndef LIS2DW12_CFG_H_
#define LIS2DW12_CFG_H_

/* Includes --------------------------------------------------------------------------------------*/

/* Public typedef --------------------------------------------------------------------------------*/

/* Public define ---------------------------------------------------------------------------------*/
#ifdef STM32F429xx
/* Select the between the three possibles instances of I2C: I2C1, I2C2 or I2C3 */
#define I2C_INSTANCE    I2C1
#define I2C_TIMEOUT     (10u)
#endif

#define STANDARD_MODE	100000u
#define FAST_MODE		400000u
/* Select the I2C mode: STANDARD (100KHz) or FAST (400KHz) */
#define I2C_MODE        STANDARD_MODE
/* Select the voltage on SA0 pin in the LIS2DW12, which selects   */
/* the final I2C address of the device: HIGH (0x19) or LOW (0x18) */
#define LIS2DW12_SA0    1u

/* Public macro ----------------------------------------------------------------------------------*/

/* Public variables ------------------------------------------------------------------------------*/

/* Functions prototypes --------------------------------------------------------------------------*/

#endif /* LIS2DW12_CFG_H_ */
