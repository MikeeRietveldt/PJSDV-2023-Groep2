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

#define PORT 1234
#define SERVER_IP "192.168.137.248"
#define MIK_PORT 80
#define MIK_IP "192.168.137.251"

int sock = 0;
int ldr;
int id;
struct sockaddr_in server_addr;
struct sockaddr_in serv_addr;

// Communication with Arduino server
char buffer[1024] = { 0 };
ssize_t valread;

void id1_server(){

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
    }

    memset(&server_addr, '0', sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
    }

    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection Failed");
    }
    
}

void id2_server(){
     if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            perror("Socket creation error");
        }

        memset(&server_addr, '0', sizeof(server_addr));

        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(MIK_PORT);

        if (inet_pton(AF_INET, MIK_IP, &server_addr.sin_addr) <= 0) {
            perror("Invalid address/ Address not supported");
        }

        if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
            perror("Connection Failed");
        }
}
void id3_server(){

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
    }

    memset(&server_addr, '0', sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(MIK_PORT);

    if (inet_pton(AF_INET, MIK_IP, &server_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
    }

    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection Failed");
    }
    
}



void sendstriprgb(int led, int rgb) {
    // Send the extracted values to the server
    char sendBuffer[64];
    snprintf(sendBuffer, sizeof(sendBuffer), "%d,%d,%d", ldr, led, rgb);
    // std::cout << "LDR IS: " << std::endl;
    // std::cout << ldr << std::endl;
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
        std::cout << "ID=0,1,2" << std::endl;
        std::cout << "Enter choice followed by values note {'id':1}:" << std::endl;

        // Read JSON input from user
        std::string userInput;
        std::getline(std::cin, userInput);

        rapidjson::Document jsoninput;
        jsoninput.Parse(userInput.c_str());

        // Validate JSON structure
        if (!jsoninput.IsObject() || !jsoninput.HasMember("id")){
            std::cerr << "Invalid input. Please enter valid JSON with 'id'" << std::endl;
            continue;  // Go back to the beginning of the loop
        }

        // Save the JSON input to a file
        saveJsonToFile(jsoninput, "input.json");

        id = jsoninput["id"].GetInt();
        

        if (id == 1) {
            id1_server();
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

            //lees waarden strip
            int strip = jsoninput["strip"].GetInt();
            int rgbwaarde = jsoninput["rgb"].GetInt();

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
            sendstriprgb(strip, rgbwaarde);
        }

        if(id == 2){
            char open = 1;
            char dicht = 2;
            int status, valread, client_fd;
            //id3_server();
            if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            std::cerr << "\n Socket creation error \n";
            return -1;
            }
            
            serv_addr.sin_family = AF_INET;
            serv_addr.sin_port = htons(MIK_PORT);

            // Convert IPv4 and IPv6 addresses from text to binary form
            if (inet_pton(AF_INET, "192.168.137.251", &serv_addr.sin_addr) <= 0) {
                std::cerr << "\nInvalid address/ Address not supported \n";
                return -1;
            }

            if ((status = connect(client_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0) {
                std::cerr << "\nConnection Failed \n";
                return -1;
            }
            
            // Print the server response
            valread = read(client_fd, buffer, sizeof(buffer) - 1);
            std::cout << "Server message to bewaker: " << buffer << "\n";
            buffer[strcspn(buffer, "\n")] = '\0'; // Remove the used byte(s)

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

            // Reset the buffer
            buffer[valread] = '\0';

            // Close the connected socket
            close(client_fd);
        }

        

        close(sock);
    }

    return 0;
}
