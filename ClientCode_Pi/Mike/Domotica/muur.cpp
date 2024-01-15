#include <iostream>
#include <cstring>

#include "muur.h"
#include "../client.h"

void Muur::readLDR(Client &client){
    char buffer[64];
    // char ldrValue;
    client.receive(buffer, sizeof(buffer));
    unsigned int anin0 = atoi(buffer);
    std::cout << "ldr value: " << anin0 << std::endl;

    if (anin0 < 180) {
        ldrValue = 1;
    } else {
        ldrValue = 0;
    }
    char sendBuffer[64];
    snprintf(sendBuffer, sizeof(sendBuffer), "%d", ldrValue);
    client.sending(sendBuffer, strlen(sendBuffer)); //1 karakter versturen.
    std::cout << "LDR verstuurd " << ldrValue << std::endl;

} 

void Muur::readPotent(Client &client){
    char buffer [64];
    client.receive(buffer, sizeof(buffer));
    unsigned int anin1 = atoi(buffer);
    std::cout << "pot meter: " << potentMeter << std::endl;
       
}

void Muur::sendRGB(int led, int rgb, Client &client) {
    ledOnOff = led;
    rgbMode = rgb;
    // Data doorsturen naar de server, similar to sendstriprgb in main.cpp
    char sendBuffer[64];
    snprintf(sendBuffer, sizeof(sendBuffer), "%d,%d,%d", ldrValue, ledOnOff, rgbMode);
    client.sending(sendBuffer, strlen(sendBuffer));
}

void Muur::update(Client &client){



}

Muur::Muur(const char* iptje): Meubel(iptje){


}