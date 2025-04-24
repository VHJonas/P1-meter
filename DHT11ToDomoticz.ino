#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <DHT.h>
#include <ArduinoOTA.h>

//=== Verander hier jouw gegevens ===
const char* ssid = "ORWI-PUBLIC";
const char* password = "26102610";
const char* hostName = "DHT11_Sensor";
const char* domoticzIP = "172.16.101.85";  // Vervang door IP van Domoticz-server (ik draai op windows --> lokaal adres; ipconfig in cmd)
const int domoticzPort = 8080;
const int domoticzDHTIdx = 3;        // Pas aan naar je dummy temp+hum sensor IDX
const char* user = "esp8266";
const char* pass_user = "esp8266";
//=== Einde instellingen ===

// DHT instellingen
#define DHTPIN 2       // GPIO2 (D4 op NodeMCU)
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

unsigned long lastDHTUpdate = 0;
const unsigned long dhtInterval = 5000; // Elke 60 seconden

void setup() {
  Serial.begin(115200);
  dht.begin();

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("Verbinden met WiFi...");

  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Verbinding mislukt! Herstart...");
    delay(5000);
    ESP.restart();
  }

  // OTA configuratie (optioneel)
  ArduinoOTA.setHostname(hostName);
  ArduinoOTA.begin();

  Serial.println("Verbonden met WiFi");
  Serial.print("IP adres: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  ArduinoOTA.handle();
  
  if (millis() - lastDHTUpdate > dhtInterval) {
    updateDHT();
    lastDHTUpdate = millis();
  }
}

void updateDHT() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Fout bij uitlezen DHT11!");
    return;
  }

  // Toon op seriële monitor
  Serial.println("=== DHT11 uitlezing ===");
  Serial.print("Temperatuur: ");
  Serial.print(temperature);
  Serial.println(" °C");

  Serial.print("Vochtigheid: ");
  Serial.print(humidity);
  Serial.println(" %");

  int humStatus = 2; // standaard = normaal
  if (humidity < 30) humStatus = 1;
  else if (humidity > 70) humStatus = 3;

  char sValue[30];
  sprintf(sValue, "%.1f;%.0f;%d", temperature, humidity, humStatus);
  sendToDomoticz(domoticzDHTIdx, 0, sValue);
}


void sendToDomoticz(int idx, int nValue, char* sValue) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    char url[256];
    sprintf(url, "http://%s:%d/json.htm?type=command&param=udevice&idx=%d&nvalue=%d&svalue=%s",
            domoticzIP, domoticzPort, idx, nValue, sValue);

    Serial.printf("Versturen naar Domoticz: %s\n", url);
    http.begin(url);

    // Voeg autorisatie toe
    http.setAuthorization(user, pass_user);

    int httpCode = http.GET();

    if (httpCode > 0) {
      Serial.printf("Domoticz HTTP code: %d\n", httpCode);
    } else {
      Serial.printf("HTTP fout: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  }
}
