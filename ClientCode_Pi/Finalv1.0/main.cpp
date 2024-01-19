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


int main() {
    Deur deur; // object aanmaken
    Client client; // object aanmaken
    Muur muur;
    Json json;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        return -1;
    }
    while (true) {
        muur.begin(); // mijn hele run
        // Jouw code hier
        json.input(); // input van die input file
    }

    return 0;
}
