#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include "Adafruit_NeoPixel.h"
#include "esp_task_wdt.h"

#define LED_PIN 12
#define LED_COUNT 10
#define LED_BRIGHTNESS 50

Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// Replace the next variables with your SSID/Password combination
const char* ssid = "EcoleDuWeb2.4g";
const char* password = "EcoleDuWEB";

// Add your MQTT Broker IP address, example:
//const char* mqtt_server = "192.168.1.144";
const char* mqtt_server = "172.16.5.202";

byte messageRecu[50] = "";

const char* TOPIC_LED = "LaPorte/LEDs";
const char* MSG_COLOR_CYCLE_1 = "Bouton Cycle Couleur 1 - Bleu_Blanc_Rouge";
const char* MSG_COLOR_CYCLE_2 = "Bouton Cycle Couleur 2 - Vert_Blanc_Rouge";
const char* MSG_COLOR_CYCLE_3 = "Bouton Cycle Couleur 3 - Rouge_Bleu_Violet";
const char* MSG_COLOR_CYCLE_4 = "Bouton Cycle Couleur 4 - Violet_Bleu_Vert_Jaune";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

// LED Pin
const int PIN_TO_SENSOR = 19; // GIOP19 pin connected to OUTPUT pin of sensor
int pinStateCurrent   = LOW;  // current state of pin
int pinStatePrevious  = LOW;  // previous state of pin
const int ledPin = 4;

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");

  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
  }
  
  memcpy(messageRecu, message, length);
  messageRecu[length] = '\0'; // ajouter le caractère de fin de chaîne
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP32Client")) {
      Serial.println("connected");
      // Subscribe to the topic
      client.subscribe("LaPorte/LEDs");
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
Serial.begin(9600);

setup_wifi();
client.setServer(mqtt_server, 1883);
client.setCallback(callback);

strip.begin();
strip.setBrightness(LED_BRIGHTNESS);
strip.show();

// Activer la surveillance de l'horloge de la tâche
esp_task_wdt_init(1, true);
}

void loop() {
  if (!client.connected()) 
  {
    reconnect();
  }
  client.loop();

  // Pour éviter les erreurs avec NeoPixel
  yield();

  if (strcmp((const char*)messageRecu, MSG_COLOR_CYCLE_1) == 0)
  {
    for (int i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(0, 0, 255)); //Couleur bleue
      strip.show();
      delay(100);
    }
    for (int i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(255, 255, 255)); //Couleur blanche
      strip.show();
      delay(100);
    }
    for (int i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(255, 0, 0)); //Couleur rouge
      strip.show();
      delay(100);
    }
  }
  else if (strcmp((const char*)messageRecu, MSG_COLOR_CYCLE_2) == 0)
  {
      for (int i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, strip.Color(0, 255, 0)); //Couleur verte
        strip.show();
        delay(100);
      }
      for (int i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, strip.Color(255, 255, 255)); //Couleur blanche
        strip.show();
        delay(100);
      }
      for (int i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, strip.Color(255, 0, 0)); //Couleur rouge
        strip.show();
        delay(100);
      }
    }
  else if (strcmp((const char*)messageRecu, MSG_COLOR_CYCLE_3) == 0)
  {
      for (int i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, strip.Color(255, 0, 0)); //Couleur rouge
        strip.show();
        delay(100);
      }
      for (int i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, strip.Color(0, 0, 255)); //Couleur bleue
        strip.show();
        delay(100);
      }
      for (int i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, strip.Color(255, 0, 255)); //Couleur violette
        strip.show();
        delay(100);
      }
    }
  else if (strcmp((const char*)messageRecu, MSG_COLOR_CYCLE_4) == 0)
  {
      for (int i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, strip.Color(255, 0, 255)); //Couleur violette
        strip.show();
        delay(100);
      }

      for (int i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, strip.Color(0, 0, 255)); //Couleur bleue
        strip.show();
        delay(100);
      }

      for (int i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, strip.Color(0, 255, 0)); //Couleur verte
        strip.show();
        delay(100);
      }

      for (int i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, strip.Color(255, 255, 0)); //Couleur jaune
        strip.show();
        delay(100);
      }
    }
  else
  {
    for (int i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(255, 0, 0)); //Couleur rouge
      strip.show();
      delay(100);
    }
  }
}