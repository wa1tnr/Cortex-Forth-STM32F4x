#include <Arduino.h>

extern void push(int n);
extern int pop(void);

#define LEDSPEED 200

void wiggleLEDOnce(void) {
    digitalWrite(13, HIGH);
    delay(LEDSPEED);
    digitalWrite(13, LOW);
    delay(LEDSPEED);
}

// ( u1 u2 -- )
void _PINWRITE(void)  {
    digitalWrite(pop(), pop());
    // digitalWrite(13, HIGH);
}

// ( u1 u2 -- )
void _PINMODE(void) {
    pinMode(pop(), pop());
    // pinMode(13, OUTPUT);
}

void _PINREAD(void) {
    push(digitalRead(pop()));
}

/*
void _analogRead(void) {
    // dStack_push(analogRead(dStack_pop()));
}

void _analogWrite(void) {
    // analogWrite(dStack_pop(), dStack_pop());
}
*/

