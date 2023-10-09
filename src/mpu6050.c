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

#include "mpu6050.h"
#include "mpu6050_registers.h"
#include "port_i2c.h"

#include <assert.h>
#include <stdbool.h>

static bool data_ready_flag = false; /*! Flag for data ready */
static uint8_t rxbuffer[6];          /*! Buffer with data from non-blocking read */

/**
 * @brief   MPU6050 user-defined error callback
 * @param   hmpu: MPU6050 handle
 */
// __weak void mpu6050_error_callback(mpu6050_t* hmpu)
// {
//     /**
//      * NOTE: This function should be implemented in the user file
//     */
// }

/**
 * @brief   Read MPU9250 register
 * @param   hmpu: Pointer to MPU6050 handle
 * @param   reg_address: Address of register to read
 * @param   pdata: Pointer to buffer where value will be stored
 * @retval  mpu6050_status_t
 */
static mpu6050_status_t mpu6050_reg_read(mpu6050_t *hmpu, uint8_t reg_address, uint8_t *pdata) {
    /* MPU6050 register read wrapper */
    return i2c_reg_read((uint16_t)hmpu->address << 1, reg_address, pdata);
}

/**
 * @brief   Burst read MPU6050 registers
 * @param   reg_address: Address of first register to read
 * @param   pdata: Pointer to buffer where data will be stored
 * @param   data_amount: Amount of data to read
 * @retval  mpu6050_status_t
 */
static mpu6050_status_t mpu6050_burst_read(mpu6050_t *hmpu, uint8_t reg_address, uint8_t *pdata,
                                           uint16_t data_amount) {
    /* MPU6050 register read wrapper */
    return i2c_burst_read((uint16_t)hmpu->address << 1, reg_address, pdata, data_amount);
}

/**
 * @brief   Write MPU6050 register
 * @param   reg_address: Address of register to write
 * @param   pdata: Pointer to buffer with value to write
 * @retval  mpu6050_status_t
 */
static mpu6050_status_t mpu6050_reg_write(mpu6050_t *hmpu, uint8_t reg_address, uint8_t *pdata) {
    /* MPU6050 register write wrapper */
    return i2c_reg_write((uint16_t)hmpu->address << 1, reg_address, pdata);
}

/**
 * @brief   MPU-6050 Non-blocking burst read
 * @param   reg_address: Address of first register to read
 * @param   pdata: Pointer to buffer where received data will be stored
 * @retval  mpu6050_status_t
 */
static mpu6050_status_t mpu6050_nonblocking_read(mpu6050_t *hmpu, uint8_t reg_address,
                                                 uint8_t *pdata, uint16_t data_amount) {
    /* MPU6050 non-blocking register read wrapper */
    return i2c_read_dma((uint16_t)hmpu->address << 1, reg_address, pdata, data_amount);
}

/**
 * @brief   MPU6050 callback for data ready
 */
void mpu6050_rxcallback(void) { data_ready_flag = true; }

/**
 * @brief   MPU6050 check for data ready
 * @retval  bool
 */
bool mpu6050_is_data_ready(void) {
    if (data_ready_flag) {
        data_ready_flag = false;
        return true;
    }
    return false;
}

/**
 * @brief   Initialize MPU9250 device
 * @retval  mpu6050_status_t
 */
mpu6050_status_t mpu6050_init(mpu6050_t *hmpu, void *hi2c) {
    /* I2C initialization */
    hmpu->address = MPU6050_I2C_ADDRESS_1;
    hmpu->i2c_timeout = 100;
    hmpu->i2c_handle = hi2c;
    if (i2c_init(hi2c) != MPU6050_OK)
        return MPU6050_ERROR;
    return MPU6050_OK;
}

/**
 * @brief   Read MPU6050 Power Management 1
 * @param   ppwrmgmt: Pointer to buffer where configuration will be stored
 * @retval  mpu6050_status_t
 */
mpu6050_status_t mpu6050_read_pwrmgmt(mpu6050_t *hmpu, uint8_t *ppwrmgmt) {
    assert(ppwrmgmt);
    if (mpu6050_reg_read(hmpu, MPU6050_PWR_MGMT_1, &ppwrmgmt[0]) != MPU6050_OK)
        return MPU6050_ERROR;
    if (mpu6050_reg_read(hmpu, MPU6050_PWR_MGMT_2, &ppwrmgmt[1]) != MPU6050_OK)
        return MPU6050_ERROR;
    return MPU6050_OK;
}

/**
 * @brief   Read MPU6050 Reset Power Management 1
 * @retval  mpu6050_status_t
 */
mpu6050_status_t mpu6050_reset_pwrmgmt(mpu6050_t *hmpu) {
    uint8_t reg_value = 0x00;
    if (mpu6050_reg_write(hmpu, MPU6050_PWR_MGMT_1, &reg_value) != MPU6050_OK)
        return MPU6050_ERROR;
    return MPU6050_OK;
}

