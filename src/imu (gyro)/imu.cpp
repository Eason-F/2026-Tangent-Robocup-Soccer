#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28); // change to 0x29 if ADR high

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 2000) {}

  Wire.begin();                 // Teensy: SDA=18, SCL=19 by default
  Wire.setClock(400000);        // fast I2C (works well on Teensy)

  if (!bno.begin()) {
    Serial.println("BNO055 not detected. Check wiring / address.");
    while (1) delay(10);
  }

  delay(50);

  // Use fused orientation (gyro+accel+mag). Best general mode:
  bno.setExtCrystalUse(true);   // if your breakout has a 32.768kHz crystal (most do)

  // Optional: print calibration status at start
  uint8_t sys, gyro, accel, mag;
  bno.getCalibration(&sys, &gyro, &accel, &mag);
  Serial.printf("Calib SYS:%u G:%u A:%u M:%u\n", sys, gyro, accel, mag);
}

void loop() {
  // Orientation (Euler, degrees)
  imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
  Serial.printf("Euler: H=%.2f R=%.2f P=%.2f  ", euler.x(), euler.z(), euler.y());

  // Quaternion (unitless, good for avoiding gimbal lock)
  imu::Quaternion q = bno.getQuat();
  Serial.printf("Quat: w=%.4f x=%.4f y=%.4f z=%.4f  ", q.w(), q.x(), q.y(), q.z());

  // Raw-ish sensors (units depend on BNO055 internal scaling; library returns sensible units)
  imu::Vector<3> accel = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER); // m/s^2
  imu::Vector<3> gyro  = bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);     // rad/s
  imu::Vector<3> mag   = bno.getVector(Adafruit_BNO055::VECTOR_MAGNETOMETER);  // uT

  Serial.printf("A(%.2f,%.2f,%.2f) G(%.3f,%.3f,%.3f) M(%.2f,%.2f,%.2f)\n",
                accel.x(), accel.y(), accel.z(),
                gyro.x(), gyro.y(), gyro.z(),
                mag.x(), mag.y(), mag.z());

  // Calibration status (0..3 each)
  uint8_t sys, g, a, m;
  bno.getCalibration(&sys, &g, &a, &m);
  Serial.printf("Calib SYS:%u G:%u A:%u M:%u\n\n", sys, g, a, m);

  delay(100);
}