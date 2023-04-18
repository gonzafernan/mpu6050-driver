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

#include <assert.h>
#include <stdbool.h>

/* Private define ------------------------------------------------------------*/
#define MPU9250_WHO_AM_I_DEFAULT    0x70

static MPU9250_HandleTypeDef hmpu1;
static bool RxFlag = false;

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
 * @brief   Write MPU9250 register
 * @retval  MPU9250 status
*/
MPU9250_StatusTypeDef MPU9250_Reg_Write(uint8_t RegAddress, uint8_t* pData)
{
    /* MPU9250 register write wrapper */
    return I2C_Reg_Write(&hmpu1, RegAddress, pData);
}

MPU9250_StatusTypeDef MPU9250_NonBlocking_Read(uint8_t RegAddress, uint8_t* pData, uint16_t DataAmount)
{
    /* MPU9250 non-blocking register read wrapper */
    return I2C_Read_DMA(&hmpu1, RegAddress, pData, DataAmount);
}

/**
 * @brief MPU9250 callback for data ready
*/
void MPU9250_RxCallback(void)
{
    RxFlag = true;
}

bool MPU9250_IsDataReady(void)
{
    if (RxFlag)
    {
        RxFlag = false;
        return true;
    }
    return false;
}

/**
 * @brief   Initialize MPU9250 device
 * @retval  MPU9250 status
*/
MPU9250_StatusTypeDef MPU9250_Init(void)
{
    /* I2C initialization */
    hmpu1.Address = MPU9250_I2C_ADDRESS_1;
    hmpu1.I2C_Timeout = 100;
    if (I2C_Init(&hmpu1) != MPU9250_OK) return MPU9250_ERROR;
    return MPU9250_OK;
}

/**
 * @brief Gyro Full Scale Selection
*/
MPU9250_StatusTypeDef MPU9250_GyroReadConfig(uint8_t* gyroConfig)
{
    if (MPU9250_Reg_Read(MPU9250_GYRO_CONFIG, gyroConfig) != MPU9250_OK) return MPU9250_ERROR;
    return MPU9250_OK;
}

/**
 * @brief Set Guro Full Scale
*/
MPU9250_StatusTypeDef MPU9250_GyroSetFullScale(MPU9250_GyroConfig_FSTypeDef gyroFullScale)
{
    uint8_t reg_value;
    if (MPU9250_GyroReadConfig(&reg_value) != MPU9250_OK) return MPU9250_ERROR;

    reg_value &= ~(0b11 << MPU9250_GYRO_FS_SEL_OFFSET);
    reg_value |= (gyroFullScale << MPU9250_GYRO_FS_SEL_OFFSET);
    if (MPU9250_Reg_Write(MPU9250_GYRO_CONFIG, &reg_value) != MPU9250_OK) return MPU9250_ERROR;
    return MPU9250_OK;
}

/**
 * @brief Accelerometer Measurements
*/
MPU9250_StatusTypeDef MPU9250_AccelReadRaw(uint16_t* AccelX, uint16_t* AccelY, uint16_t* AccelZ)
{
    uint8_t reg_value[6];
    if (MPU9250_Burst_Read(MPU9250_ACCEL_XOUT_H, reg_value, 6) != MPU9250_OK) return MPU9250_ERROR;

    *AccelX = (reg_value[0] << 8) | reg_value[1];
    *AccelY = (reg_value[2] << 8) | reg_value[3];
    *AccelZ = (reg_value[4] << 8) | reg_value[5];

    return MPU9250_OK;
}

/**
 * @brief Temperature Measurements
*/
MPU9250_StatusTypeDef MPU9250_TempRead(uint8_t* pTemp)
{
    if (MPU9250_NonBlocking_Read(MPU9250_TEMP_OUT_H, pTemp, 2) != MPU9250_OK) return MPU9250_ERROR;
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
