/**
  ******************************************************************************
  * @file           : port_i2c.c
  * @author         : Gonzalo Gabriel Fernandez
  * @brief          : MPU9250 Driver I2C port for STM32F1XX
  ******************************************************************************
  * @attention
  *
  * MPU9250 Driver I2C port for STM32F1XX.
  * Tested on STM32F103C8T6.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "port_i2c.h"
#include "mpu9250.h"
#include "stm32f1xx_hal.h"

#include <assert.h>

/* Private variables ---------------------------------------------------------*/
static I2C_HandleTypeDef hi2c;
DMA_HandleTypeDef hdma_i2c1_rx;

/* Exported functions ---------------------------------------------------------*/
/**
 * @brief I2C init function
*/
MPU9250_StatusTypeDef I2C_Init(MPU9250_HandleTypeDef* hmpu)
{
    hi2c.Instance = I2C1;
    hi2c.Init.ClockSpeed = 400000;
    hi2c.Init.DutyCycle = I2C_DUTYCYCLE_2;
    hi2c.Init.OwnAddress1 = 0;
    hi2c.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c.Init.OwnAddress2 = 0;
    hi2c.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

    if (HAL_I2C_Init(&hi2c) != HAL_OK) return MPU9250_ERROR;
    return MPU9250_OK;
}

/**
 * @brief I2C read register
*/
MPU9250_StatusTypeDef I2C_Reg_Read(MPU9250_HandleTypeDef* hmpu, uint8_t RegAddress, uint8_t* pData)
{
    assert(hmpu);
    if (HAL_I2C_Mem_Read(&hi2c, (uint16_t)hmpu->Address << 1, RegAddress, sizeof(uint8_t), pData, sizeof(uint8_t), hmpu->I2C_Timeout) != HAL_OK) return MPU9250_ERROR;
    return MPU9250_OK;
}

/**
 * @brief I2C burst read
 * @note Read multiple registers in burst mode with I2C
*/
MPU9250_StatusTypeDef I2C_Burst_Read(MPU9250_HandleTypeDef* hmpu, uint8_t RegAddress, uint8_t* pData, uint16_t DataAmont)
{
    assert(hmpu);
    if (HAL_I2C_Mem_Read(&hi2c, (uint16_t)hmpu->Address << 1, RegAddress, sizeof(uint8_t), pData, sizeof(uint8_t)*DataAmont, hmpu->I2C_Timeout) != HAL_OK) return MPU9250_ERROR;
    return MPU9250_OK;
}

/**
 * @brief I2C write register
*/
MPU9250_StatusTypeDef I2C_Reg_Write(MPU9250_HandleTypeDef* hmpu, uint8_t RegAddress, uint8_t* pData)
{
    assert(hmpu);
    if (HAL_I2C_Mem_Write(&hi2c, (uint16_t)hmpu->Address << 1, RegAddress, sizeof(uint8_t), pData, sizeof(uint8_t), hmpu->I2C_Timeout) != HAL_OK) return MPU9250_ERROR;
    return MPU9250_OK;
}

/**
 * @brief I2C non-blocking read through DMA
*/
MPU9250_StatusTypeDef I2C_Read_DMA(MPU9250_HandleTypeDef* hmpu, uint8_t RegAddress, uint8_t*pData, uint16_t DataAmount)
{
    assert(hmpu);
    if (HAL_I2C_Mem_Read_DMA(&hi2c, (uint16_t)hmpu->Address << 1, RegAddress, sizeof(uint8_t), pData, DataAmount) != HAL_OK) return MPU9250_ERROR;
    return MPU9250_OK;
}

void HAL_I2C_MspInit(I2C_HandleTypeDef* i2cHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(i2cHandle->Instance==I2C1)
  {
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**I2C1 GPIO Configuration
    PB6     ------> I2C1_SCL
    PB7     ------> I2C1_SDA
    */
    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* I2C1 clock enable */
    __HAL_RCC_I2C1_CLK_ENABLE();

    /* I2C1 DMA Init */
    /* I2C1_RX Init */
    hdma_i2c1_rx.Instance = DMA1_Channel7;
    hdma_i2c1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_i2c1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_i2c1_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_i2c1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_i2c1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_i2c1_rx.Init.Mode = DMA_NORMAL;
    hdma_i2c1_rx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_i2c1_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(i2cHandle,hdmarx,hdma_i2c1_rx);

  }
}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef* i2cHandle)
{

  if(i2cHandle->Instance==I2C1)
  {
    /* Peripheral clock disable */
    __HAL_RCC_I2C1_CLK_DISABLE();

    /**I2C1 GPIO Configuration
    PB6     ------> I2C1_SCL
    PB7     ------> I2C1_SDA
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_6);

    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_7);

    /* I2C1 DMA DeInit */
    HAL_DMA_DeInit(i2cHandle->hdmarx);
  }
}

void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef* hi2c)
{
    MPU9250_RxCallback();
}