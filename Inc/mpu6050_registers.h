/**
  ******************************************************************************
  * @file           : mpu6050_registers.h
  * @author         : Gonzalo Gabriel Fernandez
  * @brief          : MPU6050 Registers Map
  ******************************************************************************
  * @note
  *
  * MPU6050 Registers defines and macros.
  * Register Map Revision 4.2
  *
  ******************************************************************************
  */

#ifndef __MPU6050_REGISTERS_H
#define __MPU6050_REGISTERS_H

#ifdef __cplusplus
extern "C" {
#endif

#define MPU6050_SELF_TEST_X         0x0DU
#define MPU6050_SELF_TEST_Y         0x0EU
#define MPU6050_SELF_TEST_Z         0x0FU
#define MPU6050_SELF_TEST_A         0x10U

#define MPU6050_CONFIG              0x1AU
#define MPU6050_GYRO_CONFIG         0x1BU
#define MPU6050_ACCEL_CONFIG        0x1CU

#define MPU6050_INT_PIN_CFG         0x37U   /*!< INT Pin/Bypass Enable Configuration */

/**
 * @brief Accelerometer Measurements
*/
#define MPU6050_ACCEL_XOUT_H        0x3BU
#define MPU6050_ACCEL_XOUT_L        0x3CU
#define MPU6050_ACCEL_YOUT_H        0x3DU
#define MPU6050_ACCEL_YOUT_L        0x3EU
#define MPU6050_ACCEL_ZOUT_H        0x3FU
#define MPU6050_ACCEL_ZOUT_L        0x40U

/**
 * @brief Temperature Measurements
*/
#define MPU6050_TEMP_OUT_H          0x41U
#define MPU6050_TEMP_OUT_L          0x42U

/**
 * @brief Gyroscope Measurements
*/
#define MPU6050_GYRO_XOUT_H         0x43U
#define MPU6050_GYRO_XOUT_L         0x44U
#define MPU6050_GYRO_YOUT_H         0x45U
#define MPU6050_GYRO_YOUT_L         0x46U
#define MPU6050_GYRO_ZOUT_H         0x47U
#define MPU6050_GYRO_ZOUT_L         0x48U 

#define MPU6050_USER_CTRL           0x6AU   /*!< MPU6050 User Control */

#define MPU6050_PWR_MGMT_1          0x6BU   /*!< MPU6050 Power Management 1 */
#define MPU6050_PWR_MGMT_2          0x6CU   /*!< MPU6050 Power Management 2 */

/**
 * @brief This register is used to verify the identity of the device.
 * The contents of WHO_AM_I is an 8-bit device ID.
 * The default value of the register is 0x68. 
 */
#define MPU6050_WHO_AM_I            0x75U

/**
 * @brief Gyro Full Scale Select:
 * 00 = +250 dps
 * 01 = +500 dps
 * 10 = +1000 dps
 * 11 = +2000 dps
*/
#define MPU6050_GYRO_FS_SEL_OFFSET      3

/**
 * @brief Accel Full Scale Select:
 * 00 = +-2g
 * 01 = +- 4g
 * 10 = +- 8g
 * 11 = +- 16g
*/
#define MPU6050_ACCEL_FS_SEL_OFFSET     3

/**
 * @brief When asserted the i2c_master interface pins will go into bypass mode when the i2c master interface is disabled
 * The pins will float high due to the internal pull-up if not enabled and the i2c master
 * interface is disabled.
*/
#define MPU6050_BYPASS_EN_OFFSET        1

/**
 * @brief Enable (1) and disable (0) I2C Master I/F module
*/
#define MPU6050_I2C_MST_EN              5


#ifdef __cplusplus
}
#endif

#endif /* __MPU6050_REGISTERS_H */