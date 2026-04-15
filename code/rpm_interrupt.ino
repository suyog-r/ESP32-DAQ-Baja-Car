// ================= RPM MEASUREMENT USING LM393 =================
// Counts pulses using interrupt and calculates RPM
// Uses filtering for stable output

#define RPM_PIN 4
#define PPR 20   // Pulses per revolution

volatile int pulseCount = 0;

unsigned long lastTime = 0;

float rpm = 0;
float filteredRPM = 0;

float alpha = 0.8;

// Interrupt function (triggered on each pulse)
void IRAM_ATTR countPulse() {
  pulseCount++;
}

void setup() {
  Serial.begin(115200);

  pinMode(RPM_PIN, INPUT_PULLUP);

  // Attach interrupt on falling edge
  attachInterrupt(digitalPinToInterrupt(RPM_PIN), countPulse, FALLING);
}

void loop() {

  // Calculate RPM every 1 second
  if (millis() - lastTime >= 1000) {

    // RPM formula
    rpm = (pulseCount * 60.0) / PPR;

    // Apply filtering
    filteredRPM = alpha * filteredRPM + (1 - alpha) * rpm;

    // Reset count
    pulseCount = 0;
    lastTime = millis();

    Serial.print("RPM: ");
    Serial.println(filteredRPM);
  }
}