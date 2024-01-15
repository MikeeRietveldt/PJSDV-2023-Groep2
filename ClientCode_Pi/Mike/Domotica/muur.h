// declaratie
#pragma once
#include <time.h>
#include <meubel.h>

class Client;

class Muur : public Meubel {

public:
void readLDR(Client &client); //reads LDR value
void sendRGB(int led, int rgb, Client &client); // Function to send RGB values to the server
void readPotent(Client &client);
void update(Client &client) override;
Muur(const char* iptje);

private:
int ledOnOff;
int rgbMode;
int ldrValue;
char potentMeter;
};