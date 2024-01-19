#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#pragma once


class Client;

class Json {
    public:
    void saveJsonToFile(const rapidjson::Document& json, const std::string& filename);
    rapidjson::Document readJsonFromFile(const std::string& filename);
    void actJsonFromFile(const std::string& filename, Client client);
    void inputJson();
    void idJson(const std::string& filename);
    void jsonLdrPot(const std::string& filename, Client &client);
    void saveReceivedDataToJson(const std::string& filename, const char* receivedData);
    int selectId(const std::string& filename);
    void idCheck(int identifier, const std::string& filename);
    void input();

    int id;

    private:
    
    
};