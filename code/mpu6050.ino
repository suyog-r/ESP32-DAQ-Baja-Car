// ================= ACCELERATION MEASUREMENT USING MPU6050 =================
// Measures linear (X-axis) and lateral (Y-axis) acceleration
// Applies calibration and filtering before sending data to Firebase

#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Firebase_ESP_Client.h>

// WiFi + Firebase credentials
#define WIFI_SSID "YOUR_WIFI"
#define WIFI_PASSWORD "YOUR_PASSWORD"
#define API_KEY "YOUR_API_KEY"
#define DATABASE_URL "YOUR_DATABASE_URL"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

Adafruit_MPU6050 mpu;

// Raw acceleration values
float accX, accY;

// Filtered values
float linear_acc = 0;
float lateral_acc = 0;

// Offset values for calibration
float offsetX = 0, offsetY = 0, offsetZ = 0;

// Low-pass filter factor (for noise reduction)
float alpha = 0.8;

unsigned long sendDataPrevMillis = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin();

  // Initialize MPU6050
  if (!mpu.begin()) {
    Serial.println("Error: MPU6050 not detected");
    while (1);
  }

  // ================= CALIBRATION =================
  // Averaging readings to remove sensor offset
  Serial.println("Calibrating MPU6050...");
  for (int i = 0; i < 200; i++) {
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    offsetX += a.acceleration.x;
    offsetY += a.acceleration.y;
    offsetZ += a.acceleration.z;

    delay(10);
  }

  offsetX /= 200;
  offsetY /= 200;
  offsetZ /= 200;

  Serial.println("Calibration complete");

  // ================= WIFI =================
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) delay(500);

  // ================= FIREBASE =================
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() {

  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // Remove offset
  accX = a.acceleration.x - offsetX;
  accY = a.acceleration.y - offsetY;

  // Apply filtering
  linear_acc  = alpha * linear_acc  + (1 - alpha) * accX;
  lateral_acc = alpha * lateral_acc + (1 - alpha) * accY;

  // Debug output
  Serial.print("Linear: ");
  Serial.print(linear_acc);
  Serial.print(" | Lateral: ");
  Serial.println(lateral_acc);

  // Send data every 2 seconds
  if (Firebase.ready() && millis() - sendDataPrevMillis > 2000) {
    sendDataPrevMillis = millis();

    Firebase.RTDB.setFloat(&fbdo, "/acc/linear", linear_acc);
    Firebase.RTDB.setFloat(&fbdo, "/acc/lateral", lateral_acc);
  }
}