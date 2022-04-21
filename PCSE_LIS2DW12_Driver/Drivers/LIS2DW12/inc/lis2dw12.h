/**************************************************************************************************
 * @file   : lis2dw12.h
 * @brief  : LIS2DW12 Driver
 **************************************************************************************************
 * @author : Lautaro Vera
 **************************************************************************************************/

#ifndef LIS2DW12_H_
#define LIS2DW12_H_

/* Includes --------------------------------------------------------------------------------------*/

#include <stdint.h>

/* Public typedef --------------------------------------------------------------------------------*/

typedef enum {
  LIS2DW12_NOK,
  LIS2DW12_OK
} lis2dw12_status_t;

typedef enum {
  LIS2DW12_ODR_PWR_DWN      = 0x0u,
  LIS2DW12_ODR_1_6_HZ       = 0x1u,
  LIS2DW12_ODR_12_5_HZ      = 0x2u,
  LIS2DW12_ODR_25_HZ        = 0x3u,
  LIS2DW12_ODR_50_HZ        = 0x4u,
  LIS2DW12_ODR_100_HZ       = 0x5u,
  LIS2DW12_ODR_200_HZ       = 0x6u,
  LIS2DW12_ODR_400_HZ       = 0x7u,
  LIS2DW12_ODR_800_HZ       = 0x8u,
  LIS2DW12_ODR_1600_HZ      = 0x9u,
  LIS2DW12_ODR_DEFAULT      = 0x6u
} lis2dw12_odr_t;

typedef enum {
  LIS2DW12_MODE_LOW_PWR     = 0x0u,
  LIS2DW12_MODE_HI_PF       = 0x1u,
  LIS2DW12_MODE_ON_DM       = 0x2u,
  LIS2DW12_MODE_DEFAULT     = 0x1u
} lis2dw12_mode_t;

typedef enum {
  LIS2DW12_LP_MODE_1         = 0x0u,
  LIS2DW12_LP_MODE_2         = 0x1u,
  LIS2DW12_LP_MODE_3         = 0x2u,
  LIS2DW12_LP_MODE_4         = 0x3u,
  LIS2DW12_LP_MODE_DEFAULT   = 0x1u
} lis2dw12_lpmode_t;

typedef enum {
  LIS2DW12_BW_FILT_1         = 0x0u,
  LIS2DW12_BW_FILT_2         = 0x1u,
  LIS2DW12_BW_FILT_3         = 0x2u,
  LIS2DW12_BW_FILT_4         = 0x3u,
  LIS2DW12_BW_FILT_DEFAULT   = 0x1u
} lis2dw12_bwfilt_t;

typedef enum {
  LIS2DW12_FS_2_G            = 0x0u,
  LIS2DW12_FS_4_G            = 0x1u,
  LIS2DW12_FS_8_G            = 0x2u,
  LIS2DW12_FS_16_G           = 0x3u,
  LIS2DW12_FS_DEFAULT        = 0x1u
} lis2dw12_fs_t;

typedef enum {
  LIS2DW12_FDS_LP_FILT       = 0x0u,
  LIS2DW12_FDS_HP_FILT       = 0x1u,
  LIS2DW12_FDS_DEFAULT       = 0x0u
} lis2dw12_fds_t;

typedef enum {
  LIS2DW12_LOW_NOISE_OFF     = 0x0u,
  LIS2DW12_LOW_NOISE_ON      = 0x1u,
  LIS2DW12_LOW_NOISE_DEFAULT = 0x0u
} lis2dw12_lownoise_t;

typedef enum {
  LIS2DW12_FIFO_MODE_OFF     = 0x0u,
  LIS2DW12_FIFO_MODE_ON      = 0x1u,
  LIS2DW12_FIFO_MODE_DEFAULT = 0x0u
} lis2dw12_fifomode_t;

struct _lis2dw12_config  {
  lis2dw12_odr_t odr;
  lis2dw12_mode_t mode;
  lis2dw12_lpmode_t lp_mode;
  lis2dw12_bwfilt_t bw_filt;
  lis2dw12_fs_t fs;
  lis2dw12_fds_t fds;
  lis2dw12_lownoise_t low_noise;
  lis2dw12_fifomode_t fifo_mode;
  uint8_t fifo_ths;
};

typedef struct _lis2dw12_config lis2dw12_config_t;

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
lis2dw12_status_t lis2dw12_init(lis2dw12_config_t usr_config);

lis2dw12_status_t lis2dw12_get_config(lis2dw12_config_t *current_config);

lis2dw12_status_t lis2dw12_get_id(uint8_t *id);

lis2dw12_status_t lis2dw12_is_data_ready(void);

lis2dw12_status_t lis2dw12_is_fifo_ready(void);

lis2dw12_status_t lis2dw12_is_fifo_full(void);

int16_t lis2dw12_get_mg_x(void);

int16_t lis2dw12_get_mg_y(void);

int16_t lis2dw12_get_mg_z(void);

lis2dw12_status_t lis2dw12_get_fifo_mg(int16_t *buffer_x, int16_t *buffer_y, int16_t *buffer_z, uint8_t buffer_len);


#endif /* LIS2DW12_H_ */
