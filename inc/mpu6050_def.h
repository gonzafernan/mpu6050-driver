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

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Exported types ------------------------------------------------------------*/
/**
 * @brief MPU6050 Status structure definition
*/
typedef enum
{
    MPU6050_OK      = 0x00U,
    MPU6050_ERROR   = 0x01U

} MPU6050_StatusTypeDef;

/**
 * @brief   MPU6050 I2C slave address
 * @note    The slave address of the MPU6050 is 0b110100X which is 7 bits long.
 * The LSB bit of the 7 bit address is determined by the logic level on pin AD0.
*/
typedef enum
{
    MPU6050_I2C_ADDRESS_1   = 0x68U,    /*!< I2C adress without AD0 */
    MPU6050_I2C_ADDRESS_2   = 0x69U,    /*!< I2C adress with AD0    */

} MPU6050_I2C_AddressTypeDef;

/**
 * @brief MPU6050 handle structure definition
*/
typedef struct __MPU6050_HandleTypeDef
{
    MPU6050_I2C_AddressTypeDef  Address;        /*!< I2C slave address  */
    uint32_t                    I2C_Timeout;    /*!< I2C timeout        */

} MPU6050_HandleTypeDef;

#ifdef __cplusplus
}
#endif

#endif /* __MPU6050_DEF_H */