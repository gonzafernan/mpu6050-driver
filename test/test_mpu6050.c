/**
 * @author Gonzalo G. Fernandez
 * @file test_mpu6050.c
 */

#include "mock_port_i2c.h"
#include "mpu6050.h"
#include "mpu6050_registers.h"
#include "unity.h"
#include <stddef.h>

mpu6050_t hmpu;

void setUp(void) {
    i2c_init_ExpectAndReturn(NULL, MPU6050_OK);

    /**
     * From CMock documentation:
     * Maybe you overall want to use Expect and its similar variations, but you don't care what is
     * passed to a particular argument. This is particularly useful when that argument is a pointer
     * to a value that is supposed to be filled in by the function. You don't want to use
     * ExpectAnyArgs, because you still care about the other arguments. Instead, after an Expect
     * call is made, you can call this function. It tells CMock to ignore a particular argument for
     * the rest of this test, for this mock function. You may call multiple instances of this to
     * ignore multiple arguments after each expectation if desired.
     */
    i2c_reg_write_ExpectAndReturn(NULL, MPU6050_I2C_ADDRESS_1 << 1, MPU6050_PWR_MGMT_1, NULL,
                                  MPU6050_OK);
    i2c_reg_write_IgnoreArg_pdata();

    mpu6050_init(&hmpu, NULL);
}

/**
 * @brief Test correct initialization
 */
void test_mpu6050_correct_init(void) {
    mpu6050_t hmpu;
    uint8_t mock_i2c_handle = 0x45;

    // Mock I2C initialization
    i2c_init_ExpectAndReturn((void *)&mock_i2c_handle, MPU6050_OK);
    i2c_reg_write_ExpectAndReturn((void *)&mock_i2c_handle, MPU6050_I2C_ADDRESS_1 << 1,
                                  MPU6050_PWR_MGMT_1, NULL, MPU6050_OK);
    i2c_reg_write_IgnoreArg_pdata();

    // Initializa MPU6050 driver
    mpu6050_status_t status = mpu6050_init(&hmpu, (void *)&mock_i2c_handle);
    TEST_ASSERT_EQUAL(MPU6050_OK, status);
}

/**
 * @brief Test incorrect initialization
 */
void test_mpu6050_incorrect_init(void) {
    mpu6050_t hmpu;

    // Mock I2C initialization
    i2c_init_ExpectAndReturn(NULL, MPU6050_ERROR);

    // Initializa MPU6050 driver
    mpu6050_status_t status = mpu6050_init(&hmpu, NULL);
    TEST_ASSERT_EQUAL(MPU6050_ERROR, status);
}

/**
 * @brief Test correct gyroscope measurements blocking read
 */
void test_mpu6050_correct_gyro_blocking_read(void) {
    uint16_t gyrox, gyroy, gyroz;
    i2c_burst_read_ExpectAndReturn(NULL, MPU6050_I2C_ADDRESS_1 << 1, MPU6050_GYRO_XOUT_H, NULL, 6,
                                   MPU6050_OK);
    i2c_burst_read_IgnoreArg_pdata();

    mpu6050_status_t status = mpu6050_gyro_read_raw(&hmpu, &gyrox, &gyroy, &gyroz);
    TEST_ASSERT_EQUAL(MPU6050_OK, status);
}

/**
 * @brief Test incorrect gyroscope measurements blocking read
 */
void test_mpu6050_incorrect_gyro_blocking_read(void) {
    uint16_t gyrox, gyroy, gyroz;
    i2c_burst_read_ExpectAndReturn(NULL, MPU6050_I2C_ADDRESS_1 << 1, MPU6050_GYRO_XOUT_H, NULL, 6,
                                   MPU6050_ERROR);
    i2c_burst_read_IgnoreArg_pdata();

    mpu6050_status_t status = mpu6050_gyro_read_raw(&hmpu, &gyrox, &gyroy, &gyroz);
    TEST_ASSERT_EQUAL(MPU6050_ERROR, status);
}

/**
 * @brief Test correct accelerometer measurements blocking read
 */
void test_mpu6050_correct_accel_blocking_read(void) {
    uint16_t accelx, accely, accelz;
    i2c_burst_read_ExpectAndReturn(NULL, MPU6050_I2C_ADDRESS_1 << 1, MPU6050_ACCEL_XOUT_H, NULL, 6,
                                   MPU6050_OK);
    i2c_burst_read_IgnoreArg_pdata();

    mpu6050_status_t status = mpu6050_accel_read_raw(&hmpu, &accelx, &accely, &accelz);
    TEST_ASSERT_EQUAL(MPU6050_OK, status);
}

/**
 * @brief Test incorrect accelerometer measurements blocking read
 */
void test_mpu6050_incorrect_accel_blocking_read(void) {
    uint16_t accelx, accely, accelz;
    i2c_burst_read_ExpectAndReturn(NULL, MPU6050_I2C_ADDRESS_1 << 1, MPU6050_ACCEL_XOUT_H, NULL, 6,
                                   MPU6050_ERROR);
    i2c_burst_read_IgnoreArg_pdata();

    mpu6050_status_t status = mpu6050_accel_read_raw(&hmpu, &accelx, &accely, &accelz);
    TEST_ASSERT_EQUAL(MPU6050_ERROR, status);
}

/**
 * @brief Test correct temperature measurements blocking read
 */
void test_mpu6050_correct_temp_blocking_read(void) {
    uint16_t temp;
    i2c_burst_read_ExpectAndReturn(NULL, MPU6050_I2C_ADDRESS_1 << 1, MPU6050_TEMP_OUT_H, NULL, 2,
                                   MPU6050_OK);
    i2c_burst_read_IgnoreArg_pdata();

    mpu6050_status_t status = mpu6050_temp_read_raw(&hmpu, &temp);
    TEST_ASSERT_EQUAL(MPU6050_OK, status);
}

/**
 * @brief Test incorrect temperature measurements blocking read
 */
void test_mpu6050_incorrect_temp_blocking_read(void) {
    uint16_t temp;
    i2c_burst_read_ExpectAndReturn(NULL, MPU6050_I2C_ADDRESS_1 << 1, MPU6050_TEMP_OUT_H, NULL, 2,
                                   MPU6050_ERROR);
    i2c_burst_read_IgnoreArg_pdata();

    mpu6050_status_t status = mpu6050_temp_read_raw(&hmpu, &temp);
    TEST_ASSERT_EQUAL(MPU6050_ERROR, status);
}

/**
 * @brief Test correct power management reset
 */
void test_mpu6050_correct_reset_pwrmgmt(void) {
    i2c_reg_write_ExpectAndReturn(NULL, MPU6050_I2C_ADDRESS_1 << 1, MPU6050_PWR_MGMT_1, NULL,
                                  MPU6050_OK);
    i2c_reg_write_IgnoreArg_pdata();
    mpu6050_status_t status = mpu6050_reset_pwrmgmt(&hmpu);
    TEST_ASSERT_EQUAL(MPU6050_OK, status);
}

/**
 * @brief Test incorrect power management reset
 */
void test_mpu6050_incorrect_reset_pwrmgmt(void) {
    i2c_reg_write_ExpectAndReturn(NULL, MPU6050_I2C_ADDRESS_1 << 1, MPU6050_PWR_MGMT_1, NULL,
                                  MPU6050_ERROR);
    i2c_reg_write_IgnoreArg_pdata();
    mpu6050_status_t status = mpu6050_reset_pwrmgmt(&hmpu);
    TEST_ASSERT_EQUAL(MPU6050_ERROR, status);
}
