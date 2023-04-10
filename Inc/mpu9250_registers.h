/**
  ******************************************************************************
  * @file           : mpu9250_registers.h
  * @author         : Gonzalo Gabriel Fernandez
  * @brief          : MPU9250 Registers Map
  ******************************************************************************
  * @attention
  *
  * MPU9250 Registers defines and macros.
  * Register Map Revision 1.6
  *
  ******************************************************************************
  */

#ifndef __MPU9250_REGISTERS_H
#define __MPU9250_REGISTERS_H

#ifdef __cplusplus
extern "C" {
#endif

#define MPU9250_SELF_TEST_X_GYRO    0x00U
#define MPU9250_SELF_TEST_Y_GYRO    0x01U
#define MPU9250_SELF_TEST_Z_GYRO    0x02U

#define MPU9250_SELF_TEST_X_ACCEL   0x0DU
#define MPU9250_SELF_TEST_Y_ACCEL   0x0EU
#define MPU9250_SELF_TEST_Z_ACCEL   0x0FU

#define MPU9250_CONFIG              0x1AU
#define MPU9250_GYRO_CONFIG         0x1BU
#define MPU9250_ACCEL_CONFIG        0x1CU

/**
 * @brief This register is used to verify the identity of the device.
 * The contents of WHO_AM_I is an 8-bit device ID.
 * The default value of the register is 0x71. 
 */
#define MPU9250_WHO_AM_I            0x75U

#ifdef __cplusplus
}
#endif

#endif /* __MPU9250_REGISTERS_H */