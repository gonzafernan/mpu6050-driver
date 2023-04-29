/**
  ******************************************************************************
  * @file           : mpu6050.c
  * @author         : Gonzalo Gabriel Fernandez
  * @brief          : MPU6050 Driver
  ******************************************************************************
  * @attention
  *
  * MPU9250 C Driver private and public functions and variables.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "mpu6050.h"
#include "mpu6050_registers.h"
#include "port_i2c.h"

#include <assert.h>
#include <stdbool.h>

/* Private define ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static MPU6050_HandleTypeDef hmpu1;         /*! MPU6050 data structure */
static bool DataReadyFlag = false;          /*! Flag for data ready */
static uint8_t RxBuffer[6];                 /*! Buffer with data from non-blocking read */

/* Exported functions ---------------------------------------------------------*/
/**
 * @brief   Read MPU9250 register
 * @param   RegAddress: Address of register to read
 * @param   pData: Pointer to buffer where value will be stored
 * @retval  MPU6050_StatusTypeDef
*/
static MPU6050_StatusTypeDef MPU6050_Reg_Read(uint8_t RegAddress, uint8_t* pData)
{
    /* MPU6050 register read wrapper */
    return I2C_Reg_Read((uint16_t)hmpu1.Address << 1, RegAddress, pData);
}

/**
 * @brief   Burst read MPU6050 registers
 * @param   RegAddress: Address of first register to read
 * @param   pData: Pointer to buffer where data will be stored
 * @param   DataAmount: Amount of data to read
 * @retval  MPU6050_StatusTypeDef
*/
static MPU6050_StatusTypeDef MPU6050_Burst_Read(uint8_t RegAddress, uint8_t* pData, uint16_t DataAmount)
{
    /* MPU6050 register read wrapper */
    return I2C_Burst_Read((uint16_t)hmpu1.Address << 1, RegAddress, pData, DataAmount);
}

/**
 * @brief   Write MPU6050 register
 * @param   RegAddress: Address of register to write
 * @param   pData: Pointer to buffer with value to write
 * @retval  MPU6050_StatusTypeDef
*/
static MPU6050_StatusTypeDef MPU6050_Reg_Write(uint8_t RegAddress, uint8_t* pData)
{
    /* MPU6050 register write wrapper */
    return I2C_Reg_Write((uint16_t)hmpu1.Address << 1, RegAddress, pData);
}

/**
 * @brief   MPU-6050 Non-blocking burst read
 * @param   RegAddress: Address of first register to read
 * @param   pData: Pointer to buffer where received data will be stored
*/
static MPU6050_StatusTypeDef MPU6050_NonBlocking_Read(uint8_t RegAddress, uint8_t* pData, uint16_t DataAmount)
{
    /* MPU6050 non-blocking register read wrapper */
    return I2C_Read_DMA((uint16_t)hmpu1.Address << 1, RegAddress, pData, DataAmount);
}

/**
 * @brief   MPU6050 callback for data ready
*/
void MPU6050_RxCallback(void)
{
    DataReadyFlag = true;
}

/**
 * @brief   MPU6050 check for data ready
 * @retval  bool 
*/
bool MPU6050_IsDataReady(void)
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
 * @retval  MPU6050_StatusTypeDef
*/
MPU6050_StatusTypeDef MPU6050_Init(void)
{
    /* I2C initialization */
    hmpu1.Address = MPU6050_I2C_ADDRESS_1;
    hmpu1.I2C_Timeout = 100;
    if (I2C_Init() != MPU6050_OK) return MPU6050_ERROR;
    return MPU6050_OK;
}

/**
 * @brief   Read MPU6050 Power Management 1
 * @param   pPwrMgmt: Pointer to buffer where configuration will be stored
*/
MPU6050_StatusTypeDef MPU6050_ReadPwrMgmt(uint8_t* pPwrMgmt)
{
    assert(pPwrMgmt);
    if (MPU6050_Reg_Read(MPU6050_PWR_MGMT_1, &pPwrMgmt[0]) != MPU6050_OK) return MPU6050_ERROR;
    if (MPU6050_Reg_Read(MPU6050_PWR_MGMT_2, &pPwrMgmt[1]) != MPU6050_OK) return MPU6050_ERROR;
    return MPU6050_OK;
}

/**
 * @brief   Read MPU6050 Reset Power Management 1
*/
MPU6050_StatusTypeDef MPU6050_ResetPwrMgmt(void)
{
    uint8_t reg_value = 0x00;
    if (MPU6050_Reg_Write(MPU6050_PWR_MGMT_1, &reg_value) != MPU6050_OK) return MPU6050_ERROR;
    return MPU6050_OK;
}

