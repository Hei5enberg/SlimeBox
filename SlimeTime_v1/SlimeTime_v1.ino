#include <WiFi.h>
#include "time.h"

#define controlPin 4

const char* ssid     = "*****";
const char* password = "*****";
const char* ntpServer = "pool.ntp.org";

unsigned long epochTime; 

const int lightWarmupTime = 3000;
const int timeLightOnSeconds = 3; 

const int frameFrequencySeconds = 60 * 5; 

bool firstRun = true;
unsigned long nextRun = 0;

int frameCount = 0;

void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.print('.');
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
  }
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);
  pinMode(controlPin, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  initWiFi();
  configTime(0, 0, ntpServer);
}

int ledBlinkLength = 500;

void loop() {
  epochTime = getTime();

  if (firstRun) {
    StartupSequence();
    firstRun = false;
    nextRun = epochTime + 10;
  }
  
  if (epochTime >= nextRun) {
    Serial.println("Turning light on...");
    digitalWrite(controlPin, HIGH);
    delay(lightWarmupTime);

    Serial.println();
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("Camera should shoot frame");
    delay(ledBlinkLength);
    Serial.print("Frame: ");
    Serial.println(frameCount);
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println();

    delay((timeLightOnSeconds * 1000) - ledBlinkLength);
    digitalWrite(controlPin, LOW);
 
    nextRun = epochTime + frameFrequencySeconds;
    Serial.print("Next run: ");
    Serial.println(nextRun);
  }
  
  delay (500);
}

void StartupSequence () {
  Serial.println("Running startup procedure...");
  Serial.println("Starting countdown:");
  for (int i = 0; i < 3; i++) {
    Serial.println(3 - i);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000); 
  }
}

unsigned long getTime() {
  time_t now;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    for (int i = 0; i < 25; i++) {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(100);
      digitalWrite(LED_BUILTIN, LOW);
      delay(100);
    }
    return(0);
  }
  time(&now);
  return now;
}
