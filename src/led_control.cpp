#include "led_control.hpp"

// Constructor:
LEDStrip::LEDStrip(int LEDCount, int minBrightness, int maxBrightness){
    // _LEDPin = LEDPin;
    _LEDCount = LEDCount;
    _minBrightness = minBrightness;
    _maxBrightness = maxBrightness;

    _leds = (CRGB*)malloc(_LEDCount * sizeof(CRGB));
}

int LEDStrip::initStrip(){
    FastLED.addLeds<WS2812B, 2, GRB>(_leds, _LEDCount).setCorrection(TypicalLEDStrip);
    this->setStrip(0, 0, 0);
    this->update();
    return 1;
}

// Utility Functions:
void LEDStrip::setPixel(int pixel, byte red, byte green, byte blue){
    _leds[pixel].r = red;
    _leds[pixel].g = green;
    _leds[pixel].b = blue;
}

void LEDStrip::setStrip(byte red, byte green, byte blue){
    for(int i = 0; i < _LEDCount; ++i){
        setPixel(i, red, green, blue);
    }
}

void LEDStrip::update(){
    FastLED.show();
}

// Strip Operations:
void LEDStrip::leftshift(bool wrap){
    CRGB temp = _leds[_LEDCount - 1];
    for(int i = _LEDCount - 1; i > 0; --i){
        _leds[i] = _leds[i-1];
    }
    if(!wrap){
        temp.r = 0;
        temp.g = 0;
        temp.b = 0;
    }
    _leds[0] = temp;    
}

void LEDStrip::rightshift(bool wrap){
    CRGB temp = _leds[0];
    for(int i = 0; i < _LEDCount; ++i){
        _leds[i] = _leds[i+1];
    }
    if(!wrap){
        temp.r = 0;
        temp.g = 0;
        temp.b = 0;
    }
    _leds[_LEDCount - 1] = temp;    
}

// Animations:
bool LEDStrip::randomWarmup(byte r, byte g, byte b, byte dl){
  bool done = true;
  for(int i = 0; i < _LEDCount; ++i){
    if(_leds[i].r != r || _leds[i].g != g || _leds[i].b != b){
      done = false;
      byte incr = random(0, dl);
      byte r_out = constrain(_leds[i].r + incr, _minBrightness, _maxBrightness);
      byte g_out = constrain(_leds[i].g + incr, _minBrightness, _maxBrightness);
      byte b_out = constrain(_leds[i].b + incr, _minBrightness, _maxBrightness);
      this->setPixel(i, r_out, g_out, b_out);
    }
  }
  this->update();
  return done;
}

void LEDStrip::twinkle(byte r, byte g, byte b, float dr, float dg, float db, float dl){
    for(int i = 0; i < _LEDCount; ++i){
        float dl_out = random(100-dl, 100)/100.0;
        float dr_coef = random(-dr, dr)/100.0;
        float dg_coef = random(-dg, dg)/100.0;
        float db_coef = random(-db, db)/100.0;
        byte r_out = constrain(r * (1 + dr_coef) * dl_out, _minBrightness, _maxBrightness);
        byte g_out = constrain(g * (1 + dg_coef) * dl_out, _minBrightness, _maxBrightness);
        byte b_out = constrain(b * (1 + db_coef) * dl_out, _minBrightness, _maxBrightness);
        this->setPixel(i, r_out, g_out, b_out);

    }
}