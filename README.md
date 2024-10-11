-------- --------------- > > > >   CO2, CO, NH3, and Alcohol Monitoring System using ESP8266, MQ135, DHT11, and ThingSpeak < < < < --------------- -----------

- - - - - Overview - - - -
This project uses an ESP8266 NodeMCU microcontroller and an MQ135 gas sensor to monitor air quality by measuring CO2, CO, NH3, and alcohol concentrations.
Additionally, the DHT11 sensor is used to capture temperature and humidity levels. The data is sent to ThingSpeak, and visual indicators (LEDs) provide real-time feedback on CO2 levels.

- - - - - Features - - - -
Real-time air quality monitoring for CO2, CO, NH3, and alcohol concentrations.
Temperature and humidity monitoring using the DHT11 sensor.
LED indicators to alert based on CO2 concentration:
Green LED for low CO2 (< 1000 ppm).
Yellow LED for moderate CO2 (1000–2000 ppm).
Red LED for high CO2 (> 2000 ppm).
Data logging to ThingSpeak for remote monitoring and visualization of the sensor data.

- - - - - Hardware Components - - - -
ESP8266 NodeMCU
MQ135 Gas Sensor (for CO2, CO, NH3, Alcohol)
DHT11 Temperature and Humidity Sensor
3 LEDs (Red, Yellow, Green) for CO2 level indication
Resistors, Jumper wires, Breadboard

- - - - - Circuit Diagram - - - -
Connect MQ135 sensor's analog output to the A0 pin of ESP8266.
Connect DHT11 data pin to D3 of the ESP8266.
Connect Red LED to D1, Green LED to D2, and Yellow LED to D5 pins of ESP8266.
Ensure power and ground connections for all components.

- - - - - Software Requirements - - - -
Arduino IDE with the ESP8266 board package installed.
Libraries:
DHT.h for the DHT11 sensor
ESP8266WiFi.h for WiFi connectivity
ThingSpeak.h for data transmission to ThingSpeak

- - - - - Configuration - - - -
WiFi credentials: Update the ssid and password variables with your WiFi network's details.
ThingSpeak API Key and Channel: Set the correct apiKey and channelID for your ThingSpeak channel.

- - - - - Code Explanation - - - -
MQ135 Sensor: The sensor output is read, converted to voltage, and used to calculate the concentrations of gases (CO2, CO, NH3, Alcohol).
DHT11 Sensor: The DHT11 sensor provides temperature and humidity readings.
LED Indicators: The LEDs light up based on CO2 concentration thresholds:
Green LED for safe air quality (< 1000 ppm).
Yellow LED for moderate air quality (1000–2000 ppm).
Red LED for poor air quality (> 2000 ppm).
ThingSpeak Communication: Sensor data is sent to ThingSpeak every 10 seconds.

 - - - - - How to Run - - - -
Install the necessary libraries in the Arduino IDE.
Connect the hardware components as described.
Upload the code to your ESP8266 board.
Monitor the air quality through the LEDs and view detailed data on ThingSpeak.

- - - - - License - - - -
This project is open source and available under the MIT License.
