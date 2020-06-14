#include "universal-mqtt.h"
#include <Wire.h>
#include "hardware/i2c.h"

void messageReceived(String &topic, String &payload)
{
  Serial.println("incoming: " + topic + " - " + payload);
}

#define PUBLISH_DELAY 300

HardwareI2C hardwareI2C;

const char *mqttSubfolder = "/publish/sensors/test/rows";

void setup()
{
  Serial.begin(115200);
  delay(3000);

  hardwareI2C.startI2C();
  Serial.println("i2c_init_done");

  setupCloudIoT();
}

unsigned long lastMillis = 0;
unsigned long loopStime = 0;
unsigned long sinceLastPublishMs = 0;
unsigned long timestampMs = 0;

void loop()
{
  loopStime = millis();

  mqttClient->loop();
  delay(5); // <- fixes some issues with WiFi stability

  if (!mqttClient->connected())
  {
    connect();
  }

  timestampMs = millis();
  sinceLastPublishMs = timestampMs - lastMillis;
  if (sinceLastPublishMs > PUBLISH_DELAY)
  {
    lastMillis = timestampMs;

    char *data = "{ \"hello\":\"world\" }";
    publishTelemetry(mqttSubfolder, data);

    Serial.println("Publish done");

    delay(4);
  }
}