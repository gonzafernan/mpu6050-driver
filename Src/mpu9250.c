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
#define MPU9250_WHO_AM_I_DEFAULT    0x70    /*! Who Am I default value (read with logic analyzer) */

/* Private variables ---------------------------------------------------------*/
static MPU9250_HandleTypeDef hmpu1;         /*! MPU9250 data structure */
static bool DataReadyFlag = false;          /*! Flag for data ready */

/* Exported functions ---------------------------------------------------------*/
/**
 * @brief   Read MPU9250 register
 * @param   RegAddress: Address of register to read
 * @param   pData: Pointer to buffer where value will be stored
 * @retval  MPU9250_StatusTypeDef
*/
MPU9250_StatusTypeDef MPU9250_Reg_Read(uint8_t RegAddress, uint8_t* pData)
{
    /* MPU9250 register read wrapper */
    return I2C_Reg_Read((uint16_t)hmpu1.Address << 1, RegAddress, pData);
}

/**
 * @brief   Burst read MPU9250 registers
 * @param   RegAddress: Address of first register to read
 * @param   pData: Pointer to buffer where data will be stored
 * @param   DataAmount: Amount of data to read
 * @retval  MPU9250_StatusTypeDef
*/
MPU9250_StatusTypeDef MPU9250_Burst_Read(uint8_t RegAddress, uint8_t* pData, uint16_t DataAmount)
{
    /* MPU9250 register read wrapper */
    return I2C_Burst_Read((uint16_t)hmpu1.Address << 1, RegAddress, pData, DataAmount);
}

/**
 * @brief   Write MPU9250 register
 * @param   RegAddress: Address of register to write
 * @param   pData: Pointer to buffer with value to write
 * @retval  MPU9250_StatusTypeDef
*/
MPU9250_StatusTypeDef MPU9250_Reg_Write(uint8_t RegAddress, uint8_t* pData)
{
    /* MPU9250 register write wrapper */
    return I2C_Reg_Write((uint16_t)hmpu1.Address << 1, RegAddress, pData);
}

MPU9250_StatusTypeDef MPU9250_NonBlocking_Read(uint8_t RegAddress, uint8_t* pData, uint16_t DataAmount)
{
    /* MPU9250 non-blocking register read wrapper */
    return I2C_Read_DMA((uint16_t)hmpu1.Address << 1, RegAddress, pData, DataAmount);
}

/**
 * @brief   MPU9250 callback for data ready
*/
void MPU9250_RxCallback(void)
{
    DataReadyFlag = true;
}

/**
 * @brief   MPU9250 check for data ready
 * @retval  bool 
*/
bool MPU9250_IsDataReady(void)
{
    if (DataReadyFlag)
    {
        DataReadyFlag = false;
        return true;
    }
    return false;
}

/**
 * @brief   Initialize MPU9250 device
 * @retval  MPU9250_StatusTypeDef
*/
MPU9250_StatusTypeDef MPU9250_Init(void)
{
    /* I2C initialization */
    hmpu1.Address = MPU9250_I2C_ADDRESS_1;
    hmpu1.I2C_Timeout = 100;
    if (I2C_Init() != MPU9250_OK) return MPU9250_ERROR;
    return MPU9250_OK;
}

/**
 * @brief   Read current Gyro configuration
 * @param   pGyroConfig: Pointer to buffer where configuration will be stored   
 * @retval  MPU9250_StatusTypeDef
*/
MPU9250_StatusTypeDef MPU9250_GyroReadConfig(uint8_t* pGyroConfig)
{
    if (MPU9250_Reg_Read(MPU9250_GYRO_CONFIG, pGyroConfig) != MPU9250_OK) return MPU9250_ERROR;
    return MPU9250_OK;
}

