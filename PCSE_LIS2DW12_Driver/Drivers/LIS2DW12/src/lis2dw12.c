/**************************************************************************************************
 * @file
 * @brief  LIS2DW12 Driver
 **************************************************************************************************
 * @author Lautaro Vera
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

#define LIS2DW12_REG_OUT_T_L				0x0Du
#define LIS2DW12_REG_OUT_T_H				0x0Eu
#define LIS2DW12_REG_WHO_AM_I				0x0Fu
#define LIS2DW12_REG_CTRL1					0x20u
#define LIS2DW12_REG_CTRL2					0x21u
#define LIS2DW12_REG_CTRL3					0x22u
#define LIS2DW12_REG_CTRL4_INT1_PAD_CTRL	0x23u
#define LIS2DW12_REG_CTRL5_INT2_PAD_CTRL	0x24u
#define LIS2DW12_REG_CTRL6					0x25u
#define LIS2DW12_REG_OUT_T					0x26u
#define LIS2DW12_REG_STATUS					0x27u
#define LIS2DW12_REG_OUT_X_L				0x28u
#define LIS2DW12_REG_OUT_X_H				0x29u
#define LIS2DW12_REG_OUT_Y_L				0x2Au
#define LIS2DW12_REG_OUT_Y_H				0x2Bu
#define LIS2DW12_REG_OUT_Z_L				0x2Cu
#define LIS2DW12_REG_OUT_Z_H				0x2Du
#define LIS2DW12_REG_FIFO_CTRL				0x2Eu
#define LIS2DW12_REG_FIFO_SAMPLES			0x2Fu
#define LIS2DW12_REG_TAP_THS_X				0x30u
#define LIS2DW12_REG_TAP_THS_Y				0x31u
#define LIS2DW12_REG_TAP_THS_Z				0x32u
#define LIS2DW12_REG_INT_DUR				0x33u
#define LIS2DW12_REG_WAKE_UP_THS			0x34u
#define LIS2DW12_REG_WAKE_UP_DUR			0x35u
#define LIS2DW12_REG_FREE_FALL				0x36u
#define LIS2DW12_REG_STATUS_DUP				0x37u
#define LIS2DW12_REG_WAKE_UP_SRC			0x38u
#define LIS2DW12_REG_TAP_SRC				0x39u
#define LIS2DW12_REG_SIXD_SRC				0x3Au
#define LIS2DW12_REG_ALL_INT_SRC			0x3Bu
#define LIS2DW12_REG_X_OFS_USR				0x3Cu
#define LIS2DW12_REG_Y_OFS_USR				0x3Du
#define LIS2DW12_REG_Z_OFS_USR				0x3Eu
#define LIS2DW12_REG_CTRL7					0x3Fu

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
} lis2dw12_control_t;



/* Private macro ---------------------------------------------------------------------------------*/

/* Private variables -----------------------------------------------------------------------------*/

static lis2dw12_control_t lis2dw12;

/* Private prototypes ----------------------------------------------------------------------------*/
static lis2dw12_status_t set_reg(uint8_t reg_address, uint8_t reg_val);
static lis2dw12_status_t get_reg(uint8_t reg_address, uint8_t *reg_val);
static lis2dw12_status_t get_raw_x(int16_t *raw_x);
static lis2dw12_status_t get_raw_y(int16_t *raw_y);
static lis2dw12_status_t get_raw_z(int16_t *raw_z);
static int16_t raw_to_mg(int16_t raw_data);

/* Private API definitions -----------------------------------------------------------------------*/
static lis2dw12_status_t set_reg(uint8_t reg_address, uint8_t reg_val)
{
	i2c_status_t i2c_status = lis2dw12.i2c_mem_write(LIS2DW12_SAD_W, (uint16_t)reg_address, LIS2DW12_REG_SIZE, &reg_val, sizeof(uint8_t));
	lis2dw12_status_t status = (i2c_status == I2C_OK) ? LIS2DW12_OK : LIS2DW12_NOK;

	return status;
}

static lis2dw12_status_t get_reg(uint8_t reg_address, uint8_t *reg_val)
{
	lis2dw12_status_t status = LIS2DW12_NOK;

	if (NULL != reg_val) {
		i2c_status_t i2c_status = lis2dw12.i2c_mem_read(LIS2DW12_SAD_R, (uint16_t)reg_address, LIS2DW12_REG_SIZE, reg_val, sizeof(uint8_t));

		if (i2c_status == I2C_OK) {
			status = LIS2DW12_OK;
		}
		else {
			*reg_val = 0u;
		}
	}

	return status;
}

