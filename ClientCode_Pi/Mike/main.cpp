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

using namespace std;

#define PORT 1234
#define SERVER_IP "192.168.137.248"
#define MIK_PORT 80
#define MIK_IP "192.168.137.251"

int sock = 0;
int ldr;
int id;
struct sockaddr_in server_addr;
struct sockaddr_in serv_addr;
char open = 1;
char dicht = 2;
int status, client_fd; //exc valread

std::string userInput;
rapidjson::Document jsoninput;

// Communication with Arduino server
char buffer[1024] = { 0 };

void sendstriprgb(int led, int rgb, Client &client) {
    // Send the extracted values to the server
    char sendBuffer[64];
    snprintf(sendBuffer, sizeof(sendBuffer), "%d,%d,%d", ldr, led, rgb);
    // std::cout << "LDR IS: " << std::endl;
    // std::cout << ldr << std::endl;
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

void set_deur(std::string deur, Client &client){
     // Open the door
    char sendBuffer[64];
    if(deur == "openDeur"){
        //snprintf(sendBuffer, sizeof(sendBuffer), "%d", open);
        client.sending(&open, sizeof(open));
        cout << "Deur wordt geopend" << endl;
    }
    else if(deur == "sluitDeur"){
        //snprintf(sendBuffer, sizeof(sendBuffer), "%d", dicht);
        client.sending(&dicht, sizeof(dicht));
        cout << "Deur wordt gesloten" << endl; 
    }
    //std::cout << "opdracht verstuurd" << std::endl;

}

int main() {

    Client client;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        return -1;
    }
    while (true) {
        std::cout << "ID=1,2,3,4" << std::endl;
        std::cout << "Enter choice followed by values note {'id':1}:" << std::endl;


        // Read JSON input from user
        std::string userInput;
        std::getline(std::cin, userInput);

        //rapidjson::Document jsoninput;
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
            client.connecting("192.168.137.248");
            std::cout << "Strip=0,1 RGB 0..4" << std::endl;
            std::cout << "Enter choice followed by values note {'strip':1,'rgb':1}:" << std::endl;

            // Read JSON input from user
            //std::string userInput;
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

            client.receive(buffer, sizeof(buffer));
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
            sendstriprgb(strip, rgbwaarde, client);
        }

        else if(id == 2){
            client.connecting("192.168.137.251");
            
            
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

            //lees waarden deur
            std::string waarde_deur = jsoninput["deur"].GetString();
            

            //set_deur(waarde_deur);

           usleep(500000);
            // Reset the buffer
            //buffer[valread] = '\0';
            memset(buffer, 0, sizeof(buffer));
            set_deur(waarde_deur, client);

            // Close the connected socket
            //close(client_fd);
        }

        

        client.disconnect();
    }

    return 0;
}
