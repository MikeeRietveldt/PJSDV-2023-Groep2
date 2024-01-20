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
Deur(const char* iptje); //Deur met IP meegeven


private:
bool deurIsOpen; //Om te checken of de deur open is voor de timer
clock_t laatstGeopend; //Om een timestamp voor de timer te maken
bool flitsLamp; //Om de flits te checken
clock_t laatsteFlits; //Om een timestamp voor de timer te maken
int aantalFlitsen; //Om het aantal flitsen te checken

};