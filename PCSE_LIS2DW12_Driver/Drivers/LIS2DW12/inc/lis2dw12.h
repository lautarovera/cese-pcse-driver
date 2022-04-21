/**************************************************************************************************
 * @file
 * @brief  LIS2DW12 Driver
 **************************************************************************************************
 * @author Lautaro Vera
 **************************************************************************************************/

#ifndef LIS2DW12_H_
#define LIS2DW12_H_

/* Includes --------------------------------------------------------------------------------------*/

#include <stdint.h>

/* Public typedef --------------------------------------------------------------------------------*/

/**
 *  @brief Tipo para indicar si la operacion fue exitosa o no
 */
typedef enum {
  LIS2DW12_NOK,                       /**< Operacion exitosa */
  LIS2DW12_OK                         /**< Operacion fallida */
} lis2dw12_status_t;

/**
 *  @brief Tipo para la seleccion de la frecuencia de muestreo
 */
typedef enum {
  LIS2DW12_ODR_PWR_DWN      = 0x0u,   /**< Acelerometro apagado */
  LIS2DW12_ODR_1_6_HZ       = 0x1u,   /**< HR en 12.5Hz, LP en 1.6Hz */
  LIS2DW12_ODR_12_5_HZ      = 0x2u,   /**< 12.5Hz */
  LIS2DW12_ODR_25_HZ        = 0x3u,   /**< 25Hz */
  LIS2DW12_ODR_50_HZ        = 0x4u,   /**< 50Hz */
  LIS2DW12_ODR_100_HZ       = 0x5u,   /**< 100Hz */
  LIS2DW12_ODR_200_HZ       = 0x6u,   /**< 200Hz */
  LIS2DW12_ODR_400_HZ       = 0x7u,   /**< HR en 400Hz, LP en 200Hz */
  LIS2DW12_ODR_800_HZ       = 0x8u,   /**< HR en 800Hz, LP en 200Hz */
  LIS2DW12_ODR_1600_HZ      = 0x9u,   /**< HR en 1600Hz, LP en 200Hz */
  LIS2DW12_ODR_DEFAULT      = 0x6u    /**< 200Hz por defecto*/
} lis2dw12_odr_t;

/**
 *  @brief Tipo para la seleccion del modo de operacion
 */
typedef enum {
  LIS2DW12_MODE_LOW_PWR     = 0x0u,   /**< Modo bajo consumo (LP 12/14-bit) */
  LIS2DW12_MODE_HI_PF       = 0x1u,   /**< Modo alto rendimiento (HR 14-bit) */
  LIS2DW12_MODE_ON_DM       = 0x2u,   /**< Modo bajo demanda (12/14-bit) */
  LIS2DW12_MODE_DEFAULT     = 0x1u    /**< Modo alto rendimiento (14-bit) por defecto */
} lis2dw12_mode_t;

/**
 *  @brief Tipo para la seleccion del modo de bajo consumo
 */
typedef enum {
  LIS2DW12_LP_MODE_1         = 0x0u,  /**< Modo bajo consumo 1 (12-bit) */
  LIS2DW12_LP_MODE_2         = 0x1u,  /**< Modo bajo consumo 2 (14-bit) */
  LIS2DW12_LP_MODE_3         = 0x2u,  /**< Modo bajo consumo 3 (14-bit) */
  LIS2DW12_LP_MODE_4         = 0x3u,  /**< Modo bajo consumo 4 (14-bit) */
  LIS2DW12_LP_MODE_DEFAULT   = 0x1u   /**< Modo bajo consumo 2 (14-bit) por defecto */
} lis2dw12_lpmode_t;

/**
 *  @brief Tipo para la seleccion del ancho de banda
 */
typedef enum {
  LIS2DW12_BW_FILT_1         = 0x0u,  /**< Frecuencia de corte en ODR/2 */
  LIS2DW12_BW_FILT_2         = 0x1u,  /**< Frecuencia de corte en ODR/4 */
  LIS2DW12_BW_FILT_3         = 0x2u,  /**< Frecuencia de corte en ODR/10 */
  LIS2DW12_BW_FILT_4         = 0x3u,  /**< Frecuencia de corte en ODR/20 */
  LIS2DW12_BW_FILT_DEFAULT   = 0x1u   /**< Frecuencia de corte en ODR/4 por defecto */
} lis2dw12_bwfilt_t;

/**
 *  @brief Tipo para la seleccion de la escala completa
 */
typedef enum {
  LIS2DW12_FS_2_G            = 0x0u,  /**< Escala completa de ±2 */
  LIS2DW12_FS_4_G            = 0x1u,  /**< Escala completa de ±4 */
  LIS2DW12_FS_8_G            = 0x2u,  /**< Escala completa de ±8 */
  LIS2DW12_FS_16_G           = 0x3u,  /**< Escala completa de ±16 */
  LIS2DW12_FS_DEFAULT        = 0x1u   /**< Escala completa de ±4 por defecto */
} lis2dw12_fs_t;

