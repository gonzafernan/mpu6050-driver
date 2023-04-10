/**
  ******************************************************************************
  * @file           : mpu9250.c
  * @author         : Gonzalo Gabriel Fernandez
  * @brief          : MPU9250 Driver
  ******************************************************************************
  * @attention
  *
  * MPU9250 C Driver private and public functions and variables.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "mpu9250.h"
#include "mpu9250_registers.h"
#include "port_i2c.h"

/* Private define ------------------------------------------------------------*/
#define MPU9250_WHO_AM_I_DEFAULT    0x70

/* Exported functions ---------------------------------------------------------*/
/**
 * @brief   Read MPU9250 register
 * @retval  MPU9250 status
*/
MPU9250_StatusTypeDef MPU9250_Reg_Read(MPU9250_HandleTypeDef* hmpu, uint8_t RegAddress, uint8_t* pData)
{
    /* MPU9250 register read wrapper */
    return I2C_Reg_Read(hmpu, RegAddress, pData);
}

/**
 * @brief   Initialize MPU9250 device
 * @retval  MPU9250 status
*/
MPU9250_StatusTypeDef MPU9250_Init(MPU9250_HandleTypeDef* hmpu)
{
    /* I2C initialization */
    if (I2C_Init(hmpu) != MPU9250_OK) return MPU9250_ERROR;
    return MPU9250_OK;
}

/**
 * @brief   MPU9250 sanity check
 * @note    It performs a who am I to verify the I2C slave
 * @retval  MPU9250 status
*/
MPU9250_StatusTypeDef MPU9250_SanityCheck(MPU9250_HandleTypeDef* hmpu)
{
    /* Who Am I check*/
    uint8_t reg_value;
    if (MPU9250_Reg_Read(hmpu, MPU9250_WHO_AM_I, &reg_value) != MPU9250_OK) return MPU9250_ERROR;
    if (reg_value != MPU9250_WHO_AM_I_DEFAULT) return MPU9250_ERROR;

    return MPU9250_OK;
}