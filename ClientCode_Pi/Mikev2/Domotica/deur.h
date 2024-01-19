// declaratie
#pragma once
#include <time.h>
#include <meubel.h>

class Client;

class Deur : public Meubel {

public:
void openen(Client &client); //deur openen
void sluiten(Client &client); //deur sluiten
void belflits(Client &client); //lichtjes knipperen 3x binnenkant wanneer er aangebeld wordt.
void update(Client &client) override; //live update functie. Parent class overrides subclass
Deur(const char* iptje); 


private:
bool deurIsOpen;
clock_t laatstGeopend;
bool flitsLamp;
clock_t laatsteFlits;
int aantalFlitsen;

};