/**************************************************************************************************
 * @file   : lis2dw12.c
 * @brief  : LIS2DW12 Driver
 **************************************************************************************************
 * @author : Lautaro Vera
 **************************************************************************************************/

/* Includes --------------------------------------------------------------------------------------*/
#include "lis2dw12.h"
#include "lis2dw12_cfg.h"
#ifdef STM32F429xx
#include "lis2dw12_stm32f429xx_port.h"
#endif
#include <stdlib.h>

/* Private define --------------------------------------------------------------------------------*/
#if (LIS2DW12_SA0 == 0u)
#define LIS2DW12_SAD_W						0x30u
#define LIS2DW12_SAD_R						0x31u
#elif (LIS2DW12_SA0 == 1u)
#define LIS2DW12_SAD_W						0x32u
#define LIS2DW12_SAD_R						0x33u
#else
#error LIS2DW12 configuration error: SA0 switch have to be 0 or 1
#endif

#define LIS2DW12_REG_SIZE					(sizeof(uint8_t))

/* Register map (8-bit registers) */
#define LIS2DW12_REG_OUT_T_L				0x0Du /* R   */
#define LIS2DW12_REG_OUT_T_H				0x0Eu /* R   */
#define LIS2DW12_REG_WHO_AM_I				0x0Fu /* R   */
#define LIS2DW12_REG_CTRL1					0x20u /* R/W */
#define LIS2DW12_REG_CTRL2					0x21u /* R/W */
#define LIS2DW12_REG_CTRL3					0x22u /* R/W */
#define LIS2DW12_REG_CTRL4_INT1_PAD_CTRL	0x23u /* R/W */
#define LIS2DW12_REG_CTRL5_INT2_PAD_CTRL	0x24u /* R/W */
#define LIS2DW12_REG_CTRL6					0x25u /* R/W */
#define LIS2DW12_REG_OUT_T					0x26u /* R   */
#define LIS2DW12_REG_STATUS					0x27u /* R   */
#define LIS2DW12_REG_OUT_X_L				0x28u /* R   */
#define LIS2DW12_REG_OUT_X_H				0x29u /* R   */
#define LIS2DW12_REG_OUT_Y_L				0x2Au /* R   */
#define LIS2DW12_REG_OUT_Y_H				0x2Bu /* R   */
#define LIS2DW12_REG_OUT_Z_L				0x2Cu /* R   */
#define LIS2DW12_REG_OUT_Z_H				0x2Du /* R   */
#define LIS2DW12_REG_FIFO_CTRL				0x2Eu /* R/W */
#define LIS2DW12_REG_FIFO_SAMPLES			0x2Fu /* R   */
#define LIS2DW12_REG_TAP_THS_X				0x30u /* R/W */
#define LIS2DW12_REG_TAP_THS_Y				0x31u /* R/W */
#define LIS2DW12_REG_TAP_THS_Z				0x32u /* R/W */
#define LIS2DW12_REG_INT_DUR				0x33u /* R/W */
#define LIS2DW12_REG_WAKE_UP_THS			0x34u /* R/W */
#define LIS2DW12_REG_WAKE_UP_DUR			0x35u /* R/W */
#define LIS2DW12_REG_FREE_FALL				0x36u /* R/W */
#define LIS2DW12_REG_STATUS_DUP				0x37u /* R   */
#define LIS2DW12_REG_WAKE_UP_SRC			0x38u /* R   */
#define LIS2DW12_REG_TAP_SRC				0x39u /* R   */
#define LIS2DW12_REG_SIXD_SRC				0x3Au /* R   */
#define LIS2DW12_REG_ALL_INT_SRC			0x3Bu /* R   */
#define LIS2DW12_REG_X_OFS_USR				0x3Cu /* R/W */
#define LIS2DW12_REG_Y_OFS_USR				0x3Du /* R/W */
#define LIS2DW12_REG_Z_OFS_USR				0x3Eu /* R/W */
#define LIS2DW12_REG_CTRL7					0x3Fu /* R/W */

/* Private typedef -------------------------------------------------------------------------------*/
#ifdef STM32F429xx
typedef void (*i2c_init_t)(void);
typedef i2c_status_t (*i2c_master_tx_t)(uint16_t, uint8_t*, uint16_t);
typedef i2c_status_t (*i2c_master_rx_t)(uint16_t, uint8_t*, uint16_t);
typedef i2c_status_t (*i2c_mem_write_t)(uint16_t, uint16_t, uint16_t, uint8_t*, uint16_t);
typedef i2c_status_t (*i2c_mem_read_t)(uint16_t, uint16_t, uint16_t, uint8_t*, uint16_t);
typedef i2c_status_t (*i2c_is_dev_ready_t)(uint16_t, uint32_t);
#endif

