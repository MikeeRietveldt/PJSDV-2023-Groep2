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
    char Mbuffer [64];
    while (true) {
        if(round == 10){ // check if '.' is pressed
            //std::cin.get(); //consume the '.' character
            json.inputJson();
            round = 0;
        } 
        // else {
        // Read out json file
        //rapidjson::Document fileJson = json.readJsonFromFile("input.json");
        //client connect to id=1 without input
        client.connecting("192.168.137.248");   //UNCOMMENCT WANN Muur
        //muur.readLDR(client);
        client.receive(Mbuffer, sizeof(Mbuffer));
        std::cout << "MBuffer is: " << Mbuffer << std::endl;
        // client.disconnect();
        json.saveReceivedDataToJson("output.json", Mbuffer);
        json.jsonLdrPot("output.json", client);
        std::cout << "buffer is nu " << buffer << std::endl;
        client.disconnect();
        memset(Mbuffer, 0, sizeof(Mbuffer));
        //client connect to id=2 without input
        //#######Mike zn code
        client.connecting("192.168.137.251");
        //############BEGIN MIKE NIEUW CODE
        client.receive(buffer, sizeof(buffer));
        switch (buffer[0]) {
            case '0':
                std::cout << "Er is geen activiteit bij de deur" << std::endl;
                break; 
            case '1':
                std::cout << "Peter wil naar buiten" << std::endl;
                break; 
            case '2':
                std::cout << "Er wordt aangebeld!" << std::endl;
                deur.belflits(client); // Flitsen aanroepen
                break;
            default:
                break;
        } 
        memset(buffer, 0, sizeof(buffer));
        client.disconnect();
        //####MIKE NIEUW CODE
        // client.receive(buffer, sizeof(buffer));
        // std::cout << "Server message to bewaker: " << buffer << "\n";
        // deur.set_deur(waarde_deur, deur, client);
        // client.disconnect();
        // std::cout << "client discconnected" << std::endl;
        // memset(buffer, 0, sizeof(buffer));
        // json.jsonInputAct("input.json");
        //########Einde
        // ################TEST VANAF HIER 
        int idTest = json.test("input.json");
        std::cout << "id teruggekregen is: " << idTest << std::endl;
        json.test2(idTest,"input.json");
        usleep(500000);
        
        round++;
        // json.readJsonFromFile("input.json");//leest json format
        // json.actJsonFromFile("input.json");//stuurt uit input de gegevens door dus strip etc       

        // client.disconnect();
        // }
    }

    return 0;
}
