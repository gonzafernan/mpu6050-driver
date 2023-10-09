/**
 * @author Gonzalo G. Fernandez
 * @file test_mpu6050.c
 */

#include "cmock.h"
#include "mock_port_i2c.h"
#include "mpu6050.h"
#include <stddef.h>

/**
 * @brief Test correct initialization
 */
void test_mpu6050_correct_init(void) {
    mpu6050_t hmpu;

    // Mock I2C initialization
    i2c_init_ExpectAndReturn(NULL, MPU6050_OK);

    // Initializa MPU6050 driver
    mpu6050_status_t status = mpu6050_init(&hmpu, NULL);
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