typedef struct {
	i2c_init_t i2c_init;
	i2c_master_tx_t i2c_master_tx;
	i2c_master_rx_t i2c_master_rx;
	i2c_mem_write_t i2c_mem_write;
	i2c_mem_read_t i2c_mem_read;
	i2c_is_dev_ready_t i2c_is_dev_ready;
	lis2dw12_config_t config;
	lis2dw12_status_t status;
} lis2dw12_control_t;



/* Private macro ---------------------------------------------------------------------------------*/

/* Private variables -----------------------------------------------------------------------------*/

static lis2dw12_control_t lis2dw12;

/* Private prototypes ----------------------------------------------------------------------------*/
static void set_reg(uint8_t reg_address, uint8_t reg_val);
static void get_reg(uint8_t reg_address, uint8_t *reg_val);
static void get_raw_x(int16_t *raw_x);
static void get_raw_y(int16_t *raw_y);
static void get_raw_z(int16_t *raw_z);

/* Functions definitions -------------------------------------------------------------------------*/

static void set_reg(uint8_t reg_address, uint8_t reg_val)
{
	i2c_status_t i2c_status = lis2dw12.i2c_mem_write(LIS2DW12_SAD_W, (uint16_t)reg_address, LIS2DW12_REG_SIZE, &reg_val, sizeof(uint8_t));

	lis2dw12.status = (i2c_status == I2C_OK) ? LIS2DW12_OK : LIS2DW12_NOK;
}

static void get_reg(uint8_t reg_address, uint8_t *reg_val)
{
	lis2dw12.status = LIS2DW12_NOK;

	if (NULL != reg_val) {
		i2c_status_t i2c_status = lis2dw12.i2c_mem_read(LIS2DW12_SAD_R, (uint16_t)reg_address, LIS2DW12_REG_SIZE, &reg_val, sizeof(uint8_t));

		if (i2c_status == I2C_OK) {
			lis2dw12.status = LIS2DW12_OK;
		}
		else {
			*reg_val = 0u;
		}
	}
}

static void get_raw_x(int16_t *raw_x)
{
	uint8_t data[2u];
	lis2dw12.status = LIS2DW12_NOK;

	if (NULL != raw_x) {
		i2c_status_t i2c_status = lis2dw12.i2c_mem_read(LIS2DW12_SAD_R, LIS2DW12_REG_OUT_X_L, LIS2DW12_REG_SIZE, &data[0u], sizeof(data));

		if (I2C_OK == i2c_status) {
			*raw_x = (int16_t)((uint16_t)data[1u] << 8u | (uint16_t)data[0u]);
			lis2dw12.status = I2C_OK;
		}
	}
}

static void get_raw_y(int16_t *raw_y)
{
	uint8_t data[2u];
	lis2dw12.status = LIS2DW12_NOK;

	if (NULL != raw_y) {
		i2c_status_t i2c_status = lis2dw12.i2c_mem_read(LIS2DW12_SAD_R, LIS2DW12_REG_OUT_Y_L, LIS2DW12_REG_SIZE, &data[0u], sizeof(data));

		if (I2C_OK == i2c_status) {
			*raw_y = (int16_t)((uint16_t)data[1u] << 8u | (uint16_t)data[0u]);
			lis2dw12.status = I2C_OK;
		}
	}
}

static void get_raw_z(int16_t *raw_z)
{
	uint8_t data[2u];
	lis2dw12.status = LIS2DW12_NOK;

	if (NULL != raw_z) {
		i2c_status_t i2c_status = lis2dw12.i2c_mem_read(LIS2DW12_SAD_R, LIS2DW12_REG_OUT_Z_L, LIS2DW12_REG_SIZE, &data[0u], sizeof(data));

		if (I2C_OK == i2c_status) {
			*raw_z = (int16_t)((uint16_t)data[1u] << 8u | (uint16_t)data[0u]);
			lis2dw12.status = I2C_OK;
		}
	}
}