/**
 * @brief   Gyro Full Scale selection
 * @param   GyroFullScale: Gyro Full Scale to set
 * @retval  MPU9250_StatusTypeDef
*/
MPU9250_StatusTypeDef MPU9250_GyroSetFullScale(MPU9250_GyroConfig_FSTypeDef GyroFullScale)
{
    uint8_t reg_value;
    if (MPU9250_GyroReadConfig(&reg_value) != MPU9250_OK) return MPU9250_ERROR;

    reg_value &= ~(0b11 << MPU9250_GYRO_FS_SEL_OFFSET);
    reg_value |= (GyroFullScale << MPU9250_GYRO_FS_SEL_OFFSET);
    if (MPU9250_Reg_Write(MPU9250_GYRO_CONFIG, &reg_value) != MPU9250_OK) return MPU9250_ERROR;
    return MPU9250_OK;
}

/**
 * @brief   Raw Gyroscope Measurements
 * @param   pGyroX: Pointer to buffer where Gyro X-axis measurement will be stored
 * @param   pGyroY: Pointer to buffer where Gyro Y-axis measurement will be stored
 * @param   pGyroZ: Pointer to buffer where Gyro Z-axis measurement will be stored
 * @retval  MPU9250_StatusTypeDef
*/
MPU9250_StatusTypeDef MPU9250_GyroReadRaw(uint16_t* pGyroX, uint16_t* pGyroY, uint16_t* pGyroZ)
{
    uint8_t reg_value[6];
    if (MPU9250_Burst_Read(MPU9250_GYRO_XOUT_H, reg_value, 6) != MPU9250_OK) return MPU9250_ERROR;

    *pGyroX = (reg_value[0] << 8) | reg_value[1];
    *pGyroY = (reg_value[2] << 8) | reg_value[3];
    *pGyroZ = (reg_value[4] << 8) | reg_value[5];

    return MPU9250_OK;
}

/**
 * @brief   Raw Accelerometer Measurements
 * @param   pAccelX: Pointer to buffer where Accel X-axis measurement will be stored
 * @param   pAccelY: Pointer to buffer where Accel Y-axis measurement will be stored
 * @param   pAccelZ: Pointer to buffer where Accel Z-axis measurement will be stored
 * @retval  MPU9250_StatusTypeDef
*/
MPU9250_StatusTypeDef MPU9250_AccelReadRaw(uint16_t* pAccelX, uint16_t* pAccelY, uint16_t* pAccelZ)
{
    uint8_t reg_value[6];
    if (MPU9250_Burst_Read(MPU9250_ACCEL_XOUT_H, reg_value, 6) != MPU9250_OK) return MPU9250_ERROR;

    *pAccelX = (reg_value[0] << 8) | reg_value[1];
    *pAccelY = (reg_value[2] << 8) | reg_value[3];
    *pAccelZ = (reg_value[4] << 8) | reg_value[5];

    return MPU9250_OK;
}

/**
 * @brief   Temperature Measurement
 * @param   pTemp: Pointer to buffer where temperature measurement will be stored
 * @retval  MPU9250_StatusTypeDef
*/
MPU9250_StatusTypeDef MPU9250_TempReadRaw(uint8_t* pTemp)
{
    if (MPU9250_NonBlocking_Read(MPU9250_TEMP_OUT_H, pTemp, 2) != MPU9250_OK) return MPU9250_ERROR;
    return MPU9250_OK;
}



/**
 * @brief   MPU9250 Sanity Check
 * @note    It performs a who am I to verify the I2C slave
 * @retval  MPU9250_StatusTypeDef
*/
MPU9250_StatusTypeDef MPU9250_SanityCheck(void)
{
    /* Who Am I check*/
    uint8_t reg_value;
    if (MPU9250_Reg_Read(MPU9250_WHO_AM_I, &reg_value) != MPU9250_OK) return MPU9250_ERROR;
    if (reg_value != MPU9250_WHO_AM_I_DEFAULT) return MPU9250_ERROR;

    return MPU9250_OK;
}
