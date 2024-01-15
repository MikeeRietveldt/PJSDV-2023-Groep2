// declaratie
#pragma once
#include <time.h>

class Client;

class Muur {

public:
void readLDR(Client &client); //reads LDR value
void sendRGB(int led, int rgb, Client &client); // Function to send RGB values to the server
void readPotent(Client &client);

// void belflits(Client &client); //lichtjes knipperen 3x binnenkant wanneer er aangebeld wordt.
// void update(Client &client); //live update functie



private:
int ledOnOff;
int rgbMode;
int ldrValue;
char potentMeter;
};