/**
 * @brief   Read current Gyro configuration
 * @param   pgyroconfig: Pointer to buffer where configuration will be stored
 * @retval  mpu6050_status_t
 */
mpu6050_status_t mpu6050_gyro_read_config(mpu6050_t *hmpu, uint8_t *pgyroconfig) {
    if (mpu6050_reg_read(hmpu, MPU6050_GYRO_CONFIG, pgyroconfig) != MPU6050_OK)
        return MPU6050_ERROR;
    return MPU6050_OK;
}

/**
 * @brief   Read current Accel configuration
 * @param   paccelconfig: Pointer to buffer where configuration will be stored
 * @retval  mpu6050_status_t
 */
mpu6050_status_t mpu6050_accel_read_config(mpu6050_t *hmpu, uint8_t *paccelconfig) {
    if (mpu6050_reg_read(hmpu, MPU6050_ACCEL_CONFIG, paccelconfig) != MPU6050_OK)
        return MPU6050_ERROR;
    return MPU6050_OK;
}

/**
 * @brief   Gyro Full Scale selection
 * @param   GyroFullScale: Gyro Full Scale to set
 * @retval  mpu6050_status_t
 */
mpu6050_status_t mpu6050_gyro_set_fullscale(mpu6050_t *hmpu,
                                            mpu6050_gyroconfig_fs_t gyro_fullscale) {
    uint8_t reg_value;
    if (mpu6050_gyro_read_config(hmpu, &reg_value) != MPU6050_OK)
        return MPU6050_ERROR;

    reg_value &= ~(0b11 << MPU6050_GYRO_FS_SEL_OFFSET);
    reg_value |= (gyro_fullscale << MPU6050_GYRO_FS_SEL_OFFSET);
    if (mpu6050_reg_write(hmpu, MPU6050_GYRO_CONFIG, &reg_value) != MPU6050_OK)
        return MPU6050_ERROR;
    return MPU6050_OK;
}

/**
 * @brief   Accel Full Scale selection
 * @param   accel_fullscale: Accel Full Scale to set
 * @retval  mpu6050_status_t
 */
mpu6050_status_t mpu6050_accel_set_fullscale(mpu6050_t *hmpu,
                                             mpu6050_accelconfig_fs_t accel_fullscale) {
    uint8_t reg_value;
    if (mpu6050_accel_read_config(hmpu, &reg_value) != MPU6050_OK)
        return MPU6050_ERROR;

    reg_value &= ~(0b11 << MPU6050_ACCEL_FS_SEL_OFFSET);
    reg_value |= (accel_fullscale << MPU6050_ACCEL_FS_SEL_OFFSET);
    if (mpu6050_reg_write(hmpu, MPU6050_ACCEL_CONFIG, &reg_value) != MPU6050_OK)
        return MPU6050_ERROR;
    return MPU6050_OK;
}

/**
 * @brief   Raw Gyroscope Measurements
 * @param   pgyrox: Pointer to buffer where Gyro X-axis measurement will be stored
 * @param   pgyroy: Pointer to buffer where Gyro Y-axis measurement will be stored
 * @param   pgyroz: Pointer to buffer where Gyro Z-axis measurement will be stored
 * @retval  mpu6050_status_t
 */
mpu6050_status_t mpu6050_gyro_read_raw(mpu6050_t *hmpu, uint16_t *pgyrox, uint16_t *pgyroy,
                                       uint16_t *pgyroz) {
    uint8_t reg_value[6];
    if (mpu6050_burst_read(hmpu, MPU6050_GYRO_XOUT_H, reg_value, 6) != MPU6050_OK)
        return MPU6050_ERROR;

    *pgyrox = (reg_value[0] << 8) | reg_value[1];
    *pgyroy = (reg_value[2] << 8) | reg_value[3];
    *pgyroz = (reg_value[4] << 8) | reg_value[5];

    return MPU6050_OK;
}

/**
 * @brief   Fetch Gyro Measurements and load it into buffer
 * @retval  mpu6050_status_t
 */
mpu6050_status_t mpu6050_gyro_fetch(mpu6050_t *hmpu) {
    if (mpu6050_nonblocking_read(hmpu, MPU6050_GYRO_XOUT_H, rxbuffer, 6) != MPU6050_OK)
        return MPU6050_ERROR;
    return MPU6050_OK;
}

/**
 * @brief   Raw Gyroscope Measurements from buffer
 * @param   pgyrox: Pointer to buffer where Gyro X-axis measurement will be stored
 * @param   pgyroy: Pointer to buffer where Gyro Y-axis measurement will be stored
 * @param   pgyroz: Pointer to buffer where Gyro Z-axis measurement will be stored
 * @retval  mpu6050_status_t
 */