/**
 *  @brief Tipo para la seleccion del tipo de filtro de datos
 */
typedef enum {
  LIS2DW12_FDS_LP_FILT       = 0x0u,  /**< Filtro paso bajo */
  LIS2DW12_FDS_HP_FILT       = 0x1u,  /**< Filtro paso alto */
  LIS2DW12_FDS_DEFAULT       = 0x0u   /**< Filtro paso bajo por defecto */
} lis2dw12_fds_t;

/**
 *  @brief Tipo para la seleccion del modo de ruido
 */
typedef enum {
  LIS2DW12_LOW_NOISE_OFF     = 0x0u,  /**< Modo de ruido bajo desactivado */
  LIS2DW12_LOW_NOISE_ON      = 0x1u,  /**< Modo de ruido bajo activado */
  LIS2DW12_LOW_NOISE_DEFAULT = 0x0u   /**< Modo de ruido bajo desactivado por defecto */
} lis2dw12_lownoise_t;

/**
 *  @brief Tipo para la seleccion del modo FIFO
 */
typedef enum {
  LIS2DW12_FIFO_MODE_OFF     = 0x0u,  /**< Modo FIFO desactivado */
  LIS2DW12_FIFO_MODE_ON      = 0x1u,  /**< Modo FIFO activado */
  LIS2DW12_FIFO_MODE_DEFAULT = 0x0u   /**< Modo FIFO desactivado por defecto */
} lis2dw12_fifomode_t;

/**
 *  @brief Estructura de configuracion del acelerometro
 */
struct _lis2dw12_config  {
  /**
    * @name Configuracion correspondiente al registro CTRL1
    * @see  lis2dw12_odr_t, lis2dw12_mode_t, lis2dw12_lpmode_t
    */
  lis2dw12_odr_t odr;             /**< Frecuencia de muestreo (output data rate) */
  lis2dw12_mode_t mode;           /**< Modo de operacion (operation mode) */
  lis2dw12_lpmode_t lp_mode;      /**< Modo de bajo consumo (low power mode)  */
  /**
    * @name Configuracion correspondiente al registro CTRL6
    * @see  lis2dw12_bwfilt_t, lis2dw12_fs_t, lis2dw12_fds_t, lis2dw12_lownoise_t
    */
  lis2dw12_bwfilt_t bw_filt;      /**< Ancho de banda (bandwith filter) */
  lis2dw12_fs_t fs;               /**< Escala completa (full-scale) */
  lis2dw12_fds_t fds;             /**< Filtro de datos (filtered data) */
  lis2dw12_lownoise_t low_noise;  /**< Bajo ruido (low noise) */
  /**
    * @name Configuracion correspondiente al registro FIFO_CTRL
    */
  lis2dw12_fifomode_t fifo_mode;
  uint8_t fifo_ths;
};

typedef struct _lis2dw12_config lis2dw12_config_t;

/* Public define ---------------------------------------------------------------------------------*/

/* Public macro ----------------------------------------------------------------------------------*/

/* Public variables ------------------------------------------------------------------------------*/

/* Functions prototypes --------------------------------------------------------------------------*/
/**************************************************************************************************
 *  @brief Funcion para la inicializacion del driver del acelerometro.
 *
 *  @details    Esta funcion espera por parametro una estructura @ref _lis2dw12_config.
 *              Los miembros de la estructura definen la configuracion del acelerometro.
 *              La estructura debe definirse en el programa que llame a esta funcion.
 *
 *  @see        _lis2dw12_config
 *  @return     LIS2DW12_I2C_OK si la inicializacion fue exitosa, de lo contrario LIS2DW12_I2C_NOK.
 **************************************************************************************************/
lis2dw12_status_t lis2dw12_init(lis2dw12_config_t usr_config);

/**************************************************************************************************
 *  @brief Funcion para la leer la configuracion actual del acelerometro.
 *
 *  @details    Esta funcion devuelve por parametro una estructura @ref _lis2dw12_config.
 *              Los miembros de la estructura contendran la configuracion del acelerometro.
 *
 *  @see        _lis2dw12_config
 *  @return     LIS2DW12_I2C_OK si la lectura fue exitosa, de lo contrario LIS2DW12_I2C_NOK.
 **************************************************************************************************/
lis2dw12_status_t lis2dw12_get_config(lis2dw12_config_t *current_config);

/**************************************************************************************************
 *  @brief Funcion para la lectura del ID del acelerometro.
 *
 *  @details    Esta funcion devuelve por parametro un byte correspondiente al ID.
 *              Siempre devuelve el mismo valor, que corresponde a 0x44.
 * 
 *  @param[in]  id  Puntero al byte que alojara el ID.
 *  @return     LIS2DW12_I2C_OK si la lectura fue exitosa, de lo contrario LIS2DW12_I2C_NOK.
 **************************************************************************************************/
lis2dw12_status_t lis2dw12_get_id(uint8_t *id);