static lis2dw12_status_t get_raw_x(int16_t *raw_x)
{
	lis2dw12_status_t status = LIS2DW12_NOK;
	uint8_t data[2u];

	if (NULL != raw_x) {
		i2c_status_t i2c_status = lis2dw12.i2c_mem_read(LIS2DW12_SAD_R, LIS2DW12_REG_OUT_X_L, LIS2DW12_REG_SIZE, &data[0u], sizeof(data));

		if (I2C_OK == i2c_status) {
			*raw_x = (int16_t)((uint16_t)data[1u] << 8u | (uint16_t)data[0u]);
			status = LIS2DW12_OK;
		}
	}

	return status;
}

static lis2dw12_status_t get_raw_y(int16_t *raw_y)
{
	lis2dw12_status_t status = LIS2DW12_NOK;
	uint8_t data[2u];

	if (NULL != raw_y) {
		i2c_status_t i2c_status = lis2dw12.i2c_mem_read(LIS2DW12_SAD_R, LIS2DW12_REG_OUT_Y_L, LIS2DW12_REG_SIZE, &data[0u], sizeof(data));

		if (I2C_OK == i2c_status) {
			*raw_y = (int16_t)((uint16_t)data[1u] << 8u | (uint16_t)data[0u]);
			status = LIS2DW12_OK;
		}
	}

	return status;
}

static lis2dw12_status_t get_raw_z(int16_t *raw_z)
{
	lis2dw12_status_t status = LIS2DW12_NOK;
	uint8_t data[2u];

	if (NULL != raw_z) {
		i2c_status_t i2c_status = lis2dw12.i2c_mem_read(LIS2DW12_SAD_R, LIS2DW12_REG_OUT_Z_L, LIS2DW12_REG_SIZE, &data[0u], sizeof(data));

		if (I2C_OK == i2c_status) {
			*raw_z = (int16_t)((uint16_t)data[1u] << 8u | (uint16_t)data[0u]);
			status = LIS2DW12_OK;
		}
	}

	return status;
}

static lis2dw12_status_t get_raw_fifo(int16_t *raw_x, int16_t *raw_y, int16_t *raw_z, uint8_t samples)
{
	lis2dw12_status_t status = LIS2DW12_NOK;
	/* Each sample is composed by 6 bytes, since there are three axis and each axis is defined by the high and low byte registers */
	uint8_t size = 6u;
	uint8_t data[samples][size];

	if (NULL != raw_x && NULL != raw_y && NULL != raw_z) {
		for (uint8_t i = 0u; i < samples; i++) {
			i2c_status_t i2c_status = lis2dw12.i2c_mem_read(LIS2DW12_SAD_R, LIS2DW12_REG_OUT_X_L, LIS2DW12_REG_SIZE, &data[i][0u], size);

			if (I2C_OK == i2c_status) {
				*raw_x++ = (int16_t)((uint16_t)data[i][1u] << 8u | (uint16_t)data[i][0u]);
				*raw_y++ = (int16_t)((uint16_t)data[i][3u] << 8u | (uint16_t)data[i][2u]);
				*raw_z++ = (int16_t)((uint16_t)data[i][5u] << 8u | (uint16_t)data[i][4u]);

				status = LIS2DW12_OK;
			}
		}
	}

	return status;
}

static int16_t raw_to_mg(int16_t raw_data)
{
	float sen_fac = 0;
	uint8_t bit_shift = (lis2dw12.config.lp_mode == LIS2DW12_LP_MODE_1) ? 4u : 2u;

	raw_data >>= bit_shift;

	switch (lis2dw12.config.fs)
	{
		case LIS2DW12_FS_2_G:
			sen_fac = (lis2dw12.config.lp_mode == LIS2DW12_LP_MODE_1) ? 0.976 : 0.244;
			break;
		case LIS2DW12_FS_4_G:
			sen_fac = (lis2dw12.config.lp_mode == LIS2DW12_LP_MODE_1) ? 1.952 : 0.488;
			break;
		case LIS2DW12_FS_8_G:
			sen_fac = (lis2dw12.config.lp_mode == LIS2DW12_LP_MODE_1) ? 3.904 : 0.976;
			break;
		case LIS2DW12_FS_16_G:
			sen_fac = (lis2dw12.config.lp_mode == LIS2DW12_LP_MODE_1) ? 7.808 : 1.952;
			break;
		default:
			/* Data will be 0 */
			break;
	}

	raw_data *= sen_fac;

	return raw_data;
}