void lis2dw12_init(lis2dw12_config_t usr_config)
{
	lis2dw12_status_t status = LIS2DW12_NOK;
	uint8_t reg_ctrl1_val = 0u;
	uint8_t reg_ctrl6_val = 0u;

	#ifdef STM32F429xx
	lis2dw12.i2c_init = stm32f429xx_i2c_init;
	lis2dw12.i2c_master_tx = stm32f429xx_i2c_master_tx;
	lis2dw12.i2c_master_rx = stm32f429xx_i2c_master_rx;
	lis2dw12.i2c_mem_write = stm32f429xx_i2c_mem_write;
	lis2dw12.i2c_mem_read = stm32f429xx_i2c_mem_read;
	lis2dw12.i2c_is_dev_ready = stm32f429xx_i2c_is_dev_ready;
	#endif

	lis2dw12.config.odr = (usr_config.odr > LIS2DW12_ODR_1600_HZ) ? LIS2DW12_ODR_DEFAULT : usr_config.odr;
	lis2dw12.config.mode = (usr_config.mode > LIS2DW12_MODE_ON_DM) ? LIS2DW12_MODE_DEFAULT : usr_config.mode;
	lis2dw12.config.lpmode = (usr_config.lpmode > LIS2DW12_LP_MODE_4) ? LIS2DW12_LP_MODE_DEFAULT : usr_config.lpmode;
	lis2dw12.config.bwfilt = (usr_config.bwfilt > LIS2DW12_BW_FILT_4) ? LIS2DW12_BW_FILT_DEFAULT : usr_config.bwfilt;
	lis2dw12.config.fs = (usr_config.fs > LIS2DW12_FS_16_G) ?  LIS2DW12_FS_DEFAULT : usr_config.fs;
	lis2dw12.config.fds = (usr_config.fds > LIS2DW12_FDS_HP_FILT) ? LIS2DW12_FDS_DEFAULT : usr_config.fds;
	lis2dw12.config.lownoise = (usr_config.lownoise > LIS2DW12_LOW_NOISE_ON) ? LIS2DW12_LOW_NOISE_DEFAULT : usr_config.lownoise;;

	lis2dw12.i2c_init();

	reg_ctrl1_val = (lis2dw12.config.odr << 4u | lis2dw12.config.mode << 2u | lis2dw12.config.lpmode);
	reg_ctrl6_val = (lis2dw12.config.bwfilt << 6u | lis2dw12.config.fs << 4u | lis2dw12.config.fds << 3u | lis2dw12.config.lownoise << 2u);

	set_reg(LIS2DW12_REG_CTRL1, reg_ctrl1_val);

	set_reg(LIS2DW12_REG_CTRL6, reg_ctrl6_val);

}

uint8_t lis2dw12_get_id(void)
{
	uint8_t data;

	lis2dw12.i2c_mem_read(LIS2DW12_SAD_R, LIS2DW12_REG_WHO_AM_I, LIS2DW12_REG_SIZE, &data, 1u);

	return data;
}

lis2dw12_status_t lis2dw12_is_data_ready(void)
{
	uint8_t data_ready;

	get_reg(LIS2DW12_REG_STATUS, &data_ready);

	return (lis2dw12_status_t)(data_ready &= 0x1u);
}

int8_t lis2dw12_get_raw_temp_8bit(void)
{
	uint8_t data;

	lis2dw12.i2c_mem_read(LIS2DW12_SAD_R, LIS2DW12_REG_OUT_T, LIS2DW12_REG_SIZE, &data, 1u);

	return (int8_t)data;
}

int16_t lis2dw12_get_raw_temp_12bit(void)
{
	uint8_t data[2u];

	lis2dw12.i2c_mem_read(LIS2DW12_SAD_R, LIS2DW12_REG_OUT_T_L, LIS2DW12_REG_SIZE, &data[0u], 2u);

	return (int16_t)((uint16_t)data[1u] << 8u | data[0u]);
}

int16_t lis2dw12_get_mg_x(void)
{
	float sen_fac = 0u;
	uint8_t bit_shift = (lis2dw12.config.lpmode == LIS2DW12_LP_MODE_1) ? 4u : 2u;
	int16_t data;

	get_raw_x(&data);

	data >>= bit_shift;

	switch(lis2dw12.config.fs)
	{
		case LIS2DW12_FS_2_G:
			sen_fac = (lis2dw12.config.lpmode == LIS2DW12_LP_MODE_1) ? 0.976 : 0.244;
			break;
		case LIS2DW12_FS_4_G:
			sen_fac = (lis2dw12.config.lpmode == LIS2DW12_LP_MODE_1) ? 1.952 : 0.488;
			break;
		case LIS2DW12_FS_8_G:
			sen_fac = (lis2dw12.config.lpmode == LIS2DW12_LP_MODE_1) ? 3.904 : 0.976;
			break;
		case LIS2DW12_FS_16_G:
			sen_fac = (lis2dw12.config.lpmode == LIS2DW12_LP_MODE_1) ? 7.808 : 1.952;
			break;
		default:
			/* Data will be 0 */
			break;
	}

	data *= sen_fac;

	return (data);
}

