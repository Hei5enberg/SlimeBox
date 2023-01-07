#include <WiFi.h>
#include "time.h"

#define controlPin 15

const char* ssid     = "****";
const char* password = "****";

const char* ntpServer = "pool.ntp.org";
unsigned long epochTime; 

const int lightWarmupTime = 2500;

const int frameFrequencySeconds = 60; 
//const int recordingTimeHours = 36; // Hours * 3600 to convert to seconds
//const long int framesToBeShot = (recordingTimeHours * 3600L) / frameFrequencySeconds;
const int timeLightOnSeconds = 4; 

unsigned long nextLight = 0;

bool startupProcedure = true;

void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);
  pinMode(controlPin, OUTPUT);

  initWiFi();
  configTime(0, 0, ntpServer);
}

void loop() {
  epochTime = getTime();
  Serial.print("Epoch Time: ");
  Serial.println(epochTime);
  delay(1000);
}

void StartupSequence () {
  Serial.println("Running startup procedure...");
  Serial.println("Starting countdown:");
  for (int i = 0; i < 3; i++) {
    Serial.println(3 - i);
    if (i == 4) {
      digitalWrite(controlPin, HIGH);
      delay(5000);
      digitalWrite(controlPin, LOW);
      delay(250);
    } else {
      digitalWrite(controlPin, HIGH);
      delay(3000);
      digitalWrite(controlPin, LOW);
      delay(3000);
    }
  }
}

unsigned long getTime() {
  time_t now;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    //Serial.println("Failed to obtain time");
    return(0);
  }
  time(&now);
  return now;
}
