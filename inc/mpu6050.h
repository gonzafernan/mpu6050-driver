/**
 ******************************************************************************
 * @file           : mpu6050.h
 * @author         : Gonzalo Gabriel Fernandez
 * @brief          : MPU6050 Driver headers
 ******************************************************************************
 * @attention
 *
 * MPU6050 C Driver public functions and variables.
 *
 ******************************************************************************
 */

#ifndef __MPU6050_H
#define __MPU6050_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#include "mpu6050_def.h"

#define MPU6050_WHO_AM_I_DEFAULT 0x68U /*! Who Am I default value */

/**
 * @brief MPU6050 Gyro Full Scale Select
 */
typedef enum {
  MPU6050_GYRO_CONFIG_250DPS = 0b00U,
  MPU6050_GYRO_CONFIG_500DPS = 0b01U,
  MPU6050_GYRO_CONFIG_1000DPS = 0b10U,
  MPU6050_GYRO_CONFIG_2000DPS = 0b11U,

} mpu6050_gyroconfig_fs_t;

/**
 * @brief MPU6050 Accel Full Scale Select
 */
typedef enum {
  MPU6050_ACCEL_CONFIG_2G = 0b00U,
  MPU6050_ACCEL_CONFIG_4G = 0b01U,
  MPU6050_ACCEL_CONFIG_8G = 0b10U,
  MPU6050_ACCEL_CONFIG_16G = 0b11U,

} mpu6050_accelconfig_fs_t;

mpu6050_status_t mpu6050_init(void);
mpu6050_status_t mpu6050_sanity_check(void);
mpu6050_status_t mpu6050_read_pwrmgmt(uint8_t *ppwrmgmt);
mpu6050_status_t mpu6050_reset_pwrmgmt(void);
mpu6050_status_t mpu6050_gyro_read_config(uint8_t *pgyroconfig);
mpu6050_status_t mpu6050_accel_read_config(uint8_t *paccelconfig);
mpu6050_status_t mpu6050_gyro_set_fullscale(mpu6050_gyroconfig_fs_t gyro_fullscale);
mpu6050_status_t mpu6050_accel_set_fullscale(mpu6050_accelconfig_fs_t accel_fullscale);
mpu6050_status_t mpu6050_gyro_read_raw(uint16_t *pgyrox, uint16_t *pgyroy, uint16_t *pgyroz);
mpu6050_status_t mpu6050_gyro_fetch(void);
mpu6050_status_t mpu6050_gyro_read_from_buffer(uint16_t *pgyrox, uint16_t *pgyroy,
                                               uint16_t *pgyroz);
mpu6050_status_t mpu6050_accel_read_raw(uint16_t *paccelx, uint16_t *paccely, uint16_t *paccelz);
mpu6050_status_t mpu6050_accel_fetch(void);
mpu6050_status_t mpu6050_accel_read_from_buffer(uint16_t *paccelx, uint16_t *paccely,
                                                uint16_t *paccelz);
mpu6050_status_t mpu6050_temp_read_raw(uint16_t *ptemp);
mpu6050_status_t mpu6050_temp_fetch(void);
mpu6050_status_t mpu6050_temp_read_from_buffer(uint16_t *ptemp);
void mpu6050_rxcallback(void);
bool mpu6050_is_data_ready(void);

#ifdef __cplusplus
}
#endif

#endif /* __MPU9250_H */
