#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "client.h"
#include "Domotica/deur.h"
#include "Domotica/muur.h"
#include "Domotica/meubel.h"

using namespace std;

int main() {
    Deur deur("192.168.137.251"); // Object creation with IP address
    Client client; // Client object creation

    while (true) {
        // Connect to the Deur device
        client.connecting(deur.geefIp());
        client.receive();
        deur.update(client);
        char doorState = client.getLastReceivedState();

    switch (doorState) {
        case '1': 
            deur.openen(client);
        break;
        case '2': 
            deur.belflits(client);
        break;
    // Include cases for other buttons
}

        // Sleep for a short duration to prevent high CPU usage
        sleep(1); // Sleep for 1 second
    }

    return 0;
}