/**************************************************************************************************
 *  @brief Funcion que indica que existe una medicion lista en cada eje del acelerometro.
 *
 *  @details    Esta funcion esta pensada para usarse en modo encuesta (polling).
 * 
 *  @return     LIS2DW12_I2C_OK si hay datos listos, de lo contrario LIS2DW12_I2C_NOK.
 **************************************************************************************************/
lis2dw12_status_t lis2dw12_is_data_ready(void);

/**************************************************************************************************
 *  @brief Funcion que indica que el bufer FIFO ha alcanzado la marca de agua.
 *
 *  @details    Esta funcion esta pensada para usarse en modo encuesta (polling).
 *              La marca de agua la determina @ref _lis2dw12_config.fifo_ths definida en la 
 *              configuracion.
 * 
 *  @return     LIS2DW12_I2C_OK si el bufer FIFO esta listo, de lo contrario LIS2DW12_I2C_NOK.
 **************************************************************************************************/
lis2dw12_status_t lis2dw12_is_fifo_ready(void);

/**************************************************************************************************
 *  @brief Funcion que indica que el bufer FIFO esta lleno.
 *
 *  @details    Esta funcion esta pensada para usarse en modo encuesta (polling).
 *              Cuando el bufer FIFO esté lleno, se detendra la lectura de los datos hasta que
 *              sean leídos con @ref lis2dw12_get_fifo_mg .
 * 
 *  @return     LIS2DW12_I2C_OK si el bufer esta lleno, de lo contrario LIS2DW12_I2C_NOK.
 **************************************************************************************************/
lis2dw12_status_t lis2dw12_is_fifo_full(void);

/**************************************************************************************************
 *  @brief Funcion que devuelve la ultima medicion generada en el eje X del acelerometro.
 *
 *  @details    Esta funcion esta pensada para usarse en modo encuesta (polling).
 *              Se recomienda llamar a esta funcion haciendo encuesta de @ref lis2dw12_is_data_ready.
 *              Retorna la aceleracion en mili-g, donde 1g (1000 mg) es igual a la aceleracion 
 *              de la gravedad.
 * 
 *  @return     Aceleracion del eje X en mg.
 **************************************************************************************************/
int16_t lis2dw12_get_mg_x(void);

/**************************************************************************************************
 *  @brief Funcion que devuelve la ultima medicion generada en el eje Y del acelerometro.
 *
 *  @details    Esta funcion esta pensada para usarse en modo encuesta (polling).
 *              Se recomienda llamar a esta funcion haciendo encuesta de @ref lis2dw12_is_data_ready.
 *              Retorna la aceleracion en mili-g, donde 1g (1000 mg) es igual a la aceleracion 
 *              de la gravedad.
 * 
 *  @return     Aceleracion del eje Y en mg.
 **************************************************************************************************/
int16_t lis2dw12_get_mg_y(void);

/**************************************************************************************************
 *  @brief Funcion que devuelve la ultima medicion generada en el eje Z del acelerometro.
 *
 *  @details    Esta funcion esta pensada para usarse en modo encuesta (polling).
 *              Se recomienda llamar a esta funcion haciendo encuesta de @ref lis2dw12_is_data_ready.
 *              Retorna la aceleracion en mili-g, donde 1g (1000 mg) es igual a la aceleracion 
 *              de la gravedad.
 * 
 *  @return     Aceleracion del eje Z en mg.
 **************************************************************************************************/
int16_t lis2dw12_get_mg_z(void);

/**************************************************************************************************
 *  @brief Funcion que devuelve las muestras de los tres ejes contenidas en el bufer FIFO.
 *
 *  @details    Esta funcion esta pensada para usarse en modo encuesta (polling).
 *              Se recomienda llamar a esta funcion haciendo encuesta de @ref lis2dw12_is_fifo_ready 
 *              o @ref lis2dw12_is_fifo_full.
 *              El numero de muestras retornadas dependeran del nivel FIFO seleccionado con 
 *              @ref _lis2dw12_config.fifo_ths en la configuracion.
 *              El numero maximo de muestras corresponde al maximo nivel FIFO (32).
 *              Retorna por parametro la aceleracion en mili-g de cada eje, donde 1g (1000 mg) 
 *              es igual a la aceleracion de la gravedad.
 * 
 *  @param[out] buffer_x    Puntero al bufer que alojara las muestras del eje X.
 *  @param[out] buffer_y    Puntero al bufer que alojara las muestras del eje Y.
 *  @param[out] buffer_z    Puntero al bufer que alojara las muestras del eje Z.
 *  @param[in]  buffer_len  Tamaño de los tres buferes, que debe ser mayor o igual al numero de muestras
 *  @return     LIS2DW12_I2C_OK si la lectura fue exitosa, de lo contrario LIS2DW12_I2C_NOK.
 **************************************************************************************************/
lis2dw12_status_t lis2dw12_get_fifo_mg(int16_t *buffer_x, int16_t *buffer_y, int16_t *buffer_z, uint8_t buffer_len);


#endif /* LIS2DW12_H_ */
