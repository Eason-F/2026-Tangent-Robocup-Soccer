#pragma once

class PIDController {
    private:
        const float kP = 0;
        const float kI = 0;
        const float kD = 0;
        const float max = 0.0f;
        const float min = 0.0f;

        float value;
        float lastError;
        float integral;

    public:
        PIDController(const float &kP, const float &kI, const float &kD, const float &min, const float &max);

        float adjustmentValue(const float &dt, const float &target, const float &current);
};
