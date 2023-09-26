#include "fliter_library.h"

/*
 * @brief Convierte los datos obtenidos del acelerómetro en ángulos de Euler, para el caso de yaw
 * equivale 0.
 * @param data_imu Contiene los valores de aceleración lineal.
 * @return Valores de la estimación de los ángulos de Euler.
 */
struct euler_estimation accelerometer_estimation(struct data_imu linear_acceleration) {
    struct euler_estimation estimation;

    estimation.pitch = atan(linear_acceleration.y / linear_acceleration.z);
    estimation.roll = asin(linear_acceleration.x / gravity);
    estimation.yaw = 0;

    return estimation;
}

/*
 * @brief Convierte los datos obtenidos del giroscopio en ángulos de Euler, para el caso de yaw
 * equivale 0.
 * @param angular_velocity Contiene los valores de velocidad angular.
 * @param prev_estimation Contiene las últimas estimaciones en la salida del filtro complementario.
 * @return Valores de la estimación de los ángulos de Euler.
 */
struct euler_estimation gyroscope_estimation(struct data_imu angular_velocity,
                                             struct euler_estimation prev_estimation) {
    struct euler_estimation estimation;

    estimation.pitch = angular_velocity.x + sin(prev_estimation.roll) *
                                                (angular_velocity.y * sin(prev_estimation.pitch) +
                                                 angular_velocity.z * cos(prev_estimation.pitch));
    estimation.roll = angular_velocity.y * cos(prev_estimation.roll) -
                      angular_velocity.z * sin(prev_estimation.pitch);
    estimation.yaw = 0;

    return estimation;
}

/*
 * @brief Realiza una integración por el método del trapecio.
 * @param prev_output_value Valor acumulado anterior de la integral.
 * @param current_value Valor del punto actual al que se va a integrar (n).
 * @param prev_value Valor del punto anterior a la integral (n + 1).
 * @param d_time Tiempo transcurrido entre prev_value y current_value.
 * @return Valor acumulado de la integral.
 */
double integration_trapezoidal_rule(double prev_output_value, double current_value,
                                    double prev_value, float d_time) {
    return prev_output_value + (current_value + prev_value) / 2 * d_time;
}

/*
 * @brief Realiza una integración por el método de Riemann.
 * @param prev_output_value Valor acumulado anterior de la integral.
 * @param current_value Valor del punto actual al que se va a integrar (n).
 * @param d_time Tiempo transcurrido entre current_value y el punto anterior calculado.
 * @return Valor acumulado de la integral.
 */
double integration_riemann(double prev_output_value, double current_value, float d_time) {
    return prev_output_value + current_value * d_time;
}

/*
 * @brief Implementa el filtro complementario con las estimaciones del acelerómetro y giroscopio,
 * actualizarán los valores de la estimación previa.
 * @param accelerometer_estimation Valores de estimación del acelerómetro.
 * @param gyroscope_estimation Valores de estimación del giroscopio.
 * @param gyroscope_estimation_prev Valores de estimación previos de la estimación del giroscopio,
 * utilizado en la integración trapezoidal.
 * @param prev_estimation Valores de estimación del filtro complementario de la iteración anterior.
 * @param d_time Tiempo transcurrido desde la estimación anterior a la actual.
 * @param alpha Constante de ponderación.
 * @return Valores de la estimación, calculados con el filtro complementario
 */
struct euler_estimation filter_complementary(struct euler_estimation accelerometer_estimation,
                                             struct euler_estimation gyroscope_estimation,
                                             struct euler_estimation gyroscope_estimation_prev,
                                             struct euler_estimation prev_estimation, float d_time,
                                             float alpha) {
    struct euler_estimation estimation;

    estimation.pitch =
        (1 - alpha) * accelerometer_estimation.pitch +
        alpha * integration_trapezoidal_rule(prev_estimation.pitch, gyroscope_estimation_prev.pitch,
                                             gyroscope_estimation.pitch, d_time);
    estimation.roll =
        (1 - alpha) * accelerometer_estimation.roll +
        alpha * integration_trapezoidal_rule(prev_estimation.roll, gyroscope_estimation_prev.roll,
                                             gyroscope_estimation.roll, d_time);
    estimation.yaw = integration_trapezoidal_rule(prev_estimation.yaw, gyroscope_estimation.yaw,
                                                  gyroscope_estimation_prev.yaw, d_time);

    return estimation;
}

/*
 * @brief Obtiene el resultado de realizar el filtro complementario, es necesario que maneje
 * correctamente los valores previos de cada iteración al utilizar el filtro complementario, en caso
 * de ser la primer vuelta prev_estimation y prev_gyroscope_estimation tendrán por valores 0.
 * @param linear_acceleration Valor actual de la aceleración lineal, probablemente obtenida de un
 * acelerómetro.
 * @param angular_velocity Valor actual de la velocidad angular, probablemente obtenida de un
 * giroscopio.
 * @param *prev_gyroscope_estimation Valores de la estimación del giroscopio en la iteración
 * anterior, en caso de ser la primer vueltas sus valores deben ser 0, estos valores serán
 * actualizados.
 * @param prev_estimation Valores de la estimación del filtro complementario en la iteración
 * anterior, en caso de ser la primer vueltas sus valores deben ser 0.
 * @param d_time Lapso de tiempo transcurrido entre la obtención de los valores del giroscopio y
 * acelerómetro de la iteración anterior y la actual.
 * @param alpha Constante de ponderación.
 * @return Valores actuales de los ángulos de euler, de ser necesario se debe almacenar para la
 * proxima iteración.
 */
struct euler_estimation
get_angular_euler_whit_f_c(struct data_imu linear_acceleration, struct data_imu angular_velocity,
                           struct euler_estimation *prev_gyroscope_estimation,
                           struct euler_estimation prev_estimation, float d_time, float alpha) {
    struct euler_estimation out_estimation = {0.0, 0.0, 0.0};
    struct euler_estimation accelerometer_estim = accelerometer_estimation(linear_acceleration);
    struct euler_estimation gyroscope_estim =
        gyroscope_estimation(angular_velocity, prev_estimation);

    gyroscope_estim.yaw = angular_velocity.z;

    out_estimation =
        filter_complementary(accelerometer_estim, gyroscope_estim, *prev_gyroscope_estimation,
                             prev_estimation, d_time, alpha);

    prev_gyroscope_estimation->pitch = gyroscope_estim.pitch;
    prev_gyroscope_estimation->roll = gyroscope_estim.roll;
    prev_gyroscope_estimation->yaw = gyroscope_estim.yaw;

    return out_estimation;
}
