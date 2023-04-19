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

/* Includes -------------------------------------------------------------------*/
#include "mpu9250_def.h"
#include <stdint.h>

/* Defines --------------------------------------------------------------------*/
#define I2C_READ_TIMEOUT  100
#define I2C_WRITE_TIMEOUT 100

/* Exported functions ---------------------------------------------------------*/
MPU9250_StatusTypeDef I2C_Init(void);
MPU9250_StatusTypeDef I2C_Reg_Read(uint16_t SlaveAddress, uint8_t RegAddress, uint8_t* pData);
MPU9250_StatusTypeDef I2C_Burst_Read(uint16_t SlaveAddress, uint8_t RegAddress, uint8_t* pData, uint16_t DataAmont);
MPU9250_StatusTypeDef I2C_Reg_Write(uint16_t SlaveAddress, uint8_t RegAddress, uint8_t* pData);
MPU9250_StatusTypeDef I2C_Read_DMA(uint16_t SlaveAddress, uint8_t RegAddress, uint8_t*pData, uint16_t DataAmount);

#ifdef __cplusplus
}
#endif

#endif /* __PORT_I2C_H */