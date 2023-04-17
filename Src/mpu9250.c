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

static MPU9250_HandleTypeDef hmpu1;

/* Exported functions ---------------------------------------------------------*/
/**
 * @brief   Read MPU9250 register
 * @retval  MPU9250 status
*/
MPU9250_StatusTypeDef MPU9250_Reg_Read(uint8_t RegAddress, uint8_t* pData)
{
    /* MPU9250 register read wrapper */
    return I2C_Reg_Read(&hmpu1, RegAddress, pData);
}

/**
 * @brief   Burst read MPU9250 registers
 * @retval  MPU9250 status
*/
MPU9250_StatusTypeDef MPU9250_Burst_Read(uint8_t RegAddress, uint8_t* pData, uint16_t DataAmount)
{
    /* MPU9250 register read wrapper */
    return I2C_Burst_Read(&hmpu1, RegAddress, pData, DataAmount);
}

/**
 * @brief   Initialize MPU9250 device
 * @retval  MPU9250 status
*/
MPU9250_StatusTypeDef MPU9250_Init(void)
{
    /* I2C initialization */
    if (I2C_Init(&hmpu1) != MPU9250_OK) return MPU9250_ERROR;
    return MPU9250_OK;
}

/**
 * @brief Accelerometer Measurements
*/
MPU9250_StatusTypeDef MPU9250_AccelRead(void)
{
    uint8_t reg_value[6];
    if (MPU9250_Burst_Read(MPU9250_ACCEL_XOUT_H, reg_value, 6) != MPU9250_OK) return MPU9250_ERROR;
    return MPU9250_OK;
}

/**
 * @brief Temperature Measurements
*/
MPU9250_StatusTypeDef MPU9250_TempRead(void)
{
    uint8_t reg_value[6];
    if (MPU9250_Burst_Read(MPU9250_TEMP_OUT_H, reg_value, 2) != MPU9250_OK) return MPU9250_ERROR;
    return MPU9250_OK;
}

/**
 * @brief Gyroscope Measurements
*/
MPU9250_StatusTypeDef MPU9250_GyroRead(void)
{
    uint8_t reg_value[6];
    if (MPU9250_Burst_Read(MPU9250_GYRO_XOUT_H, reg_value, 6) != MPU9250_OK) return MPU9250_ERROR;
    return MPU9250_OK;
}

/**
 * @brief   MPU9250 sanity check
 * @note    It performs a who am I to verify the I2C slave
 * @retval  MPU9250 status
*/
MPU9250_StatusTypeDef MPU9250_SanityCheck(void)
{
    /* Who Am I check*/
    uint8_t reg_value;
    if (MPU9250_Reg_Read(MPU9250_WHO_AM_I, &reg_value) != MPU9250_OK) return MPU9250_ERROR;
    if (reg_value != MPU9250_WHO_AM_I_DEFAULT) return MPU9250_ERROR;

    return MPU9250_OK;
}
