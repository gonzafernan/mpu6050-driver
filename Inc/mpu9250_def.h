/**
  ******************************************************************************
  * @file           : mpu9250_def.h
  * @author         : Gonzalo Gabriel Fernandez
  * @brief          : MPU9250 types definition
  ******************************************************************************
  * @attention
  *
  * MPU9250 types and structures definition.
  *
  ******************************************************************************
  */

#ifndef __MPU9250_DEF_H
#define __MPU9250_DEF_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Exported types ------------------------------------------------------------*/
/**
 * @brief MPU9250 Status structure definition
*/
typedef enum
{
    MPU9250_OK      = 0x00U,
    MPU9250_ERROR   = 0x01U

} MPU9250_StatusTypeDef;

/**
 * @brief MPU9250 I2C slave address
*/
typedef enum
{
    MPU9250_I2C_ADDRESS_1   = 0x68U,    /*!< I2C adress without AD0 */
    MPU9250_I2C_ADDRESS_2   = 0x69U,    /*!< I2C adress with AD0    */

} MPU9250_I2C_AddressTypeDef;

/**
 * @brief MPU9250 handle structure definition
*/
typedef struct __MPU9250_HandleTypeDef
{
    MPU9250_I2C_AddressTypeDef  Address;        /*!< I2C slave address  */
    uint32_t                    I2C_Timeout;    /*!< I2C timeout        */

} MPU9250_HandleTypeDef;

#ifdef __cplusplus
}
#endif

#endif /* __MPU9250_DEF_H */