int16_t lis2dw12_get_mg_y(void)
{
	float sen_fac = 0u;
	uint8_t bit_shift = (lis2dw12.config.lpmode == LIS2DW12_LP_MODE_1) ? 4u : 2u;
	int16_t data;

	get_raw_y(&data);

	data >>= bit_shift;

	switch(lis2dw12.config.fs)
	{
		case LIS2DW12_FS_2_G:
			sen_fac = (lis2dw12.config.lpmode == LIS2DW12_LP_MODE_1) ? 0.976 : 0.244;
			break;
		case LIS2DW12_FS_4_G:
			sen_fac = (lis2dw12.config.lpmode == LIS2DW12_LP_MODE_1) ? 1.952 : 0.488;
			break;
		case LIS2DW12_FS_8_G:
			sen_fac = (lis2dw12.config.lpmode == LIS2DW12_LP_MODE_1) ? 3.904 : 0.976;
			break;
		case LIS2DW12_FS_16_G:
			sen_fac = (lis2dw12.config.lpmode == LIS2DW12_LP_MODE_1) ? 7.808 : 1.952;
			break;
		default:
			/* Data will be 0 */
			break;
	}

	data *= sen_fac;

	return (data);
}

int16_t lis2dw12_get_mg_z(void)
{
	float sen_fac = 0u;
	uint8_t bit_shift = (lis2dw12.config.lpmode == LIS2DW12_LP_MODE_1) ? 4u : 2u;
	int16_t data;

	get_raw_z(&data);

	data >>= bit_shift;

	switch(lis2dw12.config.fs)
	{
		case LIS2DW12_FS_2_G:
			sen_fac = (lis2dw12.config.lpmode == LIS2DW12_LP_MODE_1) ? 0.976 : 0.244;
			break;
		case LIS2DW12_FS_4_G:
			sen_fac = (lis2dw12.config.lpmode == LIS2DW12_LP_MODE_1) ? 1.952 : 0.488;
			break;
		case LIS2DW12_FS_8_G:
			sen_fac = (lis2dw12.config.lpmode == LIS2DW12_LP_MODE_1) ? 3.904 : 0.976;
			break;
		case LIS2DW12_FS_16_G:
			sen_fac = (lis2dw12.config.lpmode == LIS2DW12_LP_MODE_1) ? 7.808 : 1.952;
			break;
		default:
			/* Data will be 0 */
			break;
	}

	data *= sen_fac;

	return (data);
}

lis2dw12_odr_t lis2dw12_get_odr(void)
{
	uint8_t data;

	lis2dw12.i2c_mem_read(LIS2DW12_SAD_R, LIS2DW12_REG_CTRL1, LIS2DW12_REG_SIZE, &data, 1u);

	return (lis2dw12_odr_t)(data >> 4u);
}

lis2dw12_mode_t lis2dw12_get_mode(void)
{
	uint8_t data;

	lis2dw12.i2c_mem_read(LIS2DW12_SAD_R, LIS2DW12_REG_CTRL1, LIS2DW12_REG_SIZE, &data, 1u);

	return (lis2dw12_mode_t)(data >> 2u);
}

lis2dw12_lpmode_t lis2dw12_get_lpmode(void)
{
	uint8_t data;

	lis2dw12.i2c_mem_read(LIS2DW12_SAD_R, LIS2DW12_REG_CTRL1, LIS2DW12_REG_SIZE, &data, 1u);

	return (lis2dw12_lpmode_t)(data);
}

lis2dw12_fs_t lis2dw12_get_fs(void)
{
	uint8_t data;

	lis2dw12.i2c_mem_read(LIS2DW12_SAD_R, LIS2DW12_REG_CTRL6, LIS2DW12_REG_SIZE, &data, 1u);

	return (lis2dw12_fs_t)(data >> 4u);
}