/* Public API definitions -------------------------------------------------------------------------*/
lis2dw12_status_t lis2dw12_init(lis2dw12_config_t usr_config)
{
	lis2dw12_status_t status = LIS2DW12_OK;
	uint8_t reg_ctrl1_val = 0u;
	uint8_t reg_ctrl2_val = 0xC;
	uint8_t reg_ctrl6_val = 0u;
	uint8_t reg_fifo_ctrl_val = 0u;

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
	lis2dw12.config.lp_mode = (usr_config.lp_mode > LIS2DW12_LP_MODE_4) ? LIS2DW12_LP_MODE_DEFAULT : usr_config.lp_mode;
	lis2dw12.config.bw_filt = (usr_config.bw_filt > LIS2DW12_BW_FILT_4) ? LIS2DW12_BW_FILT_DEFAULT : usr_config.bw_filt;
	lis2dw12.config.fs = (usr_config.fs > LIS2DW12_FS_16_G) ?  LIS2DW12_FS_DEFAULT : usr_config.fs;
	lis2dw12.config.fds = (usr_config.fds == LIS2DW12_FDS_HP_FILT) ? usr_config.fds : LIS2DW12_FDS_DEFAULT;
	lis2dw12.config.low_noise = (usr_config.low_noise == LIS2DW12_LOW_NOISE_ON) ? usr_config.low_noise : LIS2DW12_LOW_NOISE_DEFAULT;
	lis2dw12.config.fifo_mode = (usr_config.fifo_mode == LIS2DW12_FIFO_MODE_ON) ? usr_config.fifo_mode : LIS2DW12_FIFO_MODE_DEFAULT;
	lis2dw12.config.fifo_ths = usr_config.fifo_ths - 1u;
	lis2dw12.config.fifo_ths = (usr_config.fifo_ths > 31u) ? 31u : usr_config.fifo_ths;

	lis2dw12.i2c_init();

	reg_ctrl1_val = lis2dw12.config.odr << 4u | lis2dw12.config.mode << 2u | lis2dw12.config.lp_mode;
	reg_ctrl6_val = lis2dw12.config.bw_filt << 6u | lis2dw12.config.fs << 4u | lis2dw12.config.fds << 3u | lis2dw12.config.low_noise << 2u;
	reg_fifo_ctrl_val = lis2dw12.config.fifo_mode << 5u | lis2dw12.config.fifo_ths;

	status = set_reg(LIS2DW12_REG_CTRL1, reg_ctrl1_val);
	status &= set_reg(LIS2DW12_REG_CTRL2, reg_ctrl2_val);
	status &= set_reg(LIS2DW12_REG_CTRL6, reg_ctrl6_val);
	status &= set_reg(LIS2DW12_REG_FIFO_CTRL, reg_fifo_ctrl_val);

	return status;
}

lis2dw12_status_t lis2dw12_get_config(lis2dw12_config_t *current_config)
{
	lis2dw12_status_t status = LIS2DW12_NOK;
	uint8_t reg_ctrl1_val = 0u;
	uint8_t reg_ctrl6_val = 0u;
	uint8_t reg_fifo_ctrl_val = 0u;

	if (NULL != current_config) {
		i2c_status_t i2c_status = lis2dw12.i2c_mem_read(LIS2DW12_SAD_R, LIS2DW12_REG_CTRL1, LIS2DW12_REG_SIZE, &reg_ctrl1_val, sizeof(reg_ctrl1_val));
		i2c_status &= lis2dw12.i2c_mem_read(LIS2DW12_SAD_R, LIS2DW12_REG_CTRL6, LIS2DW12_REG_SIZE, &reg_ctrl6_val, sizeof(reg_ctrl6_val));
		i2c_status &= lis2dw12.i2c_mem_read(LIS2DW12_SAD_R, LIS2DW12_REG_FIFO_CTRL, LIS2DW12_REG_SIZE, &reg_fifo_ctrl_val, sizeof(reg_fifo_ctrl_val));

		if (I2C_OK == i2c_status) {
			current_config->odr = reg_ctrl1_val >> 4u;
			current_config->mode = reg_ctrl1_val >> 2u & 0x3u;
			current_config->lp_mode = reg_ctrl1_val & 0x3u;
			current_config->bw_filt = reg_ctrl6_val >> 6u;
			current_config->fs = reg_ctrl6_val >> 4u & 0x3u;
			current_config->fds = reg_ctrl6_val >> 3u & 0x1u;
			current_config->low_noise = reg_ctrl6_val >> 2u & 0x1u;
			current_config->fifo_mode = reg_fifo_ctrl_val >> 5u;
			current_config->fifo_ths = reg_fifo_ctrl_val & 0x1Fu;

			status = LIS2DW12_OK;
		}
	}

	return status;
}