mpu6050_status_t mpu6050_gyro_read_from_buffer(mpu6050_t *hmpu, uint16_t *pgyrox, uint16_t *pgyroy,
                                               uint16_t *pgyroz) {
    *pgyrox = (rxbuffer[0] << 8) | rxbuffer[1];
    *pgyroy = (rxbuffer[2] << 8) | rxbuffer[3];
    *pgyroz = (rxbuffer[4] << 8) | rxbuffer[5];

    return MPU6050_OK;
}

/**
 * @brief   Raw Accelerometer Measurements
 * @param   paccelx: Pointer to buffer where Accel X-axis measurement will be stored
 * @param   paccely: Pointer to buffer where Accel Y-axis measurement will be stored
 * @param   paccelz: Pointer to buffer where Accel Z-axis measurement will be stored
 * @retval  mpu6050_status_t
 */
mpu6050_status_t mpu6050_accel_read_raw(mpu6050_t *hmpu, uint16_t *paccelx, uint16_t *paccely,
                                        uint16_t *paccelz) {
    uint8_t reg_value[6];
    if (mpu6050_burst_read(hmpu, MPU6050_ACCEL_XOUT_H, reg_value, 6) != MPU6050_OK)
        return MPU6050_ERROR;

    *paccelx = (reg_value[0] << 8) | reg_value[1];
    *paccely = (reg_value[2] << 8) | reg_value[3];
    *paccelz = (reg_value[4] << 8) | reg_value[5];

    return MPU6050_OK;
}

/**
 * @brief   Fetch Accelerometer Measurements and load it into buffer
 * @retval  mpu6050_status_t
 */
mpu6050_status_t mpu6050_accel_fetch(mpu6050_t *hmpu) {
    if (mpu6050_nonblocking_read(hmpu, MPU6050_ACCEL_XOUT_H, rxbuffer, 6) != MPU6050_OK)
        return MPU6050_ERROR;
    return MPU6050_OK;
}

/**
 * @brief   Raw Accelerometer Measurements from buffer
 * @param   paccelx: Pointer to buffer where Accel X-axis measurement will be stored
 * @param   paccely: Pointer to buffer where Accel Y-axis measurement will be stored
 * @param   paccelz: Pointer to buffer where Accel Z-axis measurement will be stored
 * @retval  mpu6050_status_t
 */
mpu6050_status_t mpu6050_accel_read_from_buffer(mpu6050_t *hmpu, uint16_t *paccelx,
                                                uint16_t *paccely, uint16_t *paccelz) {
    *paccelx = (rxbuffer[0] << 8) | rxbuffer[1];
    *paccely = (rxbuffer[2] << 8) | rxbuffer[3];
    *paccelz = (rxbuffer[4] << 8) | rxbuffer[5];

    return MPU6050_OK;
}

/**
 * @brief   Temperature Measurement
 * @param   ptemp: Pointer to buffer where temperature measurement will be stored
 * @retval  mpu6050_status_t
 */
mpu6050_status_t mpu6050_temp_read_raw(mpu6050_t *hmpu, uint16_t *ptemp) {
    uint8_t reg_value[6];
    if (mpu6050_burst_read(hmpu, MPU6050_TEMP_OUT_H, reg_value, 2) != MPU6050_OK)
        return MPU6050_ERROR;

    *ptemp = (reg_value[0] << 8) | reg_value[1];

    return MPU6050_OK;
}

/**
 * @brief   Fetch Temperature Measurements and load it into buffer
 * @retval  mpu6050_status_t
 */
mpu6050_status_t mpu6050_temp_fetch(mpu6050_t *hmpu) {
    if (mpu6050_nonblocking_read(hmpu, MPU6050_GYRO_XOUT_H, rxbuffer, 6) != MPU6050_OK)
        return MPU6050_ERROR;
    return MPU6050_OK;
}

/**
 * @brief   Raw Temperature Measurements from buffer
 * @param   ptemp: Pointer to buffer where Temperature measurement will be stored
 * @retval  mpu6050_status_t
 */
mpu6050_status_t mpu6050_temp_read_from_buffer(mpu6050_t *hmpu, uint16_t *ptemp) {
    *ptemp = (rxbuffer[0] << 8) | rxbuffer[1];
    return MPU6050_OK;
}

/**
 * @brief   MPU6050 Sanity Check
 * @note    It performs a who am I to verify the I2C slave
 * @retval  mpu6050_status_t
 */
mpu6050_status_t mpu6050_sanity_check(mpu6050_t *hmpu) {
    /* MPU-6050 Who Am I check */
    uint8_t reg_value;
    if (mpu6050_reg_read(hmpu, MPU6050_WHO_AM_I, &reg_value) != MPU6050_OK)
        return MPU6050_ERROR;
    if (reg_value != MPU6050_WHO_AM_I_DEFAULT)
        return MPU6050_ERROR;
    return MPU6050_OK;
}
