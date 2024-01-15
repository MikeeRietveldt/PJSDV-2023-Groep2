#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/error/en.h"

#include "json.h"
#include "Domotica/muur.h"
#include "Domotica/deur.h"
#include "client.h"

Json json;
Muur muur;
Client client;

extern std::string userInput;
extern rapidjson::Document jsoninput;



void Json::saveJsonToFile(const rapidjson::Document& json, const std::string& filename) {
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


void Json::inputJson(){
    std::cout << "ID=1,2,3,4" << std::endl;
    std::cout << "Enter choice followed by values note {'id':1}:" << std::endl;


    // Read JSON input from user
    // std::string userInput;
    std::getline(std::cin, userInput);

    jsoninput.Parse(userInput.c_str());

    // Validate JSON structure
    if (!jsoninput.IsObject() || !jsoninput.HasMember("id")){
        std::cerr << "Invalid input. Please enter valid JSON with 'id'" << std::endl;
        // Go back to the beginning of the loop
    }

    // Save the JSON input to a file
    json.saveJsonToFile(jsoninput, "input.json");

    json.id = jsoninput["id"].GetInt();
    json.idJson(userInput);
}

void Json::idJson(const std::string& filename){
    if (json.id == 1) {
            client.connecting("192.168.137.248");
            muur.readLDR(client);
            muur.readPotent(client);
            muur.vraagInput(client);
            // std::cout << "Strip=0,1 RGB 0..4" << std::endl;
            // std::cout << "Enter choice followed by values note {'strip':1,'rgb':1}:" << std::endl;

            // // Read JSON input from user
            // std::getline(std::cin, userInput);

            // //rapidjson::Document jsoninput;
            // jsoninput.Parse(userInput.c_str());
            // // Validate JSON structure
            // if (!jsoninput.IsObject() || !jsoninput.HasMember("strip") || !jsoninput.HasMember("rgb")){
            //     std::cerr << "Invalid input. Please enter valid JSON with 'strip' or 'rgb'" << std::endl;
            //     continue;  // Go back to the beginning of the loop
            // }

            // // Save the JSON input to a file
            // json.saveJsonToFile(jsoninput, "input.json");

            // //lees waarden strip
            // int strip = jsoninput["strip"].GetInt();
            // int rgbwaarde = jsoninput["rgb"].GetInt();
            // // Clear buffer
            // memset(buffer, 0, sizeof(buffer));
            // //sendstriprgb(strip, rgbwaarde, client);
            // muur.sendRGB(strip,rgbwaarde,client);
            client.disconnect();
        }

    else if(json.id == 2){
        Deur deur;
        client.connecting("192.168.137.251");
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
            // continue;  // Go back to the beginning of the loop
        }

        // Save the JSON input to a file
        json.saveJsonToFile(jsoninput, "input.json");

        //lees waarde deur
        std::string waarde_deur = jsoninput["deur"].GetString();

        usleep(500000);
        deur.set_deur(waarde_deur, deur, client);
    }
}


rapidjson::Document Json::readJsonFromFile(const std::string& filename) {
    rapidjson::Document jsonDocument;

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Unable to open file: " << filename << std::endl;
        return jsonDocument;  // Return an empty document in case of an error
    }

    std::string jsonContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    jsonDocument.Parse(jsonContent.c_str());

    if (jsonDocument.HasParseError()) {
        std::cerr << "JSON parse error: " << GetParseError_En(jsonDocument.GetParseError()) << std::endl;
        return rapidjson::Document();  // Return an empty document in case of a parse error
    }

    return jsonDocument;
}


void actJsonFromFile(const std::string& filename){
    rapidjson::Document fileJson = json.readJsonFromFile(filename);

    if (!fileJson.IsNull()) {
        // Validate JSON structure
        if (!fileJson.IsObject() || !fileJson.HasMember("id")) {
            std::cerr << "Invalid JSON structure in file" << std::endl;
            return;
        }

        // Extract values from fileJson
        int fileStrip = 0, fileRgb = 0;

        if (fileJson.HasMember("strip") && fileJson["strip"].IsInt()) {
            fileStrip = fileJson["strip"].GetInt();
        }

        if (fileJson.HasMember("rgb") && fileJson["rgb"].IsInt()) {
            fileRgb = fileJson["rgb"].GetInt();
        }
        muur.sendRGB(fileStrip,fileRgb, client);

        // Now you can use fileStrip and fileRgb in your code
        std::cout << "Read strip from file: " << fileStrip << std::endl;
        std::cout << "Read rgb from file: " << fileRgb << std::endl;

        // Clear the file content after processing
        std::ofstream clearFile(filename, std::ios::trunc);
        clearFile.close();
    }
}