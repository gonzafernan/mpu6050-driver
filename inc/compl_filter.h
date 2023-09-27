#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

struct data_imu {
    double x;
    double y;
    double z;
};

struct euler_estimation {
    double pitch;
    double roll;
    double yaw;
};

const double gravity = 9.80665;

struct euler_estimation accelerometer_estimation(struct data_imu linear_acceleration);
struct euler_estimation gyroscope_estimation(struct data_imu angular_velocity,
                                             struct euler_estimation prev_estimation);
double integration_trapezoidal_rule(double prev_output_value, double current_value,
                                    double prev_value, float d_time);
double integration_riemann(double prev_output_value, double current_value, float d_time);
struct euler_estimation filter_complementary(struct euler_estimation accelerometer_estimation,
                                             struct euler_estimation gyroscope_estimation,
                                             struct euler_estimation gyroscope_estimation_prev,
                                             struct euler_estimation prev_estimation, float d_time,
                                             float alpha);
struct euler_estimation
get_angular_euler_whit_f_c(struct data_imu linear_acceleration, struct data_imu angular_velocity,
                           struct euler_estimation *prev_gyroscope_estimation,
                           struct euler_estimation prev_estimation, float d_time, float alpha);
#ifdef __cplusplus
}
#endif
