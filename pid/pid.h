#ifndef INCLUDED_PID_H_
#define INCLUDED_PID_H_

#include <chrono>
#include <stdexcept>

class PIDController {
public:
    PIDController(double kp, double ki, double kd)
            : kp(kp), ki(ki), kd(kd), integral(0.0), previous_error(0.0),
              previous_time(std::chrono::steady_clock::time_point::min()) {}

    void start() {
        integral = 0.0;
        previous_error = 0.0;
        previous_time = std::chrono::steady_clock::now();
    }

    double update(double set_point, double measured_value) {
        auto current_time = std::chrono::steady_clock::now();
        if (previous_time == std::chrono::steady_clock::time_point::min()) {
            throw std::runtime_error("PIDController has not been started. Call start() before update().");
        }

        std::chrono::duration<double> dt_duration = current_time - previous_time;
        double dt = dt_duration.count();

        double error = set_point - measured_value;

        double p = kp * error;

        integral += error * dt;
        double i = ki * integral;

        double derivative = (error - previous_error) / dt;
        double d = kd * derivative;

        previous_time = current_time;
        previous_error = error;

        return p + i + d;
    }

    double getKp(){ return kp; }
    double getKi(){ return ki; }
    double getKd(){ return kd; }

    void setKp(double Kp){ kp = Kp; }
    void setKi(double Ki){ ki = Ki; }
    void setKd(double Kd){ kd = Kd; }

private:
    double kp;
    double ki;
    double kd;
    double integral;
    double previous_error;
    std::chrono::steady_clock::time_point previous_time;
};

#endif