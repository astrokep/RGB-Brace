#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ArduinoOTA.h>

#include "config.h"
#include "led_control.hpp"

// LED Defines:
#define LED_PIN    D4
        
#define LED_COUNT 9

#define DELAY_TIME 10

#define MIN_BRIGHTNESS 10
#define MAX_BRIGHTNESS 225

// WIFI Defines:
#define WIFI_MAX_RETRIES 20

LEDStrip strip1(LED_COUNT, MIN_BRIGHTNESS, MAX_BRIGHTNESS);

ESP8266WiFiMulti wifiMulti;

void initWIFI(){
    Serial.println('\n');
    Serial.print("Connected to ");
    Serial.println(WiFi.SSID());              // Tell us what network we're connected to
    Serial.print("IP address:\t");
    Serial.println(WiFi.localIP());           // Send the IP address of the ESP8266 to the computer
    
    ArduinoOTA.setHostname("ESP8266");
    ArduinoOTA.setPassword("esp8266");
    ArduinoOTA.onStart([]() {
        Serial.println("Start");
    });
    ArduinoOTA.onEnd([]() {
        Serial.println("\nEnd");
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    });
    ArduinoOTA.onError([](ota_error_t error) {
        Serial.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
        else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
        else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
        else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
        else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });
    ArduinoOTA.begin();
    Serial.println("OTA ready");
}


void setup(){
    
    Serial.begin(115200);
    wifiMulti.addAP(WIFI_SSID, WIFI_PASS);

    Serial.println("Connecting ...");
    for(int i = 0; i < WIFI_MAX_RETRIES; ++i){
        if(wifiMulti.run() == WL_CONNECTED){
            initWIFI();
            break;
        }
        delay(500);
        Serial.print('.');
    }
  
    // while(!randomWarmup(200, 10, 200, 10));
    // <WS2812, LED_PIN, GRB>
    // LEDStripConfig strip1Config;
    strip1.initStrip();
    strip1.setPixel(0, 0, 20, 20); 
    strip1.update();
}

bool warmup = false;
void loop(){
    if(wifiMulti.run() == WL_CONNECTED){
        ArduinoOTA.handle();
    }
    // twinkle(160, 10, 160, 20, 0, 20, 100);
    strip1.leftshift(1);
    strip1.update();
    // Serial.println(randomWarmup(200, 10, 200, 10));
    delay(100);

}