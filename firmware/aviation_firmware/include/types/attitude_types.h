#ifndef TYPES_ATTITUDE_TYPES_H
#define TYPES_ATTITUDE_TYPES_H

namespace Types {
    namespace Attitude {

        // Holds angles
        struct AttitudeState {
            double roll_deg;
            double pitch_deg;
        };

        // Holds complementary filter state for one axis
        struct ComplementaryAxisState {
            double angle_deg;
            bool initialized;
        };

        // Holds complementary filter tunables and state
        struct ComplementaryFilterState {
            double alpha;
            ComplementaryAxisState roll;
            ComplementaryAxisState pitch;
        };

        // Holds Kalman filter state for one axis
        struct KalmanAxisState {
            double angle_deg;
            double bias_dps;
            double rate_dps;
            double error_covariance_00;
            double error_covariance_01;
            double error_covariance_10;
            double error_covariance_11;
            bool initialized;
        };

        // Holds Kalman filter tunables and state
        struct KalmanFilterState {
            double process_noise_angle;
            double process_noise_bias;
            double measurement_noise;
            KalmanAxisState roll;
            KalmanAxisState pitch;
        };

    }
}

#endif
