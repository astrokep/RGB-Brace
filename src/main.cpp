#include <Arduino.h>
#include <FastLED.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ArduinoOTA.h>

#include "config.h"

#define LED_PIN    D4
        
// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 9

#define DELAY_TIME 10

#define MIN_BRIGHTNESS 10
#define MAX_BRIGHTNESS 225

CRGB leds[LED_COUNT];

ESP8266WiFiMulti wifiMulti;

void setPixel(int pixel, byte red, byte green, byte blue){
    leds[pixel].r = red;
    leds[pixel].g = green;
    leds[pixel].b = blue;
}

void setStrip(byte red, byte green, byte blue){
    for(int i = 0; i < LED_COUNT; ++i){
        setPixel(i, red, green, blue);
    }
}

void update(){
    FastLED.show();
}


//LED DISPLAY FUNCTIONS:
void twinkle(byte r, byte g, byte b, float dr, float dg, float db, float dl){
    for(int i = 0; i < LED_COUNT; ++i){
        float dl_out = random(100-dl, 100)/100.0;
        float dr_coef = random(-dr, dr)/100.0;
        float dg_coef = random(-dg, dg)/100.0;
        float db_coef = random(-db, db)/100.0;
        byte r_out = constrain(r * (1 + dr_coef) * dl_out, MIN_BRIGHTNESS, MAX_BRIGHTNESS);
        byte g_out = constrain(g * (1 + dg_coef) * dl_out, MIN_BRIGHTNESS, MAX_BRIGHTNESS);
        byte b_out = constrain(b * (1 + db_coef) * dl_out, MIN_BRIGHTNESS, MAX_BRIGHTNESS);
        setPixel(i, r_out, g_out, b_out);

    }
}

void leftshift(bool wrap){
    CRGB temp = leds[LED_COUNT - 1];
    for(int i = LED_COUNT - 1; i > 0; --i){
        leds[i] = leds[i-1];
    }
    if(!wrap){
        temp.r = 0;
        temp.g = 0;
        temp.b = 0;
    }
    leds[0] = temp;    
}

void rightshift(bool wrap){
    CRGB temp = leds[0];
    for(int i = 0; i < LED_COUNT; ++i){
        leds[i] = leds[i+1];
    }
    if(!wrap){
        temp.r = 0;
        temp.g = 0;
        temp.b = 0;
    }
    leds[LED_COUNT - 1] = temp;    
}

bool randomWarmup(byte r, byte g, byte b, byte dl){
  bool done = true;
  for(int i = 0; i < LED_COUNT; ++i){
    if(leds[i].r != r || leds[i].g != g|| leds[i].b != b){
      done = false;
      byte incr = random(0, dl);
      byte r_out = constrain(leds[i].r + incr, MIN_BRIGHTNESS, MAX_BRIGHTNESS);
      byte g_out = constrain(leds[i].g + incr, MIN_BRIGHTNESS, MAX_BRIGHTNESS);
      byte b_out = constrain(leds[i].b + incr, MIN_BRIGHTNESS, MAX_BRIGHTNESS);
      setPixel(i, r_out, g_out, b_out);
    }
  }
  update();
  return done;
}


void setup(){
    FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, LED_COUNT).setCorrection(TypicalLEDStrip);
    setStrip(0, 0, 0);
    update();

    Serial.begin(115200);
    wifiMulti.addAP(WIFI_SSID, WIFI_PASS);

    Serial.println("Connecting ...");
    int i = 0;
    while (wifiMulti.run() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
        delay(250);
        Serial.print('.');
    }
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

    // while(!randomWarmup(200, 10, 200, 10));

    setPixel(0, 0, 20, 20); 
    // setStrip(160, 10, 160);
    update();
}

bool warmup = false;
void loop(){
    ArduinoOTA.handle();
    // twinkle(160, 10, 160, 20, 0, 20, 100);
    leftshift(1);
    update();
    // Serial.println(randomWarmup(200, 10, 200, 10));
    delay(100);

}