lis2dw12_status_t lis2dw12_get_id(uint8_t *id)
{
	lis2dw12_status_t status = LIS2DW12_NOK;

	if (NULL != id) {
		i2c_status_t i2c_status = lis2dw12.i2c_mem_read(LIS2DW12_SAD_R, LIS2DW12_REG_WHO_AM_I, LIS2DW12_REG_SIZE, id, sizeof(id));

		status = (i2c_status == I2C_OK) ? LIS2DW12_OK : LIS2DW12_NOK;
	}

	return status;
}

lis2dw12_status_t lis2dw12_is_data_ready(void)
{
	uint8_t data_ready;
	lis2dw12_status_t status = get_reg(LIS2DW12_REG_STATUS, &data_ready);

	data_ready &= (status == LIS2DW12_OK) ? 0x1u : 0x0u;

	return (lis2dw12_status_t)data_ready;
}

lis2dw12_status_t lis2dw12_is_fifo_ready(void)
{
	uint8_t fifo_ready;
	lis2dw12_status_t status = get_reg(LIS2DW12_REG_FIFO_SAMPLES, &fifo_ready);

	fifo_ready >>= (status == LIS2DW12_OK) ? 7u : 8u;

	return (lis2dw12_status_t)fifo_ready;
}

lis2dw12_status_t lis2dw12_is_fifo_full(void)
{
	uint8_t fifo_full;
	lis2dw12_status_t status = get_reg(LIS2DW12_REG_FIFO_SAMPLES, &fifo_full);

	fifo_full >>= (status == LIS2DW12_OK) ? 6u : 8u;

	return (lis2dw12_status_t)fifo_full;
}

int16_t lis2dw12_get_mg_x(void)
{
	int16_t data = 0;

	if (LIS2DW12_OK == get_raw_x(&data)) {
		data = raw_to_mg(data);
	}

	return data;
}

int16_t lis2dw12_get_mg_y(void)
{
	int16_t data = 0;

	if (LIS2DW12_OK == get_raw_y(&data)) {
		data = raw_to_mg(data);
	}

	return data;
}

int16_t lis2dw12_get_mg_z(void)
{
	int16_t data = 0;

	if (LIS2DW12_OK == get_raw_z(&data)) {
		data = raw_to_mg(data);
	}

	return data;
}

lis2dw12_status_t lis2dw12_get_fifo_mg(int16_t *buffer_x, int16_t *buffer_y, int16_t *buffer_z, uint8_t buffer_len)
{
	lis2dw12_status_t status = LIS2DW12_NOK;
	uint8_t samples = lis2dw12.config.fifo_ths + 1u;

	if (buffer_len >= samples) {
		uint8_t reg_fifo_ctrl_val = LIS2DW12_FIFO_MODE_OFF << 5u | lis2dw12.config.fifo_ths;

		if (LIS2DW12_OK == get_raw_fifo(buffer_x, buffer_y, buffer_z, samples)) {
			int16_t tmp_x = 0;
			int16_t tmp_y = 0;
			int16_t tmp_z = 0;

			while (samples--) {
				tmp_x = *buffer_x;
				tmp_y = *buffer_y;
				tmp_z = *buffer_z;

				*buffer_x++ = raw_to_mg(tmp_x);
				*buffer_y++ = raw_to_mg(tmp_y);
				*buffer_z++ = raw_to_mg(tmp_z);
			}
		}

		/* Enables again the FIFO */
		status = set_reg(LIS2DW12_REG_FIFO_CTRL, reg_fifo_ctrl_val);

		reg_fifo_ctrl_val = lis2dw12.config.fifo_mode << 5u | lis2dw12.config.fifo_ths;

		status &= set_reg(LIS2DW12_REG_FIFO_CTRL, reg_fifo_ctrl_val);
	}

	return status;
}
