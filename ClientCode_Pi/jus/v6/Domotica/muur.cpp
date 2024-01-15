#include <iostream>
#include <cstring>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "muur.h"
#include "../client.h"
#include "../json.h"

extern std::string userInput;
extern rapidjson::Document jsoninput;



void Muur::readLDR(Client &client){
    char buffer[64];
    // char ldrValue;
    client.receive(buffer, sizeof(buffer));
    unsigned int anin0 = atoi(buffer);
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