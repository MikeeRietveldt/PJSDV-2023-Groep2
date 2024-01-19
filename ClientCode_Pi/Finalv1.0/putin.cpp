#include <iostream>
#include <fstream>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

void createInputJson(int id, int strip, int rgb, const std::string& filename) {
    rapidjson::Document jsonDocument;
    jsonDocument.SetObject();

    rapidjson::Document::AllocatorType& allocator = jsonDocument.GetAllocator();

    // Add id to the JSON object
    rapidjson::Value idValue;
    idValue.SetInt(id);
    jsonDocument.AddMember("id", idValue, allocator);

    // If id is 1, add strip and rgb to the JSON object
    if (id == 1) {
        rapidjson::Value stripValue;
        stripValue.SetInt(strip);
        jsonDocument.AddMember("strip", stripValue, allocator);

        rapidjson::Value rgbValue;
        rgbValue.SetInt(rgb);
        jsonDocument.AddMember("rgb", rgbValue, allocator);
    
    }
    // Convert JSON object to string
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    jsonDocument.Accept(writer);

    // Save the JSON string to the file
    std::ofstream file(filename);
    if (file.is_open()) {
        file << buffer.GetString() << std::endl;
        file.close();
        std::cout << "JSON structure saved to " << filename << std::endl;
    } else {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }
}

int main() {
    int id, strip, rgb;

    // Get user input for id, strip, and rgb
    std::cout << "Enter id: ";
    std::cin >> id;

    std::cout << "Enter strip: ";
    std::cin >> strip;

    std::cout << "Enter rgb: ";
    std::cin >> rgb;

    // Create and save the input JSON structure
    createInputJson(id, strip, rgb, "input.json");

    return 0;
}
