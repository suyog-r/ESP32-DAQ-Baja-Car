#include <Wire.h>
#include <RTClib.h>

RTC_DS3231 rtc;

const int ldrPin = A0;  // LDR connected to A0
int ldrValue = 0;       // Variable to store LDR value
unsigned long startTime = 0;
unsigned long endTime = 0;
bool laserCut = false;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  // Set the RTC to the current time only once
  rtc.adjust(DateTime(2024, 11, 9, 8, 18, 0));  // Set date to Nov 9, 2024, and time to 08:18:00
}

void loop() {
  ldrValue = analogRead(ldrPin);  // Read LDR value
  
  // Laser cut condition (no light detected on LDR)
  if (ldrValue > 500 && !laserCut) {
    laserCut = true;
    startTime = millis();  // Store the time when laser is cut
    Serial.println("Laser Cut Detected!");
    
    // Get RTC time
    DateTime now = rtc.now();
    Serial.print("Laser cut started at: ");
    Serial.print(now.hour(), DEC);
    Serial.print(":");
    Serial.print(now.minute(), DEC);
    Serial.print(":");
    Serial.println(now.second(), DEC);
  }
  
  // Laser restored condition (light detected on LDR)
  if (ldrValue < 500 && laserCut) {
    laserCut = false;
    endTime = millis();  // Store the time when laser is restored
    unsigned long duration = endTime - startTime;  // Calculate duration of interruption
    Serial.print("Laser Restored after ");
    Serial.print(duration / 1000.0, 4);  // Duration in seconds with 4 decimal places
    Serial.println(" seconds");
    
    // Get RTC time
    DateTime now = rtc.now();
    Serial.print("Laser restored at: ");
    Serial.print(now.hour(), DEC);
    Serial.print(":");
    Serial.print(now.minute(), DEC);
    Serial.print(":");
    Serial.println(now.second(), DEC);
  }
  
  delay(50);  // Delay for stability
}
