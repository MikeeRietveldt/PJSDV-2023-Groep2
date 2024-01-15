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