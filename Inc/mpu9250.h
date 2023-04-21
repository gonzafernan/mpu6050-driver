/**
  ******************************************************************************
  * @file           : mpu9250.h
  * @author         : Gonzalo Gabriel Fernandez
  * @brief          : MPU9250 Driver headers
  ******************************************************************************
  * @attention
  *
  * MPU9250 C Driver public functions and variables.
  *
  ******************************************************************************
  */

#ifndef __MPU9250_H
#define __MPU9250_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes -------------------------------------------------------------------*/
#include "mpu9250_def.h"
#include <stdbool.h>

/* Exported types ------------------------------------------------------------*/
/**
 * @brief MPU9250 Gyro Full Scale Select
*/
typedef enum
{
    MPU9250_GYRO_CONFIG_250DPS  = 0b00U,
    MPU9250_GYRO_CONFIG_500DPS  = 0b01U,
    MPU9250_GYRO_CONFIG_1000DPS = 0b10U,
    MPU9250_GYRO_CONFIG_2000DPS = 0b11U,

} MPU9250_GyroConfig_FSTypeDef;

/**
 * @brief MPU9250 Accel Full Scale Select
*/
typedef enum
{
    MPU9250_ACCEL_CONFIG_2G     = 0b00U,
    MPU9250_ACCEL_CONFIG_4G     = 0b01U,
    MPU9250_ACCEL_CONFIG_8G     = 0b10U,
    MPU9250_ACCEL_CONFIG_16G    = 0b11U,

} MPU9250_AccelConfig_FSTypeDef;


/* Exported functions ---------------------------------------------------------*/
MPU9250_StatusTypeDef MPU9250_Init(void);
MPU9250_StatusTypeDef MPU9250_SanityCheck(void);
MPU9250_StatusTypeDef MPU9250_GyroReadConfig(uint8_t* pGyroConfig);
MPU9250_StatusTypeDef MPU9250_AccelReadConfig(uint8_t* pAccelConfig);
MPU9250_StatusTypeDef MPU9250_GyroSetFullScale(MPU9250_GyroConfig_FSTypeDef GyroFullScale);
MPU9250_StatusTypeDef MPU9250_GyroReadRaw(uint16_t* pGyroX, uint16_t* pGyroY, uint16_t* pGyroZ);
MPU9250_StatusTypeDef MPU9250_GyroFetch(void);
MPU9250_StatusTypeDef MPU9250_GyroReadFromBuffer(uint16_t* pGyroX, uint16_t* pGyroY, uint16_t* pGyroZ);
MPU9250_StatusTypeDef MPU9250_AccelReadRaw(uint16_t* pAccelX, uint16_t* pAccelY, uint16_t* pAccelZ);
MPU9250_StatusTypeDef MPU9250_AccelFetch(void);
MPU9250_StatusTypeDef MPU9250_AccelReadFromBuffer(uint16_t* pAccelX, uint16_t* pAccelY, uint16_t* pAccelZ);
MPU9250_StatusTypeDef MPU9250_TempReadRaw(uint16_t* pTemp);
MPU9250_StatusTypeDef MPU9250_TempFetch(void);
MPU9250_StatusTypeDef MPU9250_TempReadFromBuffer(uint16_t* pTemp);
void MPU9250_RxCallback(void);
bool MPU9250_IsDataReady(void);

#ifdef __cplusplus
}
#endif

#endif /* __MPU9250_H */