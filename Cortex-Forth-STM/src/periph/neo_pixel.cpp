// SO 27 May 2019: This sketch ran on the Metro M0 Express, at the time.
// therm-TMP36-aa/tmp36-cc/neo_pixel.h

#include <Adafruit_NeoPixel.h> // REQUIRED: https://github.com/adafruit/Adafruit_NeoPixel
#include "neo_pixel.h"
#define PIN              8 // Metro M4 Express - as found wired to 8x NeoPixel strip - May 2019
#define NUMPIXELS        1 // external 8x npx strip - may 2019

extern void push(int n);
extern int pop(void);

// kelloggs
int red   = 0;
int green = 0;
int blue  = 0;

int st_index = 0; // stack index

// oldcode may 2019: Adafruit_NeoPixel pixels = Adafruit_NeoPixel(1, 40, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void pop_tuple(void) {
    int extra = pop();
    red = pop();
    green = pop();
    blue = pop();
}

void push_tuple(void) {
    push(blue);
    push(green);
    push(red); // will pop as RGB tuple
    push(0); // need 32 byte stack 4x8
}

/* void dimPixels(void) {
    pixels.setBrightness(25); } */

void glow_Dark(void) {
    pixels.setPixelColor(0, 0, 0, 0);
    delay(25);
    pixels.show();
    delay(50);
}

void glow_Blue4(void) {
    red   =  0;
    green =  0;
    blue  = 1;
}

void glow_Blue1(void) {
    red   =  0;
    green =  0;
    blue  = 5;
}

void glow_Red1(void) {
    red   =  2;
    green =  0;
    blue  =  0;
}

void _illuminate(void) {
    glow_Dark();
    pop_tuple(); // setup red green and blue values popped off the stack
    pixels.setPixelColor(st_index, red, green, blue);
    red = 0; green = 0; blue = 0;
    delay(20);
    pixels.show();
    // delay(50); // kludge debug 0122z 28 may
}

void Marche_des_Enfants_Rouges(void) {
    push_tuple(); // no clear idea where this should happen, but ..
                  // it was happening too often, prior.  The idea
                  // is that it updates the next interval-station.

    for (int i=8;i>0;i--) {
        st_index = i-1; // move this stack pointer-like entity
        _illuminate();
    }
}

void illuminate(void) { // alias
    Marche_des_Enfants_Rouges();
}

void show_master_pixel(void) {
    pixels.setPixelColor(0, red, green, blue);
    pixels.show();
}

void setup_neoPixel(void) {
    pixels.begin();
    glow_Dark();

    delay(100);

    red = 10;
    blue = 9;
    green = 0;
    show_master_pixel();
    delay(700);
    glow_Dark();
    delay(700);
    show_master_pixel();
    delay(700);
    glow_Dark();
    delay(700);
}
