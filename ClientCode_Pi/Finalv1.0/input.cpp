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

// void saveJsonToFile(const rapidjson::Document& json, const std::string& filename) {
//     rapidjson::StringBuffer s;
//     rapidjson::Writer<rapidjson::StringBuffer> writer(s);
//     json.Accept(writer);

//     std::ofstream file(filename, std::ios::app);  // Open file in append mode
//     if (file.is_open()) {
//         file << s.GetString() << std::endl;  // Append newline after each JSON
//         file.close();
//     } else {
//         std::cerr << "Unable to open file: " << filename << std::endl;
//     }
// }

void saveJsonToFile(const rapidjson::Document& json, const std::string& filename) {
    rapidjson::StringBuffer s;
    rapidjson::Writer<rapidjson::StringBuffer> writer(s);
    json.Accept(writer);

    std::ofstream file(filename, std::ios::app);  // Open file in append mode
    if (file.is_open()) {
        if (file.tellp() == 0) {
            // If the file is empty, start with an array
            file << "[" << std::endl;
        } else {
            // Otherwise, separate objects with a comma and newline
            file << "," << std::endl;
        }
        file << s.GetString();  // Append the JSON object
        file.close();
    } else {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }
}


int main(){
    while(true){
        std::cout << "ID=1,2,3,4" << std::endl;
        std::cout << "Enter choice followed by values note {'id':1}:" << std::endl;


        // Read JSON input from user
        // std::string userInput;
        std::getline(std::cin, userInput);

        jsoninput.Parse(userInput.c_str());

        // Validate JSON structure
        if (!jsoninput.IsObject() || !jsoninput.HasMember("id")){
            std::cerr << "Invalid input. Please enter valid JSON with 'id'" << std::endl;
            continue;
            // Go back to the beginning of the loop
        }
        
        // Save the JSON input to a file
        saveJsonToFile(jsoninput, "input.json");

        // json.id = jsoninput["id"].GetInt();
        // json.idJson(userInput);
        id = jsoninput["id"].GetInt();
        

        if (id == 1) {
            std::cout << "Strip=0,1 RGB 0..4" << std::endl;
            std::cout << "Enter choice followed by values note {'strip':1,'rgb':1}:" << std::endl;

            // Read JSON input from user
            std::string userInput;
            std::getline(std::cin, userInput);

            rapidjson::Document jsoninput;
            jsoninput.Parse(userInput.c_str());
            // Validate JSON structure
            if (!jsoninput.IsObject() || !jsoninput.HasMember("strip") || !jsoninput.HasMember("rgb")){
                std::cerr << "Invalid input. Please enter valid JSON with 'strip' or 'rgb'" << std::endl;
                continue;  // Go back to the beginning of the loop
            }

            // Save the JSON input to a file
            saveJsonToFile(jsoninput, "input.json");

            
        }

        if(id == 2){
            // User input
            std::cout << "Bewaker options: 'openDeur' or 'sluitDeur' ";
            std::cin.getline(buffer, sizeof(buffer));
            buffer[strcspn(buffer, "\n")] = '\0'; // Remove the used byte(s)

            // Open the door
            if (strcmp(buffer, "openDeur") == 0) {
                std::cout << "Deur wordt geopend...\n";
                send(client_fd, &open, sizeof(open), 0);
            }
            // Close the door
            else if (strcmp(buffer, "sluitDeur") == 0) {
                std::cout << "Deur wordt gesloten...\n";
                send(client_fd, &dicht, sizeof(dicht), 0);
            }
            // Invalid input
            else {
                std::cout << "This input is invalid. \n";
            }

            
        }
    }
}


