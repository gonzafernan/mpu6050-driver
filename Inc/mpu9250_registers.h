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
 * @brief Accelerometer Measurements
*/
#define MPU9250_ACCEL_XOUT_H        0x3BU
#define MPU9250_ACCEL_XOUT_L        0x3CU
#define MPU9250_ACCEL_YOUT_H        0x3DU
#define MPU9250_ACCEL_YOUT_L        0x3EU
#define MPU9250_ACCEL_ZOUT_H        0x3FU
#define MPU9250_ACCEL_ZOUT_L        0x40U

/**
 * @brief Temperature Measurements
*/
#define MPU9250_TEMP_OUT_H          0x41U
#define MPU9250_TEMP_OUT_L          0x42U

/**
 * @brief Gyroscope Measurements
*/
#define MPU9250_GYRO_XOUT_H         0x43U
#define MPU9250_GYRO_XOUT_L         0x44U
#define MPU9250_GYRO_YOUT_H         0x45U
#define MPU9250_GYRO_YOUT_L         0x46U
#define MPU9250_GYRO_ZOUT_H         0x47U
#define MPU9250_GYRO_ZOUT_L         0x48U 

/**
 * @brief This register is used to verify the identity of the device.
 * The contents of WHO_AM_I is an 8-bit device ID.
 * The default value of the register is 0x71. 
 */
#define MPU9250_WHO_AM_I            0x75U

/**
 * @brief Gyro Full Scale Select:
 * 00 = +250 dps
 * 01 = +500 dps
 * 10 = +1000 dps
 * 11 = +2000 dps
*/
#define MPU9250_GYRO_FS_SEL_OFFSET   3

/**
 * @brief Accel Full Scale Select:
 * 00 = +-2g
 * 01 = +- 4g
 * 10 = +- 8g
 * 11 = +- 16g
*/
#define MPU9250_ACCEL_FS_SEL_OFFSET   3


#ifdef __cplusplus
}
#endif

#endif /* __MPU9250_REGISTERS_H */