/**
 * @brief   Read current Gyro configuration
 * @param   pGyroConfig: Pointer to buffer where configuration will be stored   
 * @retval  MPU6050_StatusTypeDef
*/
MPU6050_StatusTypeDef MPU6050_GyroReadConfig(uint8_t* pGyroConfig)
{
    if (MPU6050_Reg_Read(MPU6050_GYRO_CONFIG, pGyroConfig) != MPU6050_OK) return MPU6050_ERROR;
    return MPU6050_OK;
}

/**
 * @brief   Read current Accel configuration
 * @param   pAccelConfig: Pointer to buffer where configuration will be stored   
 * @retval  MPU6050_StatusTypeDef
*/
MPU6050_StatusTypeDef MPU6050_AccelReadConfig(uint8_t* pAccelConfig)
{
    if (MPU6050_Reg_Read(MPU6050_ACCEL_CONFIG, pAccelConfig) != MPU6050_OK) return MPU6050_ERROR;
    return MPU6050_OK;
}

/**
 * @brief   Gyro Full Scale selection
 * @param   GyroFullScale: Gyro Full Scale to set
 * @retval  MPU6050_StatusTypeDef
*/
MPU6050_StatusTypeDef MPU6050_GyroSetFullScale(MPU6050_GyroConfig_FSTypeDef GyroFullScale)
{
    uint8_t reg_value;
    if (MPU6050_GyroReadConfig(&reg_value) != MPU6050_OK) return MPU6050_ERROR;

    reg_value &= ~(0b11 << MPU6050_GYRO_FS_SEL_OFFSET);
    reg_value |= (GyroFullScale << MPU6050_GYRO_FS_SEL_OFFSET);
    if (MPU6050_Reg_Write(MPU6050_GYRO_CONFIG, &reg_value) != MPU6050_OK) return MPU6050_ERROR;
    return MPU6050_OK;
}

/**
 * @brief   Accel Full Scale selection
 * @param   AccelFullScale: Accel Full Scale to set
 * @retval  MPU6050_StatusTypeDef
*/
MPU6050_StatusTypeDef MPU6050_AccelSetFullScale(MPU6050_AccelConfig_FSTypeDef AccelFullScale)
{
    uint8_t reg_value;
    if (MPU6050_AccelReadConfig(&reg_value) != MPU6050_OK) return MPU6050_ERROR;

    reg_value &= ~(0b11 << MPU6050_ACCEL_FS_SEL_OFFSET);
    reg_value |= (AccelFullScale << MPU6050_ACCEL_FS_SEL_OFFSET);
    if (MPU6050_Reg_Write(MPU6050_ACCEL_CONFIG, &reg_value) != MPU6050_OK) return MPU6050_ERROR;
    return MPU6050_OK;
}

/**
 * @brief   Raw Gyroscope Measurements
 * @param   pGyroX: Pointer to buffer where Gyro X-axis measurement will be stored
 * @param   pGyroY: Pointer to buffer where Gyro Y-axis measurement will be stored
 * @param   pGyroZ: Pointer to buffer where Gyro Z-axis measurement will be stored
 * @retval  MPU6050_StatusTypeDef
*/
MPU6050_StatusTypeDef MPU6050_GyroReadRaw(uint16_t* pGyroX, uint16_t* pGyroY, uint16_t* pGyroZ)
{
    uint8_t reg_value[6];
    if (MPU6050_Burst_Read(MPU6050_GYRO_XOUT_H, reg_value, 6) != MPU6050_OK) return MPU6050_ERROR;

    *pGyroX = (reg_value[0] << 8) | reg_value[1];
    *pGyroY = (reg_value[2] << 8) | reg_value[3];
    *pGyroZ = (reg_value[4] << 8) | reg_value[5];

    return MPU6050_OK;
}

/**
 * @brief   Fetch Gyro Measurements and load it into buffer
 * @retval  MPU6050_StatusTypeDef
*/
MPU6050_StatusTypeDef MPU6050_GyroFetch(void)
{
    if (MPU6050_NonBlocking_Read(MPU6050_GYRO_XOUT_H, RxBuffer, 6) != MPU6050_OK) return MPU6050_ERROR;
    return MPU6050_OK;
}

/**
 * @brief   Raw Gyroscope Measurements from buffer
 * @param   pGyroX: Pointer to buffer where Gyro X-axis measurement will be stored
 * @param   pGyroY: Pointer to buffer where Gyro Y-axis measurement will be stored
 * @param   pGyroZ: Pointer to buffer where Gyro Z-axis measurement will be stored
 * @retval  MPU6050_StatusTypeDef
*/
MPU6050_StatusTypeDef MPU6050_GyroReadFromBuffer(uint16_t* pGyroX, uint16_t* pGyroY, uint16_t* pGyroZ)
{
    *pGyroX = (RxBuffer[0] << 8) | RxBuffer[1];
    *pGyroY = (RxBuffer[2] << 8) | RxBuffer[3];
    *pGyroZ = (RxBuffer[4] << 8) | RxBuffer[5];

    return MPU6050_OK;
}

