#include <HCSR04.h>

HCSR04 hc(PD2, PD3); //initialisation class HCSR04 (trig pin , echo pin)

void setup()
{ Serial.begin(9600); }

void loop()
{
    Serial.println(hc.dist()); //return current distance (cm) in serial
    delay(600);                   // we suggest to use over 60ms measurement cycle, in order to prevent trigger signal to the echo signal.
}