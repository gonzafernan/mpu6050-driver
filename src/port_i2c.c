/**
 ******************************************************************************
 * @file           : port_i2c.c
 * @author         : Gonzalo Gabriel Fernandez
 * @brief          : MPU6050 Driver I2C port for STM32F1XX
 ******************************************************************************
 * @attention
 *
 * MPU6050 Driver I2C port for STM32F1XX.
 * Tested on STM32F103C8T6.
 *
 ******************************************************************************
 */

#include <assert.h>

#ifdef STM32F103xB
#include "stm32f1xx_hal.h"
#elif STM32F429xx
#include "stm32f4xx_hal.h"
#endif

#include "mpu6050.h"
#include "port_i2c.h"

extern I2C_HandleTypeDef hi2c1;

/**
 * @brief I2C init function
 * @retval mpu6050_status_t
 */
mpu6050_status_t i2c_init(void) { return MPU6050_OK; }

/**
 * @brief I2C read register
 * @param slave_address: I2C slave address
 * @param reg_address: Address of register to read
 * @param pdata: Pointer to buffer where the register value will be stored
 * @retval mpu6050_status_t
 */
mpu6050_status_t i2c_reg_read(uint16_t slave_address, uint8_t reg_address, uint8_t *pdata) {
    if (HAL_I2C_Mem_Read(&hi2c1, slave_address, reg_address, sizeof(uint8_t), pdata,
                         sizeof(uint8_t), I2C_READ_TIMEOUT) != HAL_OK)
        return MPU6050_ERROR;
    return MPU6050_OK;
}

/**
 * @brief I2C burst read
 * @note Read multiple registers in burst mode with I2C
 * @param slave_address: I2C slave address
 * @param reg_address: Addres of first register to read
 * @param pdata: Pointer to buffer where the data received is stored
 * @param data_amount: Amount of data to read
 * @retval mpu6050_status_t
 */
mpu6050_status_t i2c_burst_read(uint16_t slave_address, uint8_t reg_address, uint8_t *pdata,
                                uint16_t data_amont) {
    if (HAL_I2C_Mem_Read(&hi2c1, slave_address, reg_address, sizeof(uint8_t), pdata,
                         sizeof(uint8_t) * data_amont, I2C_READ_TIMEOUT) != HAL_OK)
        return MPU6050_ERROR;
    return MPU6050_OK;
}

/**
 * @brief I2C write register
 * @param slave_address: I2C slave address
 * @param reg_address: Addres of register to write
 * @param pdata: Pointer to buffer with value to write
 * @retval mpu6050_status_t
 */
mpu6050_status_t i2c_reg_write(uint16_t slave_address, uint8_t reg_address, uint8_t *pdata) {
    if (HAL_I2C_Mem_Write(&hi2c1, slave_address, reg_address, sizeof(uint8_t), pdata,
                          sizeof(uint8_t), I2C_WRITE_TIMEOUT) != HAL_OK)
        return MPU6050_ERROR;
    return MPU6050_OK;
}

/**
 * @brief I2C non-blocking read through DMA
 * @param slave_address: I2C slave address
 * @param reg_address: Addres of register to write
 * @param pdata: Pointer to buffer with value to write
 * @param data_amount: Amount of data to write
 * @retval mpu6050_status_t
 */
mpu6050_status_t i2c_read_dma(uint16_t slave_address, uint8_t reg_address, uint8_t *pdata,
                              uint16_t data_amount) {
    if (HAL_I2C_Mem_Read_DMA(&hi2c1, slave_address, reg_address, sizeof(uint8_t), pdata,
                             data_amount) != HAL_OK)
        return MPU6050_ERROR;
    return MPU6050_OK;
}

/**
 * @brief I2C Rx completed callback
 */
void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c) { mpu6050_rxcallback(); }
