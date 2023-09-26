/**
 ******************************************************************************
 * @file           : mpu6050_def.h
 * @author         : Gonzalo Gabriel Fernandez
 * @brief          : MPU6050 types definition
 ******************************************************************************
 * @note
 *
 * MPU6050 types and structures definition.
 *
 ******************************************************************************
 */

#ifndef __MPU6050_DEF_H
#define __MPU6050_DEF_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 * @brief MPU6050 Status structure definition
 */
typedef enum {
    MPU6050_OK = 0x00U,
    MPU6050_ERROR = 0x01U

} mpu6050_status_t;

/**
 * @brief   MPU6050 I2C slave address
 * @note    The slave address of the MPU6050 is 0b110100X which is 7 bits long.
 * The LSB bit of the 7 bit address is determined by the logic level on pin AD0.
 */
typedef enum {
    MPU6050_I2C_ADDRESS_1 = 0x68U, /*!< I2C adress without AD0 */
    MPU6050_I2C_ADDRESS_2 = 0x69U, /*!< I2C adress with AD0    */

} mpu6050_i2c_address_t;

/**
 * @brief MPU6050 handle structure definition
 */
typedef struct {
    mpu6050_i2c_address_t address; /*!< I2C slave address  */
    uint32_t i2c_timeout;          /*!< I2C timeout        */

} mpu6050_t;

#ifdef __cplusplus
}
#endif

#endif /* __MPU6050_DEF_H */
