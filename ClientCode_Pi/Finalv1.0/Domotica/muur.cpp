#include <iostream>
#include <cstring>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "muur.h"
#include "../client.h"
#include "../json.h"
// #include "meubel.h"

extern std::string userInput;
extern rapidjson::Document jsoninput;

Client client;
Json json;

void Muur::readLDR(Client &client){
    char buffer[64];
    // char ldrValue;
    client.receive(buffer, sizeof(buffer));
    unsigned int anin0 = atoi(buffer);
    memset(buffer, 0, sizeof(buffer));
    std::cout << "ldr value: " << anin0 << std::endl;
    //usleep(500000);

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
    char buffer[64];
    client.receive(buffer, sizeof(buffer));
    unsigned int anin1 = atoi(buffer);
    memset(buffer, 0, sizeof(buffer));
    std::cout << "pot meter: " << potentMeter << std::endl;
       
}

void Muur::sendRGB(int led, int rgb,Client &client) {
    // ldrValue = ldr;
    ledOnOff = led;
    rgbMode = rgb;
    // Data doorsturen naar de server, similar to sendstriprgb in main.cpp
    char sendBuffer[64];
    snprintf(sendBuffer, sizeof(sendBuffer), "%d,%d,%d,%d",ldrValue,potValue/*ldrValue*/,ledOnOff, rgbMode);
    std::cout << "strip rgb zou verstuurd moeten zijn" << std::endl;
    std::cout << "buffer die verstsuurd is naar wemos: " << sendBuffer << std::endl;
    client.sending(sendBuffer, strlen(sendBuffer));
    memset(sendBuffer, 0, sizeof(sendBuffer));    
}



int Muur::actLDR(int ldr, Client &client){
    std::cout << "ldr value: " << ldr << std::endl;
    //usleep(500000);

    if (ldr < 180) {
        ldrValue = 1;
    } else {
        ldrValue = 0;
    }
    return ldrValue;
}

int Muur::actPot(int pot, Client &client){
    // int potValue;
    std::cout <<"ontvangen pot is:" << pot << std::endl;
    if (pot > 1000) {
        potValue = 5;
    } else if (pot > 800) {
        potValue = 4;
    } else if (pot > 600) {
        potValue = 3;
    } else if (pot > 400) {
        potValue = 2;
    } else if(pot > 200){
        potValue = 1;
    }   else {
        potValue = 0; // Default if none of the above conditions are met
    }

    return potValue;
}


void Muur::sendLdrPot(int lValue, int pValue, Client &client){
    char sendBuffer[64];
    snprintf(sendBuffer, sizeof(sendBuffer), "%d,%d", lValue,pValue);
    client.sending(sendBuffer, strlen(sendBuffer)); //1 karakter versturen.
    std::cout << "LDR verstuurd: " << lValue << "Pot: " << pValue << std::endl;
}


void Muur::begin(){
    client.connecting("192.168.137.248");   
    client.receive(Mbuffer, sizeof(Mbuffer));
    std::cout << "MBuffer is: " << Mbuffer << std::endl;
    json.saveReceivedDataToJson("output.json", Mbuffer);
    json.jsonLdrPot("output.json", client);
    client.disconnect();
    memset(Mbuffer, 0, sizeof(Mbuffer));
}