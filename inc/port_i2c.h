/**
 ******************************************************************************
 * @file           : port_i2c.h
 * @author         : Gonzalo Gabriel Fernandez
 * @brief          : MPU6050 Driver I2C port header
 ******************************************************************************
 * @attention
 *
 * MPU6050 Driver I2C port header, hardware independant.
 *
 ******************************************************************************
 */

#ifndef __PORT_I2C_H
#define __PORT_I2C_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "mpu6050_def.h"

#define I2C_READ_TIMEOUT 100
#define I2C_WRITE_TIMEOUT 100

mpu6050_status_t i2c_init(void *hi2c);

mpu6050_status_t i2c_reg_read(void *hi2c, uint16_t slave_address, uint8_t reg_address,
                              uint8_t *pdata);
mpu6050_status_t i2c_burst_read(void *hi2c, uint16_t slave_address, uint8_t reg_address,
                                uint8_t *pdata, uint16_t data_amont);
mpu6050_status_t i2c_reg_write(void *hi2c, uint16_t slave_address, uint8_t reg_address,
                               uint8_t *pdata);
mpu6050_status_t i2c_read_dma(void *hi2c, uint16_t slave_address, uint8_t reg_address,
                              uint8_t *pdata, uint16_t data_amount);

#ifdef __cplusplus
}
#endif

#endif /* __PORT_I2C_H */
