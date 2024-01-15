#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
//#include <wiringPi.h>
#include <arpa/inet.h>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "client.h"
#include "Domotica/deur.h"
#include "Domotica/muur.h"
#include "main.h"
#include "json.h"

using namespace std;

std::string userInput;
rapidjson::Document jsoninput;



void sendstriprgb(int led, int rgb, Client &client) {
    // Data doorsturen naar de server
    char sendBuffer[64];
    snprintf(sendBuffer, sizeof(sendBuffer), "%d,%d,%d", ldr, led, rgb);
    client.sending(sendBuffer, strlen(sendBuffer));
}

// void set_deur(std::string commando, Deur &deur, Client &client){
//      // Open de deur
//     char sendBuffer[64];
//     if(commando == "openDeur"){
//         deur.openen(client);
//     }
//     // sluit de deur
//     else if(commando == "sluitDeur"){
//         client.sending(&dicht, sizeof(dicht));
//     }

// }
std::string waarde_deur;

int main() {
    Deur deur; // object aanmaken
    Client client; // object aanmaken
    Muur muur;
    Json json;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        return -1;
    }
    int round = 0;
    while (true) {
        if(round == 10){ // check if '.' is pressed
            //std::cin.get(); //consume the '.' character
            json.inputJson();
            round = 0;
        } else {
        // Read out json file
        //rapidjson::Document fileJson = json.readJsonFromFile("input.json");
        //client connect to id=1 without input
            client.connecting("192.168.137.248");
            muur.readLDR(client);
            client.disconnect();
            //client connect to id=2 without input
            client.connecting("192.168.137.251");
            client.receive(buffer, sizeof(buffer));
            std::cout << "Server message to bewaker: " << buffer << "\n";
            deur.set_deur(waarde_deur, deur, client);
            client.disconnect();
            round++;
        

        // if (json.id == 1) {
        //     client.connecting("192.168.137.248");
        //     muur.readLDR(client);
        //     muur.vraagInput(client);
        //     // std::cout << "Strip=0,1 RGB 0..4" << std::endl;
        //     // std::cout << "Enter choice followed by values note {'strip':1,'rgb':1}:" << std::endl;

        //     // // Read JSON input from user
        //     // std::getline(std::cin, userInput);

        //     // //rapidjson::Document jsoninput;
        //     // jsoninput.Parse(userInput.c_str());
        //     // // Validate JSON structure
        //     // if (!jsoninput.IsObject() || !jsoninput.HasMember("strip") || !jsoninput.HasMember("rgb")){
        //     //     std::cerr << "Invalid input. Please enter valid JSON with 'strip' or 'rgb'" << std::endl;
        //     //     continue;  // Go back to the beginning of the loop
        //     // }

        //     // // Save the JSON input to a file
        //     // json.saveJsonToFile(jsoninput, "input.json");

        //     // //lees waarden strip
        //     // int strip = jsoninput["strip"].GetInt();
        //     // int rgbwaarde = jsoninput["rgb"].GetInt();
        //     // // Clear buffer
        //     // memset(buffer, 0, sizeof(buffer));
        //     // //sendstriprgb(strip, rgbwaarde, client);
        //     // muur.sendRGB(strip,rgbwaarde,client);
        // }

        // else if(json.id == 2){
        //     client.connecting("192.168.137.251");
            
            
        //     // Print the server response
        //     client.receive(buffer, sizeof(buffer));
        //     std::cout << "Server message to bewaker: " << buffer << "\n";
           
        //     std::cout << "Bewaker options: {'deur':openDeur} or {'deur':sluitDeur}" << std::endl;
        //     //std::string userInput; //userinput van user
        //     std::getline(std::cin, userInput);

        //     //rapidjson::Document jsoninput;
        //     jsoninput.Parse(userInput.c_str());
        //     // Validate JSON structure
        //     if (!jsoninput.IsObject() || !jsoninput.HasMember("deur")){
        //         std::cerr << "Invalid input. Please enter valid JSON with 'deur':openDeur or 'deur':sluitDeur" << std::endl;
        //         continue;  // Go back to the beginning of the loop
        //     }

        //     // Save the JSON input to a file
        //     json.saveJsonToFile(jsoninput, "input.json");

        //     //lees waarde deur
        //     std::string waarde_deur = jsoninput["deur"].GetString();

        //     usleep(500000);
        //     set_deur(waarde_deur, deur, client);
        // }

        

        client.disconnect();
        }
    }

    return 0;
}
