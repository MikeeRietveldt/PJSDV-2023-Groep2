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
    void inputJson();
    void idJson(const std::string& filename);
    int id;

    private:
    
    
};