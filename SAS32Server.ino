 #define BLYNK_TEMPLATE_ID "TMPL3UWLXBSMd"
#define BLYNK_TEMPLATE_NAME "Smart Agricultural System Using IOT"
#define BLYNK_AUTH_TOKEN "J-kSpp7kbLZpJoBFTbDm2vw_TQDtnaRA"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>
#include <ESP32Servo.h>

// Firebase ESP32 Library
#include <Firebase_ESP_Client.h>
// Provide token generation process info
#include <addons/TokenHelper.h>
// Provide RTDB payload printing info
#include <addons/RTDBHelper.h>

// Firebase database project settings
#define DATABASE_URL "https://sasiot-default-rtdb.asia-southeast1.firebasedatabase.app/"
#define API_KEY "AIzaSyC0Ok-OWBc9QY4OPf1jyGsNXBAWeLA8Bxo"

// Wi-Fi Credentials
char ssid[] = "Pritesh";
char pass[] = "Piyu1618";

// Pin Definitions for ESP32
#define MQ7_PIN 34
#define DHT_PIN 26
#define RAIN_PIN 33
#define SOIL_PIN 32
#define LDR_PIN 35
#define RELAY_MOTOR 25
#define RELAY_LED 23
#define BUZZER_PIN 22
#define SERVO_PIN 14
#define IR_SENSOR_PIN 27

// Define Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Data update interval (milliseconds)
const unsigned long FIREBASE_INTERVAL = 5000;  // Send data every 5 seconds
unsigned long lastFirebaseUpdate = 0;

DHT dht(DHT_PIN, DHT11);
Servo myServo;
volatile int ir_count = 0;

// **Threshold Values for Automation**
int soilMoistureThreshold = 25;  // Percentage threshold
int ldrThreshold = 2000;
int mq7Threshold = 700;  // Example value for CO detection

// **Variables for Manual Control (from Blynk)**
bool manualPumpControl = false;
bool manualServoControl = false;
bool manualLEDControl = false;

// **Blynk Virtual Pins**
#define VP_MOTOR V1
#define VP_LED V9
#define VP_SERVO V3
#define VP_NOTIFICATION V4

BLYNK_WRITE(VP_MOTOR) {
  manualPumpControl = param.asInt();
  Serial.print(manualPumpControl);
  digitalWrite(RELAY_MOTOR, manualPumpControl ? HIGH : LOW);
}

BLYNK_WRITE(VP_LED) {
  manualLEDControl = param.asInt();
  Serial.print(manualLEDControl);
  digitalWrite(RELAY_LED, manualLEDControl ? HIGH : LOW);
}

BLYNK_WRITE(VP_SERVO) {
  manualServoControl = param.asInt();
  Serial.print(manualServoControl);
  if (manualServoControl) {
    smoothMoveServo(70);
    beepBuzzer(4);
  } else {
    smoothMoveServo(0);
  }
}
void IRAM_ATTR IR_interrupt() {
  ir_count++;
}

// **Trigger Buzzer 3 Times**
void beepBuzzer(int beepNum) {
  for (int i = 0; i < beepNum; i++) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(200);
    digitalWrite(BUZZER_PIN, LOW);
    delay(300);
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("\n\nESP32 Smart Agricultural System");
  Serial.println("-------------------------------");

  dht.begin();
  myServo.attach(SERVO_PIN);
  myServo.write(45);
  pinMode(RELAY_MOTOR, OUTPUT);
  pinMode(RELAY_LED, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(IR_SENSOR_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(IR_SENSOR_PIN), IR_interrupt, RISING);

  // Connect to WiFi
  WiFi.begin(ssid, pass);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }

  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  // Start Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Configure Firebase
  Serial.println("Setting up Firebase...");
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  auth.user.email = "prit9265@gmail.com";
  auth.user.password = "Pritesh";

  // Enable token status callback
  config.token_status_callback = tokenStatusCallback;

  // Initialize Firebase
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  // Set database read timeout to 1 minute
  Firebase.RTDB.setReadTimeout(&fbdo, 1000 * 60);

  Serial.println("Firebase setup complete!");
}

