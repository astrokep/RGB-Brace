#ifndef LED_CONTROL_H
#define LED_CONTROL_H

#include <FastLED.h>

class LEDStrip {
    public:
        LEDStrip(int LEDCount, int minBrightness, int _maxBrightness);
        
        // template <ESPIChipsets CHIPSET,  uint8_t DATA_PIN, EOrder RGB_ORDER>
        int initStrip();

        // Utility Functions:
        void setPixel(int pixel, byte red, byte green, byte blue);
        void setStrip(byte red, byte green, byte blue);
        void update();

        // Strip Operations:
        void leftshift(bool wrap);
        void rightshift(bool wrap);

        // Animations:
        bool randomWarmup(byte r, byte g, byte b, byte dl);
        void twinkle(byte r, byte g, byte b, float dr, float dg, float db, float dl);

    private:
        uint8_t _LEDPin;
        int _LEDCount;

        int _minBrightness;
        int _maxBrightness;
        
        CRGB* _leds;
};

#endif