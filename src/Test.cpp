#include <Arduino.h>

// Teensy 4.1 + AT8236 dual motor driver + 2 quadrature encoders

// ---------------- Motor driver pins ----------------
const int AIN1 = 2;
const int AIN2 = 3;

const int BIN1 = 4;
const int BIN2 = 5;

// ---------------- Encoder pins ----------------
const int E1A = 6;
const int E1B = 7;

const int E2A = 8;
const int E2B = 9;

// ---------------- Encoder settings ----------------
// CHANGE THIS to match your motor encoder.
// Example: if encoder gives 11 pulses per motor shaft revolution, use 11.
// If gearbox motor gives pulses at motor shaft, multiply by gear ratio.
const float PULSES_PER_REV = 800;

// ---------------- PID settings ----------------
float targetRPM_A = -340.0;
float targetRPM_B = 60.0;

// Start with small values, then tune
float Kp = 0.35;
float Ki = 0.0;
float Kd = 0.0;

const unsigned long PID_INTERVAL_MS = 100;

// PWM range for Teensy
const int PWM_MAX = 255;

// ---------------- Encoder counters ----------------
volatile long encoderCountA = 0;
volatile long encoderCountB = 0;

long lastCountA = 0;
long lastCountB = 0;

unsigned long lastPIDTime = 0;

// ---------------- PID variables ----------------
float errorA = 0;
float lastErrorA = 0;
float integralA = 0;

float errorB = 0;
float lastErrorB = 0;
float integralB = 0;

float pwmA = 0;
float pwmB = 0;

// ---------------- Encoder ISRs ----------------
void encoderA_ISR() {
  bool a = digitalRead(E1A);
  bool b = digitalRead(E1B);

  if (a == b) {
    encoderCountA--;
  } else {
    encoderCountA++;
  }
}

void encoderB_ISR() {
  bool a = digitalRead(E2A);
  bool b = digitalRead(E2B);

  if (a == b) {
    encoderCountB--;
  } else {
    encoderCountB++;
  }
}

// ---------------- Motor control functions ----------------
void setMotorA(int pwm) {
  pwm = constrain(pwm, -PWM_MAX, PWM_MAX);

  if (pwm > 0) {
    analogWrite(AIN1, pwm);
    analogWrite(AIN2, 0);
  } else if (pwm < 0) {
    analogWrite(AIN1, 0);
    analogWrite(AIN2, -pwm);
  } else {
    analogWrite(AIN1, 0);
    analogWrite(AIN2, 0);
  }
}

void setMotorB(int pwm) {
  pwm = constrain(pwm, -PWM_MAX, PWM_MAX);

  if (pwm > 0) {
    analogWrite(BIN1, pwm);
    analogWrite(BIN2, 0);
  } else if (pwm < 0) {
    analogWrite(BIN1, 0);
    analogWrite(BIN2, -pwm);
  } else {
    analogWrite(BIN1, 0);
    analogWrite(BIN2, 0);
  }
}

// ---------------- Setup ----------------
void setup() {
  Serial.begin(115200);

  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);

  pinMode(E1A, INPUT_PULLUP);
  pinMode(E1B, INPUT_PULLUP);
  pinMode(E2A, INPUT_PULLUP);
  pinMode(E2B, INPUT_PULLUP);

  analogWriteResolution(8);
  analogWriteFrequency(AIN1, 20000);
  analogWriteFrequency(AIN2, 20000);
  analogWriteFrequency(BIN1, 20000);
  analogWriteFrequency(BIN2, 20000);

  attachInterrupt(digitalPinToInterrupt(E1A), encoderA_ISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(E2A), encoderB_ISR, CHANGE);

  lastPIDTime = millis();

  Serial.println("Motor PID RPM control started");
}

// ---------------- Main loop ----------------
void loop() {
  unsigned long now = millis();

  if (now - lastPIDTime >= PID_INTERVAL_MS) {
    float dt = (now - lastPIDTime) / 1000.0;
    lastPIDTime = now;

    noInterrupts();
    long currentCountA = encoderCountA;
    long currentCountB = encoderCountB;
    interrupts();

    long deltaA = currentCountA - lastCountA;
    long deltaB = currentCountB - lastCountB;

    lastCountA = currentCountA;
    lastCountB = currentCountB;

    // RPM = revolutions per minute
    float rpmA = (deltaA / PULSES_PER_REV) * (60.0 / dt);
    float rpmB = (deltaB / PULSES_PER_REV) * (60.0 / dt);

    // ---------------- PID for Motor A ----------------
    errorA = targetRPM_A - rpmA;
    integralA += errorA * dt;
    float derivativeA = (errorA - lastErrorA) / dt;
    lastErrorA = errorA;

    pwmA += Kp * errorA + Ki * integralA + Kd * derivativeA;
    pwmA = constrain(pwmA, -PWM_MAX, PWM_MAX);

    // ---------------- PID for Motor B ----------------
    errorB = targetRPM_B - rpmB;
    integralB += errorB * dt;
    float derivativeB = (errorB - lastErrorB) / dt;
    lastErrorB = errorB;

    pwmB += Kp * errorB + Ki * integralB + Kd * derivativeB;
    pwmB = constrain(pwmB, -PWM_MAX, PWM_MAX);

    setMotorA((int)pwmA);
    setMotorB((int)pwmB);

    // Debug output
    Serial.print("Target A: ");
    Serial.print(targetRPM_A);
    Serial.print(" | RPM A: ");
    Serial.print(rpmA);
    Serial.print(" | PWM A: ");
    Serial.print(pwmA);

    Serial.print(" || Target B: ");
    Serial.print(targetRPM_B);
    Serial.print(" | RPM B: ");
    Serial.print(rpmB);
    Serial.print(" | PWM B: ");
    Serial.println(pwmB);
  }

  // Example: change target RPM from Serial Monitor
  // Send: A150 or B-120
  if (Serial.available()) {
    char motor = Serial.read();
    float value = Serial.parseFloat();

    if (motor == 'A' || motor == 'a') {
      targetRPM_A = value;
      integralA = 0;
      Serial.print("New target RPM A: ");
      Serial.println(targetRPM_A);
    }

    if (motor == 'B' || motor == 'b') {
      targetRPM_B = value;
      integralB = 0;
      Serial.print("New target RPM B: ");
      Serial.println(targetRPM_B);
    }
  }
}