// ================= TEMPERATURE MEASUREMENT USING MLX90614 =================
// Measures object (disc/CVT) and ambient temperature using IR sensor
// Applies filtering before sending data to Firebase

#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <Firebase_ESP_Client.h>

#define WIFI_SSID "YOUR_WIFI"
#define WIFI_PASSWORD "YOUR_PASSWORD"
#define API_KEY "YOUR_API_KEY"
#define DATABASE_URL "YOUR_DATABASE_URL"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

Adafruit_MLX90614 mlx;

// Raw values
float objectTemp, ambientTemp;

// Filtered values
float fObjectTemp = 0;
float fAmbientTemp = 0;

float alpha = 0.8;

unsigned long sendDataPrevMillis = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin();

  // Initialize MLX90614
  if (!mlx.begin()) {
    Serial.println("Error: MLX90614 not detected");
    while (1);
  }

  // WiFi setup
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) delay(500);

  // Firebase setup
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  Firebase.begin(&config, &auth);
}

void loop() {

  // Read temperatures
  objectTemp = mlx.readObjectTempC();
  ambientTemp = mlx.readAmbientTempC();

  // Apply filtering
  fObjectTemp = alpha * fObjectTemp + (1 - alpha) * objectTemp;
  fAmbientTemp = alpha * fAmbientTemp + (1 - alpha) * ambientTemp;

  Serial.print("Object Temp: ");
  Serial.print(fObjectTemp);
  Serial.print(" | Ambient Temp: ");
  Serial.println(fAmbientTemp);

  // Send to Firebase
  if (Firebase.ready() && millis() - sendDataPrevMillis > 2000) {
    sendDataPrevMillis = millis();

    Firebase.RTDB.setFloat(&fbdo, "/temp/object", fObjectTemp);
    Firebase.RTDB.setFloat(&fbdo, "/temp/ambient", fAmbientTemp);
  }
}