#include <ESP8266WiFi.h>  // Use #include <WiFi.h> for ESP32
#include <Adafruit_Sensor.h>
#include <DHT.h>

#define DHTPIN D2       // GPIO 4 for DHTxx
#define DHTTYPE DHT22   // DHT 22 (AM2302), AM2321 or Type DHT11

DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

WiFiServer server(80);

float temperature = 0.0;
float humidity = 0.0;

void setup() {
  Serial.begin(115200);
  delay(10);
  dht.begin();

  // Connect to WiFi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  // Read temperature and humidity
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print("°C, Humidity: ");
  Serial.print(humidity);
  Serial.println("%");

  // Listen for incoming clients
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New Client.");
    while (client.connected()) {
      if (client.available()) {
        String request = client.readStringUntil('\r');
        client.flush();
        Serial.print("Request: ");
        Serial.println(request);
        Serial.print("Client IP: ");
        Serial.println(client.remoteIP());

        if (request.indexOf("TEMP") != -1) {
          client.print("Temperature: " + String(temperature) + "°C");
        } else if (request.indexOf("HUM") != -1) {
          client.print("Humidity: " + String(humidity) + "%");
        } else {
          client.print("Invalid command");
        }

        break;
      }
    }
    client.stop();
    Serial.println("Client Disconnected.");
  }

  delay(2000); // Read every 2 seconds
}
