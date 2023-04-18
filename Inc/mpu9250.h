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

#include "mpu9250_def.h"

#include <stdbool.h>

/**
 * @brief MPU9250 Gyro Full Scale Select
*/
typedef enum
{
    MPU9250_GYRO_Config_250DPS  = 0b00U,
    MPU9250_GYRO_Config_500DPS  = 0b01U,
    MPU9250_GYRO_Config_1000DPS = 0b10U,
    MPU9250_GYRO_Config_2000DPS = 0b11U,

} MPU9250_GyroConfig_FSTypeDef;


MPU9250_StatusTypeDef MPU9250_Init(void);
MPU9250_StatusTypeDef MPU9250_SanityCheck(void);
MPU9250_StatusTypeDef MPU9250_AccelReadRaw(uint16_t* AccelX, uint16_t* AccelY, uint16_t* AccelZ);
MPU9250_StatusTypeDef MPU9250_GyroRead(void);
MPU9250_StatusTypeDef MPU9250_TempRead(uint8_t* pTemp);
void MPU9250_RxCallback(void);
bool MPU9250_IsDataReady(void);

#ifdef __cplusplus
}
#endif

#endif /* __MPU9250_H */