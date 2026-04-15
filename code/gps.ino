// ================= SPEED MEASUREMENT USING GPS NEO-7M =================
// Reads speed via UART from GPS module and filters it before sending to Firebase

#include <WiFi.h>
#include <TinyGPS++.h>
#include <Firebase_ESP_Client.h>

#define WIFI_SSID "YOUR_WIFI"
#define WIFI_PASSWORD "YOUR_PASSWORD"
#define API_KEY "YOUR_API_KEY"
#define DATABASE_URL "YOUR_DATABASE_URL"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

TinyGPSPlus gps;

// UART2 for GPS
HardwareSerial gpsSerial(2);

float speed = 0;
float filteredSpeed = 0;

float alpha = 0.8;

unsigned long sendDataPrevMillis = 0;

void setup() {
  Serial.begin(115200);

  // Initialize GPS UART (RX=16, TX=17)
  gpsSerial.begin(9600, SERIAL_8N1, 16, 17);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) delay(500);

  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  Firebase.begin(&config, &auth);
}

void loop() {

  // Read incoming GPS data
  while (gpsSerial.available()) {
    gps.encode(gpsSerial.read());
  }

  // Update only when new speed data is available
  if (gps.speed.isUpdated()) {

    speed = gps.speed.kmph();

    // Apply filtering to smooth fluctuations
    filteredSpeed = alpha * filteredSpeed + (1 - alpha) * speed;

    Serial.print("Speed: ");
    Serial.println(filteredSpeed);

    if (Firebase.ready() && millis() - sendDataPrevMillis > 2000) {
      sendDataPrevMillis = millis();

      Firebase.RTDB.setFloat(&fbdo, "/gps/speed", filteredSpeed);
    }
  }
}