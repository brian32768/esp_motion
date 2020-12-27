/*
  written for a WeMOS D1 ESP8266 Arduino board

  Connects to WiFi
  Publishes a "connected" message to let MQTT know it's online.
  Watches for a signal from a radar motion sensor.
  Publishes a "motion" message each time it sees motion.

  It will reconnect to the server if the connection is lost using a blocking
  reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
  achieve the same result without blocking the main loop.
*/
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "wifi.h"

const char* PUB_DISCOVERY = "HOME/device_automation/1/WEMOS01/config";
const char* DISCOVERY_JSON = "{\"automation_type\":\"trigger\",\"topic\":\"WEMOS\",\"payload\":\"1\",\"uniq_id\":\"WEMOS01\"";

const char* PUB_MOTION    = "WEMOS/Motion";
const char* PUB_TICK      = "WEMOS/Tick";

const char* SUB_TOPIC = "HOME";
const char* MYNAME = "WEMOS01";

const int TICKTIME = 15000; // send message every TICKTIME milliseconds
const int RADAR = D8;

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

int state = 0; // no motion = 0, motion = 1

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Received [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      
      // Once connected, publish an announcement...
      client.publish(PUB_DISCOVERY, DISCOVERY_JSON);
      
      // ... and resubscribe
      //client.subscribe(SUB_TOPIC);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  pinMode(RADAR, INPUT);

  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long now = millis();
  if (now - lastMsg > TICKTIME) {
    lastMsg = now;
    ++value;
    snprintf (msg, MSG_BUFFER_SIZE, MYNAME);
    client.publish(PUB_TICK, msg);
    client.publish(PUB_DISCOVERY, DISCOVERY_JSON);
  }

  int radar = digitalRead(RADAR);
  if (radar == 1) {
    if (!state) {
      digitalWrite(BUILTIN_LED, 0); // Turn on LED
      client.publish(PUB_MOTION, "1");
      lastMsg = now; // Reset timer to 0
      state = 1;
    }
  } else {
    if (state) {
      digitalWrite(BUILTIN_LED, 1); // Turn off LED
      client.publish(PUB_MOTION, "0");
      state = 0;
    }
  }
}
