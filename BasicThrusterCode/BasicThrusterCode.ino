#include <Arduino.h>

// ---- Pin and PWM config ----
const int LEFT_PIN  = 18;
const int RIGHT_PIN = 19;

const int PWM_FREQ = 50;        // 50 Hz = 20 ms period
const int PWM_BITS = 16;        // 16-bit resolution

const int PULSE_MIN  = 1000;    // full reverse (us)
const int PULSE_STOP = 1500;    // neutral (us)
const int PULSE_MAX  = 2000;    // full forward (us)

// Limit power while testing. 1.0 = full range, 0.3 = 30%.
const float THROTTLE_LIMIT = 0.3;

// Convert a microsecond pulse width to LEDC duty
uint32_t usToDuty(int us) {
  return (uint32_t)((us / 20000.0) * ((1 << PWM_BITS) - 1));
}

void writePulse(int pin, int us) {
  us = constrain(us, PULSE_MIN, PULSE_MAX);
  ledcWrite(pin, usToDuty(us));
}

// throttle: -1.0 (full reverse) to +1.0 (full forward)
void setThruster(int pin, float throttle) {
  throttle = constrain(throttle, -1.0, 1.0) * THROTTLE_LIMIT;
  int us = PULSE_STOP + (int)(throttle * 500);
  writePulse(pin, us);
}

// Convenience: drive both thrusters
void setThrusters(float left, float right) {
  setThruster(LEFT_PIN, left);
  setThruster(RIGHT_PIN, right);
}

// Differential steering: speed and turn both in -1.0..+1.0
// turn > 0 steers right (left thruster faster than right)
void drive(float speed, float turn) {
  float left  = speed + turn;
  float right = speed - turn;
  // Keep within range while preserving the ratio
  float maxMag = max(1.0f, max(fabs(left), fabs(right)));
  setThrusters(left / maxMag, right / maxMag);
}

void setup() {
  Serial.begin(115200);

  // Core 3.x API: attach pin directly with freq and resolution
  ledcAttach(LEFT_PIN,  PWM_FREQ, PWM_BITS);
  ledcAttach(RIGHT_PIN, PWM_FREQ, PWM_BITS);

  // --- ESC initialisation (per the diagram) ---
  // 1. Send neutral right away, then power the ESCs.
  setThrusters(0, 0);
  Serial.println("Power on ESCs now. Waiting for 3 beeps...");
  delay(5000);

  // 2. Max-speed signal: ESC gives one beep
  writePulse(LEFT_PIN, PULSE_MAX);
  writePulse(RIGHT_PIN, PULSE_MAX);
  delay(1500);

  // 3. Stop signal (1.5 ms): ESC gives one beep
  setThrusters(0, 0);
  delay(1500);

  Serial.println("Initialised. Ready.");
}

void loop() {
  // Demo: forward, stop, turn right, stop, reverse, stop
  drive(0.5, 0);    delay(3000);
  drive(0, 0);      delay(2000);
  drive(0.4, 0.4);  delay(3000);
  drive(0, 0);      delay(2000);
  drive(-0.5, 0);   delay(3000);
  drive(0, 0);      delay(2000);
}
