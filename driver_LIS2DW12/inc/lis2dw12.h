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
  LIS2DW12_ODR_UNDEF        = 0xFu
} lis2dw12_odr_t;

typedef enum {
  LIS2DW12_MODE_LOW_PWR     = 0x0u,
  LIS2DW12_MODE_HI_PF       = 0x1u,
  LIS2DW12_MODE_ON_DM       = 0x2u,
  LIS2DW12_MODE_UNDEF       = 0xFu
} lis2dw12_mode_t;

typedef enum {
  LIS2DW12_LP_MODE_1        = 0x0u,
  LIS2DW12_LP_MODE_2        = 0x1u,
  LIS2DW12_LP_MODE_3        = 0x2u,
  LIS2DW12_LP_MODE_4        = 0x3u,
  LIS2DW12_LP_MODE_UNDEF    = 0xFu
} lis2dw12_lpmode_t;

typedef enum {
  LIS2DW12_BW_FILT_1        = 0x0u,
  LIS2DW12_BW_FILT_2        = 0x1u,
  LIS2DW12_BW_FILT_3        = 0x2u,
  LIS2DW12_BW_FILT_4        = 0x3u,
  LIS2DW12_BW_FILT_UNDEF    = 0xFu
} lis2dw12_bwfilt_t;

typedef enum {
  LIS2DW12_FS_2_G           = 0x0u,
  LIS2DW12_FS_4_G           = 0x1u,
  LIS2DW12_FS_8_G           = 0x2u,
  LIS2DW12_FS_16_G          = 0x3u,
  LIS2DW12_FS_UNDEF         = 0xFu
} lis2dw12_fs_t;

typedef enum {
  LIS2DW12_FDS_LP_FILT      = 0u,
  LIS2DW12_FDS_HP_FILT      = 1u
} lis2dw12_fds_t;

typedef enum {
  LIS2DW12_LOW_NOISE_OFF    = 0u,
  LIS2DW12_LOW_NOISE_ON     = 1u
} lis2dw12_lownoise_t;

struct _lis2dw12_config  {
  lis2dw12_odr_t odr;
  lis2dw12_mode_t mode;
  lis2dw12_lpmode_t lpmode;
  lis2dw12_bwfilt_t bwfilt;
  lis2dw12_fs_t fs;
  lis2dw12_fds_t fds;
  lis2dw12_lownoise_t lownoise;
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
void lis2dw12_init(lis2dw12_config_t usr_config);

int8_t lis2dw12_get_raw_temp_8bit(void);

int16_t lis2dw12_get_raw_temp_12bit(void);

int16_t lis2dw12_get_raw_x(void);

int16_t lis2dw12_get_raw_y(void);

int16_t lis2dw12_get_raw_z(void);

int16_t lis2dw12_get_mg_x(void);

int16_t lis2dw12_get_mg_y(void);

int16_t lis2dw12_get_mg_z(void);

uint8_t lis2dw12_who_am_i(void);

lis2dw12_odr_t lis2dw12_get_odr(void);

lis2dw12_mode_t lis2dw12_get_mode(void);

lis2dw12_lpmode_t lis2dw12_get_lpmode(void);

lis2dw12_fs_t lis2dw12_get_fs(void);


#endif /* LIS2DW12_H_ */