void loop() {
  Blynk.run();

  int mq7_value = analogRead(MQ7_PIN);
  int soil_value = analogRead(SOIL_PIN);
  int ldr_value = analogRead(LDR_PIN);
  int rain_value = analogRead(RAIN_PIN);
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  float wind_speed = ir_count * 5 /1000 ;

  // **Convert Soil Moisture and LDR to Percentage**
  int soil_percentage = map(soil_value, 4095, 0, 0, 100);  // Invert and convert to percentage
  int light_percentage = map(ldr_value, 0, 4095, 0, 100);
  // **Serial Print Statements for Sensor Values**
  Serial.println("\n---- Sensor Data ----");
  Serial.print("MQ-7 Gas Sensor: ");
  Serial.println(mq7_value);

  Serial.print("Soil Moisture (%): ");
  Serial.println(soil_percentage);

  Serial.print("Light Sensor (LDR): ");
  Serial.println(light_percentage);

  Serial.print("Rain Sensor: ");
  Serial.println(rain_value);

  Serial.print("Temperature (C): ");
  Serial.println(temperature);

  Serial.print("Humidity (%): ");
  Serial.println(humidity);

  Serial.print("Wind Speed (m/s): ");
  Serial.println(wind_speed);

  Serial.println("---------------------");

  // **Water Pump Control**
  // **Soil Moisture Pump Control**
  // Both manual override and automatic control based on soil moisture
  if (!manualPumpControl && soil_percentage > 25) {  // Example threshold 25%
    digitalWrite(RELAY_MOTOR, HIGH);
    Blynk.logEvent("low_soil_mositure", "Low Soil Moisture - Pump ON!");

  } else {
    digitalWrite(RELAY_MOTOR, LOW);
    Serial.print("Low Soil Moisture - Pump ON!");
  }

  // **Rain Detection & Buzzer Alert**
  if (rain_value <= LOW) {
    beepBuzzer(2);
    Blynk.logEvent("rain", "Rain Detected!");
  }

  // **Low Light LED & Servo Control**
  if (!manualLEDControl && ldr_value < 2000) {
    digitalWrite(RELAY_LED, HIGH);
  } else {
    digitalWrite(RELAY_LED, LOW);
  }

  // **High Temperature Notification**
  if (temperature > 30) {
    Blynk.logEvent("high_temperature", "High Temperature!");
  }

  if (!manualServoControl && ldr_value > 2000) {
    smoothMoveServo(70);
    beepBuzzer(4);
    Serial.println("High Light - Adjusting Servo!");
  } else if (!manualServoControl) {
    smoothMoveServo(0);
  } else if (rain_value < 2000) {
    smoothMoveServo(0);
  } else if (rain_value > 2000) {
    smoothMoveServo(70);
    beepBuzzer(4);
  }
  if(mq7_value > 1000){
    beepBuzzer(6);
  }
  // **Send Sensor Data to Blynk**
  Blynk.virtualWrite(V6, mq7_value);
  Blynk.virtualWrite(V7, temperature);
  Blynk.virtualWrite(V8, humidity);
  Blynk.virtualWrite(V2, soil_percentage);
  Blynk.virtualWrite(V4, rain_value);
  Blynk.virtualWrite(V5, ldr_value);
  Blynk.virtualWrite(V0, wind_speed);

  // Send data to Firebase at defined intervals
  if (millis() - lastFirebaseUpdate >= FIREBASE_INTERVAL) {
    lastFirebaseUpdate = millis();

    // Check if Firebase is ready before sending data
    if (Firebase.ready()) {
      bool success = true;

      // Using individual path method
      success &= Firebase.RTDB.setFloat(&fbdo, "/sensor_data/temperature", temperature);
      success &= Firebase.RTDB.setFloat(&fbdo, "/sensor_data/humidity", humidity);
      success &= Firebase.RTDB.setInt(&fbdo, "/sensor_data/soil_moisture", soil_percentage);
      success &= Firebase.RTDB.setInt(&fbdo, "/sensor_data/light", ldr_value);
      success &= Firebase.RTDB.setInt(&fbdo, "/sensor_data/rain", rain_value);
      success &= Firebase.RTDB.setFloat(&fbdo, "/sensor_data/wind_speed", wind_speed);
      success &= Firebase.RTDB.setInt(&fbdo, "/sensor_data/air_quality", mq7_value);

      // Store timestamp (seconds since boot)
      unsigned long timestamp = millis() / 1000;
      success &= Firebase.RTDB.setInt(&fbdo, "/sensor_data/timestamp", timestamp);

      // Store all data in a timestamped node for historical data
      FirebaseJson json;
      json.set("temperature", temperature);
      json.set("humidity", humidity);
      json.set("soil_moisture", soil_percentage);
      json.set("light", ldr_value);
      json.set("rain", rain_value);
      json.set("wind_speed", wind_speed);
      json.set("air_quality", mq7_value);

      Serial.println("JSON Data:");
      json.toString(Serial, true);

      // Push to history node with timestamp as child name
      String historyPath = "/sensor_history/" + String(timestamp);
      success &= Firebase.RTDB.setJSON(&fbdo, historyPath.c_str(), &json);

      // Report success or failure
      if (success) {
        Serial.println("Data sent to Firebase successfully!");
      } else {
        Serial.println("Failed to send data to Firebase");
        if (fbdo.errorReason() != "") {
          Serial.println("Reason: " + fbdo.errorReason());
        }
      }
    } else {
      Serial.println("Firebase not ready. Skipping data upload.");
      Serial.print("Firebase connection status: ");
      Serial.println(fbdo.errorReason());
    }
  }

  // Reset IR count after reading
  ir_count = 0;

  // Small delay to prevent resource hogging
  delay(3000);
}
void smoothMoveServo(int targetPosition) {
  int currentPosition = myServo.read();
  if (currentPosition < targetPosition) {
    for (int pos = currentPosition; pos <= targetPosition; pos++) {
      myServo.write(pos);
      delay(30);
    }
  } else if (currentPosition > targetPosition) {
    for (int pos = currentPosition; pos >= targetPosition; pos--) {
      myServo.write(pos);
      delay(30);
    }
  }
}