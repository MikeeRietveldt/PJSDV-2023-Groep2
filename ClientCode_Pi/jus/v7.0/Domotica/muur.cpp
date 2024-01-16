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

void Muur::sendRGB(int led, int rgb, Client &client) {
    // ldrValue = ldr;
    ledOnOff = led;
    rgbMode = rgb;
    // Data doorsturen naar de server, similar to sendstriprgb in main.cpp
    char sendBuffer[64];
    snprintf(sendBuffer, sizeof(sendBuffer), "%d,%d,%d", ldrValue, ledOnOff, rgbMode);
    client.sending(sendBuffer, strlen(sendBuffer));
}

void Muur::vraagInput(Client &client){
    Json json;
    Muur muur;
    char buffer[64];
    std::cout << "Strip=0,1 RGB 0..4" << std::endl;
    std::cout << "Enter choice followed by values note {'strip':1,'rgb':1}:" << std::endl;

    // Read JSON input from user
    std::getline(std::cin, userInput);

    //rapidjson::Document jsoninput;
    jsoninput.Parse(userInput.c_str());
    // Validate JSON structure
    if (!jsoninput.IsObject() || !jsoninput.HasMember("strip") || !jsoninput.HasMember("rgb")){
        std::cerr << "Invalid input. Please enter valid JSON with 'strip' or 'rgb'" << std::endl;
    }

    // Save the JSON input to a file
    json.saveJsonToFile(jsoninput, "input.json");

    //lees waarden strip
    int strip = jsoninput["strip"].GetInt();
    int rgbwaarde = jsoninput["rgb"].GetInt();
    // Clear buffer
    memset(buffer, 0, sizeof(buffer));
    //sendstriprgb(strip, rgbwaarde, client);
    muur.sendRGB(strip,rgbwaarde,client);
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
    // char sendBuffer[64];
    // snprintf(sendBuffer, sizeof(sendBuffer), "%d", ldrValue);
    // client.sending(sendBuffer, strlen(sendBuffer)); //1 karakter versturen.
    // std::cout << "LDR verstuurd " << ldrValue << std::endl;

}

int Muur::actPot(int pot, Client &client){
    // int potValue;
    if (pot < 1000) {
        potValue = 0; //helemaal uit
    } else if(pot < 800){
        potValue = 1;
    } else if(pot < 600){
        potValue = 2;
    } else if(pot < 400){
        potValue = 3;
    } else if(pot < 200){
        potValue = 4;
    } else if(pot > 2000){
        potValue=9;
    }
    return potValue;
}


void Muur::sendLdrPot(int lValue, int pValue, Client &client){
    char sendBuffer[64];
    snprintf(sendBuffer, sizeof(sendBuffer), "%d,%d", lValue,pValue);
    client.sending(sendBuffer, strlen(sendBuffer)); //1 karakter versturen.
    std::cout << "LDR verstuurd: " << lValue << "Pot: " << pValue << std::endl;
}
