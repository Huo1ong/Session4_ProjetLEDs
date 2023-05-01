/* Copyright (C) 2023 Guillou Quentin
 * All rights reserved.
 *
 * Projet LEDs
 * Ecole du Web
 * Cours : Projets Technologiques (c)2023
 *  
    @file     main.cpp
    @author   Guillou Quentin
    @version  1.3 30/04/23 

    Historique des versions
           Version    Date       Auteur       Description
           1.1        21/08/23   Quentin      Première version du logiciel
           1.2        21/08/23   Quentin      Seconde version du logiciel
           1.3        21/08/23   Quentin      Troisième version du logiciel

    platform = espressif32
    board = esp32doit-devkit-v1
    framework = arduino
    lib_deps = 
	        adafruit/Adafruit NeoPixel@^1.11.0  (Pour la gestion des LED)
	        knolleary/PubSubClient@^2.8         (Pour la communication MQTT)

    Autres librairies (à copier dans le répertoire lib)
         WifiManagerDevelopment
            //Remarques
            //Pour trouver le WifiManager (dans la branche development)
            //   https://github.com/tzapu/WiFiManager/tree/development
            //   Ne pas oublier d'appuyez sur l'ampoule et choisir : ajouter Lib
    
    Fonctions utiles (utilitaires)

    Classes du système
 * */

#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include "Adafruit_NeoPixel.h"
#include "esp_task_wdt.h"

#define LED_PIN 12
#define LED_COUNT 10
#define LED_BRIGHTNESS 50

Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// Remplacez les variables suivantes par votre combinaison SSID/Mot de passe
const char* ssid = "EcoleDuWeb2.4g";
const char* password = "EcoleDuWEB";

// Ajoutez votre adresse IP MQTT Broker, exemple :
//const char* mqtt_server = "192.168.1.144";
const char* mqtt_server = "172.16.5.202";

//Initialisation du tableau de messages reçus
byte messageRecu[50] = "";

//Topic pour les commandes de LED
const char* TOPIC_LED = "LaPorte/LEDs";

//Messages pour les cycles de couleurs
const char* MSG_COLOR_CYCLE_1 = "Bouton Cycle Couleur 1 - Bleu_Blanc_Rouge"; 
const char* MSG_COLOR_CYCLE_2 = "Bouton Cycle Couleur 2 - Vert_Blanc_Rouge";
const char* MSG_COLOR_CYCLE_3 = "Bouton Cycle Couleur 3 - Rouge_Bleu_Violet";
const char* MSG_COLOR_CYCLE_4 = "Bouton Cycle Couleur 4 - Violet_Bleu_Vert_Jaune";

//Initialisation du client WiFi et Mqtt
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

// LED Pin
const int PIN_TO_SENSOR = 19; // GIOP19 pin connecté to OUTPUT pin du capteur
int pinStateCurrent   = LOW;  // état actuel de la pin
int pinStatePrevious  = LOW;  // état passé de la pin
const int ledPin = 4;

void setup_wifi() {
  delay(10);
  // On commence par se connecter à un réseau WiFi
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
  Serial.println(WiFi.localIP()); //Affichage de l'adresse IP du module
}

// Fonction de rappel appelée lorsqu'un message est reçu
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

// Fonction pour se reconnecter au broker MQTT en cas de perte de connexion
void reconnect() {
// Boucle jusqu'à ce que nous soyons reconnectés
  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");
    // Tentative de connexion
    if (client.connect("ESP32Client")) {
      Serial.println("connected");
      // Abonnez-vous au sujet
      client.subscribe("LaPorte/LEDs");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Attendez 5 secondes avant de réessayer
      delay(5000);
    }
  }
}

// Initialise la communication série, configure le WiFi, configure le serveur MQTT et initialise la bande de LED
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

// Vérification si le client MQTT est connecté, 
  // Exécution d'une séquence de couleurs sur la bande de LED en fonction du message MQTT reçu
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