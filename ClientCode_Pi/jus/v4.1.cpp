#include <iostream>
#include <fstream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#define PORT 1234
#define SERVER_IP "192.168.137.248"

int sock = 0;
int ldr;
int id;
struct sockaddr_in server_addr;

// Communication with Arduino server
char buffer[1024] = { 0 };
ssize_t valread;

void setServer(int led, int rgb) {
    // Send the extracted values to the server
    char sendBuffer[64];
    snprintf(sendBuffer, sizeof(sendBuffer), "%d,%d,%d", ldr, led, rgb);
    send(sock, sendBuffer, strlen(sendBuffer), 0);
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

int main() {
    while (true) {
        std::cout << "ID=0,1,2 Strip=0,1 RGB 0..4" << std::endl;
        std::cout << "Enter choice followed by values note {'id':1,'strip':1,'rgb':1}:" << std::endl;

        // Read JSON input from user
        std::string userInput;
        std::getline(std::cin, userInput);

        rapidjson::Document jsoninput;
        jsoninput.Parse(userInput.c_str());

        // Validate JSON structure
        if (!jsoninput.IsObject() || !jsoninput.HasMember("id") || !jsoninput.HasMember("strip") || !jsoninput.HasMember("rgb")) {
            std::cerr << "Invalid input. Please enter valid JSON with 'id', 'strip', and 'rgb'." << std::endl;
            continue;  // Go back to the beginning of the loop
        }

        // Save the JSON input to a file
        saveJsonToFile(jsoninput, "input.json");

        id = jsoninput["id"].GetInt();
        int strip = jsoninput["strip"].GetInt();
        int rgbwaarde = jsoninput["rgb"].GetInt();

        if (id == 1) {
            if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
                perror("Socket creation error");
                return -1;
            }

            memset(&server_addr, '0', sizeof(server_addr));

            server_addr.sin_family = AF_INET;
            server_addr.sin_port = htons(PORT);

            if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
                perror("Invalid address/ Address not supported");
                return -1;
            }

            if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
                perror("Connection Failed");
                return -1;
            }

            valread = read(sock, buffer, sizeof(buffer));
            unsigned int anin0 = atoi(buffer);
            std::cout << "ldr value: " << anin0 << std::endl;
            usleep(500000);

            if (anin0 < 180) {
                ldr = 1;
            } else {
                ldr = 0;
            }

            // Clear buffer
            memset(buffer, 0, sizeof(buffer));
        }

        setServer(strip, rgbwaarde);

        close(sock);
    }

    return 0;
}
