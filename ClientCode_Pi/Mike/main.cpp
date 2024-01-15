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
#include "Domotica/meubel.h"


using namespace std;


std::string userInput;
rapidjson::Document jsoninput;


void sendstriprgb(int led, int rgb, Client &client) {
    // Data doorsturen naar de server
    char sendBuffer[64];
    snprintf(sendBuffer, sizeof(sendBuffer), "%d,%d,%d", led, rgb);
    client.sending(sendBuffer, strlen(sendBuffer));
}

void saveJsonToFile(const rapidjson::Document& json, const std::string& filename) {
    rapidjson::StringBuffer s;
    rapidjson::Writer<rapidjson::StringBuffer> writer(s);
    json.Accept(writer);

    std::ofstream file(filename, std::ios::app);  // Open file in append mode
    if (file.is_open()) {
        file << s.GetString() << std::endl;  // Append newline after each JSON
        file.close();
    } else {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }
}

void set_deur(std::string commando, Deur &deur, Client &client){
     // Open de deur
    char sendBuffer[64];
    if(commando == "openDeur"){
        deur.openen(client);
    }
    // sluit de deur
    else if(commando == "sluitDeur"){
        deur.sluiten(client);
    }

}

int main() {
    Deur deur("192.168.137.251"); // object aanmaken en ip meegeven
    Client client; // object aanmaken
    Muur muur("192.168.137.248"); // object aanmaken en ip meegeven
    Meubel *meubels[] = {&deur, &muur}; // array van meubels voor iteratie

    while (true) {

        deur.update(client);

        jsoninput.Parse(userInput.c_str());

        // Validate JSON structure
        if (!jsoninput.IsObject() || !jsoninput.HasMember("id")){
            std::cerr << "Invalid input. Please enter valid JSON with 'id'" << std::endl;
            continue;  // Go back to the beginning of the loop
        }

        // Save the JSON input to a file
        saveJsonToFile(jsoninput, "input.json");

        int id = jsoninput["id"].GetInt();
        

        if (id == 1) {
            client.connecting(muur.geefIp());
            muur.readLDR(client);
            std::cout << "Strip=0,1 RGB 0..4" << std::endl;
            std::cout << "Enter choice followed by values note {'strip':1,'rgb':1}:" << std::endl;

            // Read JSON input from user
            std::getline(std::cin, userInput);

            //rapidjson::Document jsoninput;
            jsoninput.Parse(userInput.c_str());
            // Validate JSON structure
            if (!jsoninput.IsObject() || !jsoninput.HasMember("strip") || !jsoninput.HasMember("rgb")){
                std::cerr << "Invalid input. Please enter valid JSON with 'strip' or 'rgb'" << std::endl;
                continue;  // Go back to the beginning of the loop
            }

            // Save the JSON input to a file
            saveJsonToFile(jsoninput, "input.json");

            //lees waarden strip
            int strip = jsoninput["strip"].GetInt();
            int rgbwaarde = jsoninput["rgb"].GetInt();

            //sendstriprgb(strip, rgbwaarde, client);
            muur.sendRGB(strip,rgbwaarde,client);
        }

        else if(id == 2){
            client.connecting(deur.geefIp());
            
            char buffer[64];
            // Print the server response
            client.receive(buffer, sizeof(buffer));
            std::cout << "Server message to bewaker: " << buffer << "\n";
           
            std::cout << "Bewaker options: {'deur':openDeur} or {'deur':sluitDeur}" << std::endl;
            //std::string userInput; //userinput van user
            std::getline(std::cin, userInput);

            //rapidjson::Document jsoninput;
            jsoninput.Parse(userInput.c_str());
            // Validate JSON structure
            if (!jsoninput.IsObject() || !jsoninput.HasMember("deur")){
                std::cerr << "Invalid input. Please enter valid JSON with 'deur':openDeur or 'deur':sluitDeur" << std::endl;
                continue;  // Go back to the beginning of the loop
            }

            // Save the JSON input to a file
            saveJsonToFile(jsoninput, "input.json");

            //lees waarde deur
            std::string waarde_deur = jsoninput["deur"].GetString();

            usleep(500000);
            set_deur(waarde_deur, deur, client);
        }

        

        client.disconnect();
    }

    return 0;
}
