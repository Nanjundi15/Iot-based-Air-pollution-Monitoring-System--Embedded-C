#include <DHT.h>
#include <ESP8266WiFi.h>
#include <ThingSpeak.h>

#define MQ135_PIN A0
#define RLOAD 10.0        // Load resistance on the board, in kilo ohms
#define RZERO 76.63       // Calibration resistance at atmospheric CO2 level

// Gas constants for CO2, CO, NH3, and Alcohol
#define CO2_PARA 116.6020682 
#define CO2_PARB 2.769034857
#define CO_PARA  107.22     // Example constant for CO, adjust as necessary
#define CO_PARB  2.57       // Example constant for CO, adjust as necessary
#define NH3_PARA 124.85     // Example constant for NH3, adjust as necessary
#define NH3_PARB 2.69       // Example constant for NH3, adjust as necessary
#define ALCOHOL_PARA 50.0   // Example constant for Alcohol, adjust as necessary
#define ALCOHOL_PARB 2.3    // Example constant for Alcohol, adjust as necessary

// Define LED pins
#define RED_LED_PIN D1
#define GREEN_LED_PIN D2
#define YELLOW_LED_PIN D5 // Define the pin for the yellow LED

// CO2 concentration thresholds in ppm
#define CO2_LOW_THRESHOLD 1000.0
#define CO2_MID_THRESHOLD 2000.0

// Define DHT11 pin and type
#define DHTPIN D3
#define DHTTYPE DHT11

// WiFi credentials
const char* ssid = "Nanjundi";
const char* password = "12345678";

// ThingSpeak channel information
const char* apiKey = "ZR7YBB1U07D7LK4Q";
const unsigned long channelID = 2618701; // Replace with your ThingSpeak channel ID

DHT dht(DHTPIN, DHTTYPE);
WiFiClient client; // Create a WiFiClient object

void setup() {
  Serial.begin(115200);
  
  // Initialize LED pins as outputs
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(YELLOW_LED_PIN, OUTPUT);
  
  // Initialize LEDs to be off
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(YELLOW_LED_PIN, LOW);
  
  pinMode(MQ135_PIN, INPUT);
  
  // Initialize the DHT11 sensor
  dht.begin();

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");

  // Initialize ThingSpeak
  ThingSpeak.begin(client);
}

void loop() {
  int sensorValue = analogRead(MQ135_PIN);
  float voltage = sensorValue * (3.3 / 1023.0);

  float resistance = ((3.3 * RLOAD) / voltage) - RLOAD; // Calculate sensor resistance
  float ratio = resistance / RZERO;                     // Ratio of sensor resistance to RZERO

  // Estimate concentrations in ppm using sensor's equations
  float co2_ppm = CO2_PARA * pow(ratio, -CO2_PARB);
  float co_ppm = CO_PARA * pow(ratio, -CO_PARB);
  float nh3_ppm = NH3_PARA * pow(ratio, -NH3_PARB);
  float alcohol_ppm = ALCOHOL_PARA * pow(ratio, -ALCOHOL_PARB);

  // Read temperature and humidity from DHT11
  float temperature = dht.readTemperature(); // Read temperature as Celsius
  float humidity = dht.readHumidity();       // Read humidity as percentage

  // Check if reading failed
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Print sensor data to Serial Monitor
  Serial.print("Voltage: ");
  Serial.print(voltage);
  Serial.print(" V, CO2 Concentration: ");
  Serial.print(co2_ppm);
  Serial.print(" ppm, CO Concentration: ");
  Serial.print(co_ppm);
  Serial.print(" ppm, NH3 Concentration: ");
  Serial.print(nh3_ppm);
  Serial.print(" ppm, Alcohol Concentration: ");
  Serial.print(alcohol_ppm);
  Serial.print(" ppm, Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C, Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  // Control LEDs based on CO2 concentration
  if (co2_ppm < CO2_LOW_THRESHOLD) {
    // Low CO2 concentration
    digitalWrite(RED_LED_PIN, LOW);
    digitalWrite(GREEN_LED_PIN, HIGH);
    digitalWrite(YELLOW_LED_PIN, LOW);
  } else if (co2_ppm >= CO2_LOW_THRESHOLD && co2_ppm <= CO2_MID_THRESHOLD) {
    // Medium CO2 concentration
    digitalWrite(RED_LED_PIN, LOW);
    digitalWrite(GREEN_LED_PIN, LOW);
    digitalWrite(YELLOW_LED_PIN, HIGH);
  } else {
    // High CO2 concentration
    digitalWrite(RED_LED_PIN, HIGH);
    digitalWrite(GREEN_LED_PIN, LOW);
    digitalWrite(YELLOW_LED_PIN, LOW);
  }

  // Send data to ThingSpeak
  if (WiFi.status() == WL_CONNECTED) {
    ThingSpeak.setField(1, co2_ppm);
    ThingSpeak.setField(2, co_ppm);
    ThingSpeak.setField(3, nh3_ppm);
    ThingSpeak.setField(4, alcohol_ppm);
    ThingSpeak.setField(5, temperature);
    ThingSpeak.setField(6, humidity);
    
    // Update ThingSpeak
    int httpCode = ThingSpeak.writeFields(channelID, apiKey);
    if (httpCode == 200) {
      Serial.println("Data sent to ThingSpeak successfully.");
    } else {
      Serial.println("........");
    }
  } else {
    Serial.println("WiFi not connected");
  }

  delay(10000); // Adding a delay between readings and data sending
}
