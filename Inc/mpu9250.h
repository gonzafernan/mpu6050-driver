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

MPU9250_StatusTypeDef MPU9250_Init(MPU9250_HandleTypeDef* hmpu);
MPU9250_StatusTypeDef MPU9250_SanityCheck(MPU9250_HandleTypeDef* hmpu);
MPU9250_StatusTypeDef MPU9250_AccelRead(MPU9250_HandleTypeDef* hmpu);

#ifdef __cplusplus
}
#endif

#endif /* __MPU9250_H */