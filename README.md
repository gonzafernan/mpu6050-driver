# MPU-6050 Driver

Author: Gonzalo Gabriel Fernandez

## Features
- Device initialization
- Sanity check
- Get Gyroscope and Accelerometer configuration word
- Full Scale selection for Gyroscope and Accelerometer
- Blocking read of raw Gyroscope, Accelerometer, and Temperature measurements
- Non-blocking read of raw Gyroscope, Accelerometer, and Temperature measurements

## Port
Currently, the microcontroller families supported are:
- STM32F103C8T6 (Blue Pill board) (STM32F1XX)
- STM32F429ZI (STM32F4XX)

The non-blocking read is supported via DMA.

## Arquitectura de driver MPU-6050

### Driver OSAL (OS Abstraction Layer)

Operaciones con Mutex:
```c
mpu6050_mutex_t mpu6050_osal_create_mutex(void);
mpu6050_status_t mpu6050_osal_take_mutex(mpu6050_mutex_t mutex);
mpu6050_status_t mpu6050_osal_give_mutex(mpu6050_mutex_t mutex);
```

Donde `mpu6050_mutex_t` es una redefinición de un `void *`.

### Driver HAL (Hardware Abstraction Layer)

Escritura de memoria bloqueante a través de I2C:

```c
mpu6050_status_t mpu6050_hal_i2c_reg_write(
    mpu6050_i2c_handle_t hi2c,
    uint16_t slave_address,
    uint8_t reg_address,
    uint8_t *buffer,
    uint16_t size,
    uint32_t timeout,
);  // HAL_I2C_Mem_Write wrapper
```

Donde `mpu6050_i2c_handle_t` es una redefinición de un `void *`.

Lectura no bloqueante con DMA a través de I2C:

```c
mpu6050_status_t mpu6050_hal_i2c_read_dma(
    mpu6050_i2c_handle_t hi2c,
    uint16_t slave_address,
    uint8_t reg_address,
    uint8_t *buffer,
    uint16_t size,
);  // HAL_I2C_Mem_Read_DMA wrapper
```

En la HAL de STM32, el callback de lectura completa:

```c
void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c);
```

El port define la función anterior llamando la siguiente función que utilizará el driver como callback:

```c
void mpu6050_hal_i2c_read_cplt_callback(mpu6050_i2c_handle_t hi2c);
```

### Inicialización del driver

```c
mpu6050_status_t mpu6050_init(
    mpu6050_t *hmpu,
    mpu6050_i2c_handle_t hi2c,
    mpu6050_i2c_address_t address,
    uint32_t write_timeout,
    uint32_t read_timeout,
);
```

La función inicializa la estructura `mpu6050_t`:

```c
typedef struct {
    mpu6050_i2c_handle_t hi2c;
    mpu6050_i2c_address_t address;
    ringbuffer_t gyro_ringbuffer;
    ringbuffer_t accel_ringbuffer;
    ringbuffer_t temp_ringbuffer;
} mpu6050_t;
```

El módulo MPU6050 por hardware puede tener dos posibles direcciones:

```c
typedef enum {
    MPU6050_I2C_ADDRESS_1 = 0x68U,
    MPU6050_I2C_ADDRESS_2 = 0x69U,
} mpu6050_i2c_address_t;
```

La inicialización retorna un tipo estado para chequear que se realizó correctamente:

```c
typedef enum {
    MPU6050_OK = 0x00U,
    MPU6050_ERROR = 0x01U
} mpu6050_status_t;
```


### Lectura de acelerómetro

Para iniciar el proceso de lectura no bloqueante del acelerómetro:

```c
mpu6050_status_t mpu6050_accel_read_dma(mpu6050_t *hmpu);
```

Si la cola de solicitudes de lectura no bloqueante se encuentra vacía, se lanza la lectura
no bloqueante de forma directa y se setea un flag de lectura en proceso.

Para obtener muestra de acelerómetro:

```c
mpu6050_status_t mpu6050_accel_read(
    mpu6050_t *hmpu,
    uint16_t *accel_x,
    uint16_t *accel_y,
    uint16_t *accel_z,
);
```

Con macros se define si el comportamiento es bloqueante o no. En caso de ser no bloqueante,
la lectura se realiza del buffer circular `hmpu->accel_ringbuffer`.

### Queue de lecturas no bloqueantes

Cuando el usuario lanza una lectura no bloqueante, estas no se ejecutan inmediatamente.
La solicitud se encola, siendo la estructura a encolar:

```c
typedef struct {

} mpu6050_read_queue_t;
```

El callback `mpu6050_hal_i2c_read_cplt_callback` tiene dos tareas:

1. Obtener la siguiente solicitud de lectura no bloqueante de la cola y lanzarla.
Si la cola queda vacía, se limpia el flag de lectura en proceso.
2. Si hay información en el ring buffer del acelerómetro se ejecuta el callback de éste.
3. Si hay información en el ring buffer del giróscopo se ejecuta el callback de éste.
4. Si hay información en el ring buffer del sensor de temperatura se ejecuta el callback de éste.


### Error callback

A definir por el usuario:

```c
__weak void mpu6050_error_callback(void);
```

### A considerar
- Tal vez la flag de lectura no bloqueante en proceso no es necesaria si se puede consultar el estado
del canal de DMA.
- Renombrar variables para indicar privacidad
