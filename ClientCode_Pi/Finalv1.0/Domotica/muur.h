// declaratie
#pragma once
#include <time.h>

class Client;

class Muur {

public:
void readLDR(Client &client); //reads LDR value
void sendRGB(int led, int rgb, Client &client); // Function to send RGB values to the server
void readPotent(Client &client);
int actLDR(int ldr, Client &client);
int actPot(int pot, Client &client);
void sendLdrPot(int lValue, int pValue, Client &client);
int test;
int test2;
void begin();
char Mbuffer [64];


private:
int ledOnOff;
int rgbMode;
int ldrValue;
char potentMeter;
int potValue;
};