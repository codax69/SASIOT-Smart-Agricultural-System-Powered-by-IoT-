🌱 **SASIOT — Smart Agricultural System Powered by IoT**

SASIOT is a complete IoT-based agriculture automation system built using the ESP32, Blynk IoT, and Firebase Realtime Database.  
It enables farmers to monitor environmental conditions, automate irrigation and lighting, receive alerts, and store data for historical analysis — all from a mobile phone.




 📌 Overview 

SASIOT combines automation and remote monitoring to make farming smarter and more efficient.  
The system:
- Reads environmental data from sensors
- Automates irrigation, lighting, and shade control
- Sends alerts to the user in case of abnormal conditions
- Stores data in Firebase for analysis



 ✨ Key Features

 Monitoring
- 🌡 Temperature & Humidity (DHT11)
- 💧 Soil Moisture Level
- ☀ Light Intensity (LDR)
- 🌧 Rain Detection
- 🪫 Air Quality (MQ-7 Gas Sensor)
- 🌬 Wind Speed (IR Sensor)

 Automation
- Automatic pump control based on soil moisture
- Automatic LED lighting control based on LDR readings
- Automatic servo movement for shade control
- Buzzer alerts for rain, high temperature, and gas detection

 Manual Control
- Control pump, LED, and servo via Blynk mobile app

 Data Management
- Real-time updates to Blynk IoT
- Live and historical data storage in Firebase



 🛠 Hardware Requirements

| Component                   | Quantity | Purpose                |
| --------------------------- | -------- | ---------------------- |
| ESP32 Development Board     | 1        | Main controller        |
| DHT11 Sensor                | 1        | Temperature & humidity |
| MQ-7 Gas Sensor             | 1        | Air quality            |
| LDR Sensor                  | 1        | Light detection        |
| Soil Moisture Sensor        | 1        | Irrigation control     |
| Rain Sensor                 | 1        | Rain detection         |
| IR Sensor                   | 1        | Wind speed             |
| Relay Module (2-channel)    | 1        | Pump & LED control     |
| Servo Motor                 | 1        | Shade control          |
| Buzzer                      | 1        | Alerts                 |
| Power Supply & Jumper Wires | -        | Circuit connections    |




 🔌 ESP32 Pin Configuration

| Component   | Pin |
|-|--|
| MQ-7 Gas Sensor | 34 |
| DHT11 Sensor | 26 |
| Rain Sensor | 33 |
| Soil Moisture Sensor | 32 |
| LDR | 35 |
| Relay (Pump) | 25 |
| Relay (LED) | 23 |
| Buzzer | 22 |
| Servo Motor | 14 |
| IR Sensor | 27 |



 🖼 System Architecture

[Sensors] → [ESP32 Controller] → [Blynk IoT App]
↘
[Firebase RTDB]

- Sensors: Collect environmental readings.
- ESP32: Processes readings and controls devices.
- Blynk IoT App: Displays data & enables manual control.
- Firebase: Stores real-time and historical data.



 ⚙️ Software Setup

1. Clone Repository
   ```bash
   git clone https://github.com/codax69/SASIOT-Smart-Agricultural-System-Powered-by-IoT-.git
2.	Install Required Libraries
o	Blynk
o	DHT Sensor Library
o	Firebase ESP Client
o	ESP32Servo
3.	Update Code Configuration
o	Wi-Fi SSID & Password
o	Blynk Template ID, Name, and Auth Token
o	Firebase API Key & Database URL
o	Firebase Authentication (Email & Password)
4.	Upload to ESP32
o	Select ESP32 Dev Module in Arduino IDE/PlatformIO
o	Compile & upload
5.	Monitor Serial Output
o	View live sensor logs and connection status.
________________________________________
📱 Blynk Virtual Pin Mapping
Virtual Pin	Function
V1	Manual pump control
V9	Manual LED control
V3	Manual servo control
V4	Rain status
V6	Gas sensor reading
V7	Temperature
V8	Humidity
V2	Soil moisture
V5	Light level
V0	Wind speed
________________________________________
🔄 Data Flow
1.	Data Collection → Sensors send readings to ESP32.
2.	Processing → ESP32 applies automation logic.
3.	Control Actions → Relays, servo, and buzzer respond automatically or via manual commands.
4.	Data Upload → Real-time updates sent to Blynk and Firebase.
5.	Historical Logging → Firebase stores past data with timestamps.
________________________________________
🚀 Future Enhancements
•	📡 Integration with weather APIs for predictive irrigation
•	☀ Solar power integration
•	🧪 pH and nutrient sensors
•	🤖 AI-driven crop management suggestions
________________________________________
👨‍💻 Author
Pritesh Patel
📧 Email: prit9265@gmail.com
💻 Powered by: ESP32 • Blynk IoT • Firebase Realtime Database
________________________________________

📒 Note

🤖 AI Assistance in Development

The SASIOT firmware and documentation were developed with the support of AI coding assistants such as **ChatGPT** and **Claude**.  
These tools were used to:
- Generate boilerplate code for ESP32, Blynk, and Firebase integration
- Suggest optimizations for sensor reading and automation logic
- Help in debugging and explaining complex functions
- Create clear, human-readable documentation

All AI-generated code was reviewed, tested, and adapted to meet the specific needs of this project.
