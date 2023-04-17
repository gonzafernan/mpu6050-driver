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

MPU9250_StatusTypeDef MPU9250_Init(void);
MPU9250_StatusTypeDef MPU9250_SanityCheck(void);
MPU9250_StatusTypeDef MPU9250_AccelRead(void);
MPU9250_StatusTypeDef MPU9250_GyroRead(void);
MPU9250_StatusTypeDef MPU9250_TempRead(void);

#ifdef __cplusplus
}
#endif

#endif /* __MPU9250_H */