/**
  ******************************************************************************
  * @file           : mpu6050.h
  * @author         : Gonzalo Gabriel Fernandez
  * @brief          : MPU6050 Driver headers
  ******************************************************************************
  * @attention
  *
  * MPU6050 C Driver public functions and variables.
  *
  ******************************************************************************
  */

#ifndef __MPU6050_H
#define __MPU6050_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes -------------------------------------------------------------------*/
#include "mpu6050_def.h"
#include <stdbool.h>

/* Defines --------------------------------------------------------------------*/
#define MPU6050_WHO_AM_I_DEFAULT    0x68U   /*! Who Am I default value */

/* Exported types ------------------------------------------------------------*/
/**
 * @brief MPU6050 Gyro Full Scale Select
*/
typedef enum
{
    MPU6050_GYRO_CONFIG_250DPS  = 0b00U,
    MPU6050_GYRO_CONFIG_500DPS  = 0b01U,
    MPU6050_GYRO_CONFIG_1000DPS = 0b10U,
    MPU6050_GYRO_CONFIG_2000DPS = 0b11U,

} MPU6050_GyroConfig_FSTypeDef;

/**
 * @brief MPU6050 Accel Full Scale Select
*/
typedef enum
{
    MPU6050_ACCEL_CONFIG_2G     = 0b00U,
    MPU6050_ACCEL_CONFIG_4G     = 0b01U,
    MPU6050_ACCEL_CONFIG_8G     = 0b10U,
    MPU6050_ACCEL_CONFIG_16G    = 0b11U,

} MPU6050_AccelConfig_FSTypeDef;


/* Exported functions ---------------------------------------------------------*/
MPU6050_StatusTypeDef MPU6050_Init(void);
MPU6050_StatusTypeDef MPU6050_SanityCheck(void);
MPU6050_StatusTypeDef MPU6050_ReadPwrMgmt(uint8_t* pPwrMgmt);
MPU6050_StatusTypeDef MPU6050_ResetPwrMgmt(void);
MPU6050_StatusTypeDef MPU6050_GyroReadConfig(uint8_t* pGyroConfig);
MPU6050_StatusTypeDef MPU6050_AccelReadConfig(uint8_t* pAccelConfig);
MPU6050_StatusTypeDef MPU6050_GyroSetFullScale(MPU6050_GyroConfig_FSTypeDef GyroFullScale);
MPU6050_StatusTypeDef MPU6050_GyroReadRaw(uint16_t* pGyroX, uint16_t* pGyroY, uint16_t* pGyroZ);
MPU6050_StatusTypeDef MPU6050_GyroFetch(void);
MPU6050_StatusTypeDef MPU6050_GyroReadFromBuffer(uint16_t* pGyroX, uint16_t* pGyroY, uint16_t* pGyroZ);
MPU6050_StatusTypeDef MPU6050_AccelReadRaw(uint16_t* pAccelX, uint16_t* pAccelY, uint16_t* pAccelZ);
MPU6050_StatusTypeDef MPU6050_AccelFetch(void);
MPU6050_StatusTypeDef MPU6050_AccelReadFromBuffer(uint16_t* pAccelX, uint16_t* pAccelY, uint16_t* pAccelZ);
MPU6050_StatusTypeDef MPU6050_TempReadRaw(uint16_t* pTemp);
MPU6050_StatusTypeDef MPU6050_TempFetch(void);
MPU6050_StatusTypeDef MPU6050_TempReadFromBuffer(uint16_t* pTemp);
void MPU6050_RxCallback(void);
bool MPU6050_IsDataReady(void);

#ifdef __cplusplus
}
#endif

#endif /* __MPU9250_H */