/**
  ******************************************************************************
  * @file           : port_i2c.c
  * @author         : Gonzalo Gabriel Fernandez
  * @brief          : MPU9250 Driver I2C port for STM32F1XX
  ******************************************************************************
  * @attention
  *
  * MPU9250 Driver I2C port for STM32F1XX.
  * Tested on STM32F103C8T6.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "port_i2c.h"
#include "stm32f1xx_hal.h"

#include <assert.h>

/* Private variables ---------------------------------------------------------*/
static I2C_HandleTypeDef hi2c;

/* Exported functions ---------------------------------------------------------*/
/**
 * @brief I2C init function
*/
MPU9250_StatusTypeDef I2C_Init(MPU9250_HandleTypeDef* hmpu)
{
    hi2c.Instance = I2C1;
    hi2c.Init.ClockSpeed = 400000;
    hi2c.Init.DutyCycle = I2C_DUTYCYCLE_2;
    hi2c.Init.OwnAddress1 = 0;
    hi2c.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c.Init.OwnAddress2 = 0;
    hi2c.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

    if (HAL_I2C_Init(&hi2c) != HAL_OK) return MPU9250_ERROR;
    return MPU9250_OK;
}

/**
 * @brief I2C read register
*/
MPU9250_StatusTypeDef I2C_Reg_Read(MPU9250_HandleTypeDef* hmpu, uint8_t RegAddress, uint8_t* pData)
{
    assert(hmpu);
    if (HAL_I2C_Mem_Read(&hi2c, (uint16_t)hmpu->Address << 1, RegAddress, sizeof(uint8_t), pData, sizeof(uint8_t), hmpu->I2C_Timeout) != HAL_OK) return MPU9250_ERROR;
    return MPU9250_OK;
}

/**
 * @brief I2C burst read
 * @note Read multiple registers in burst mode with I2C
*/
MPU9250_StatusTypeDef I2C_Burst_Read(MPU9250_HandleTypeDef* hmpu, uint8_t RegAddress, uint8_t* pData, uint16_t DataAmont)
{
    assert(hmpu);
    if (HAL_I2C_Mem_Read(&hi2c, (uint16_t)hmpu->Address << 1, RegAddress, sizeof(uint8_t), pData, sizeof(uint8_t)*DataAmont, hmpu->I2C_Timeout) != HAL_OK) return MPU9250_ERROR;
    return MPU9250_OK;
}