#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#pragma once

// std::string userInput;
// rapidjson::Document jsoninput;

class Client;

class Json {
    public:
    void saveJsonToFile(const rapidjson::Document& json, const std::string& filename);
    rapidjson::Document readJsonFromFile(const std::string& filename);
    void actJsonFromFile(const std::string& filename);
    void inputJson();
    void idJson(const std::string& filename);
    // void processJsonInput(const std::string& filename, Client& client, Deur& deur, Muur& muur);
    void jsonLdrPot(const std::string& filename, Client &client);
    void saveReceivedDataToJson(const std::string& filename, const char* receivedData);

    int id;

    private:
    
    
};