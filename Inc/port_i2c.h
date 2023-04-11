/**
  ******************************************************************************
  * @file           : port_i2c.h
  * @author         : Gonzalo Gabriel Fernandez
  * @brief          : MPU9250 Driver I2C port header
  ******************************************************************************
  * @attention
  *
  * MPU9250 Driver I2C port header, hardware independant.
  *
  ******************************************************************************
  */

#ifndef __PORT_I2C_H
#define __PORT_I2C_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "mpu9250_def.h"

#include <stdint.h>

/* Exported functions ---------------------------------------------------------*/
MPU9250_StatusTypeDef I2C_Init(MPU9250_HandleTypeDef* hmpu);
MPU9250_StatusTypeDef I2C_Reg_Read(MPU9250_HandleTypeDef* hmpu, uint8_t RegAddress, uint8_t* pData);
MPU9250_StatusTypeDef I2C_Burst_Read(MPU9250_HandleTypeDef* hmpu, uint8_t RegAddress, uint8_t* pData, uint16_t DataAmont);

#ifdef __cplusplus
}
#endif

#endif /* __PORT_I2C_H */