/**
 * @brief   Raw Accelerometer Measurements
 * @param   pAccelX: Pointer to buffer where Accel X-axis measurement will be stored
 * @param   pAccelY: Pointer to buffer where Accel Y-axis measurement will be stored
 * @param   pAccelZ: Pointer to buffer where Accel Z-axis measurement will be stored
 * @retval  MPU6050_StatusTypeDef
*/
MPU6050_StatusTypeDef MPU6050_AccelReadRaw(uint16_t* pAccelX, uint16_t* pAccelY, uint16_t* pAccelZ)
{
    uint8_t reg_value[6];
    if (MPU6050_Burst_Read(MPU6050_ACCEL_XOUT_H, reg_value, 6) != MPU6050_OK) return MPU6050_ERROR;

    *pAccelX = (reg_value[0] << 8) | reg_value[1];
    *pAccelY = (reg_value[2] << 8) | reg_value[3];
    *pAccelZ = (reg_value[4] << 8) | reg_value[5];

    return MPU6050_OK;
}

/**
 * @brief   Fetch Accelerometer Measurements and load it into buffer
 * @retval  MPU6050_StatusTypeDef
*/
MPU6050_StatusTypeDef MPU6050_AccelFetch(void)
{
    if (MPU6050_NonBlocking_Read(MPU6050_ACCEL_XOUT_H, RxBuffer, 6) != MPU6050_OK) return MPU6050_ERROR;
    return MPU6050_OK;
}

/**
 * @brief   Raw Accelerometer Measurements from buffer
 * @param   pAccelX: Pointer to buffer where Accel X-axis measurement will be stored
 * @param   pAccelY: Pointer to buffer where Accel Y-axis measurement will be stored
 * @param   pAccelZ: Pointer to buffer where Accel Z-axis measurement will be stored
 * @retval  MPU6050_StatusTypeDef
*/
MPU6050_StatusTypeDef MPU6050_AccelReadFromBuffer(uint16_t* pAccelX, uint16_t* pAccelY, uint16_t* pAccelZ)
{
    *pAccelX = (RxBuffer[0] << 8) | RxBuffer[1];
    *pAccelY = (RxBuffer[2] << 8) | RxBuffer[3];
    *pAccelZ = (RxBuffer[4] << 8) | RxBuffer[5];

    return MPU6050_OK;
}

/**
 * @brief   Temperature Measurement
 * @param   pTemp: Pointer to buffer where temperature measurement will be stored
 * @retval  MPU6050_StatusTypeDef
*/
MPU6050_StatusTypeDef MPU6050_TempReadRaw(uint16_t* pTemp)
{
    uint8_t reg_value[6];
    if (MPU6050_Burst_Read(MPU6050_TEMP_OUT_H, reg_value, 2) != MPU6050_OK) return MPU6050_ERROR;

    *pTemp = (reg_value[0] << 8) | reg_value[1];

    return MPU6050_OK;
}

/**
 * @brief   Fetch Temperature Measurements and load it into buffer
 * @retval  MPU6050_StatusTypeDef
*/
MPU6050_StatusTypeDef MPU6050_TempFetch(void)
{
    if (MPU6050_NonBlocking_Read(MPU6050_GYRO_XOUT_H, RxBuffer, 6) != MPU6050_OK) return MPU6050_ERROR;
    return MPU6050_OK;
}

/**
 * @brief   Raw Temperature Measurements from buffer
 * @param   pTemp: Pointer to buffer where Temperature measurement will be stored
 * @retval  MPU6050_StatusTypeDef
*/
MPU6050_StatusTypeDef MPU6050_TempReadFromBuffer(uint16_t* pTemp)
{
    *pTemp = (RxBuffer[0] << 8) | RxBuffer[1];

    return MPU6050_OK;
}


/**
 * @brief   MPU6050 Sanity Check
 * @note    It performs a who am I to verify the I2C slave
 * @retval  MPU6050_StatusTypeDef
*/
MPU6050_StatusTypeDef MPU6050_SanityCheck(void)
{
    /* MPU-6050 Who Am I check */
    uint8_t reg_value;
    if (MPU6050_Reg_Read(MPU6050_WHO_AM_I, &reg_value) != MPU6050_OK) return MPU6050_ERROR;
    if (reg_value != MPU6050_WHO_AM_I_DEFAULT) return MPU6050_ERROR;
    return MPU6050_OK;
}
