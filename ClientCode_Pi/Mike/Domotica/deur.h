// declaratie
#pragma once
#include <time.h>

class Client;

class Deur {

public:
void openen(Client &client); //deur openen
void sluiten(Client &client); //deur sluiten
void belflits(Client &client); //lichtjes knipperen 3x binnenkant wanneer er aangebeld wordt.
void update(Client &client); //live update functie

private:
bool deurIsOpen;
clock_t laatstGeopend;
bool flitsLamp;
clock_t laatsteFlits;
int aantalFlitsen;

};