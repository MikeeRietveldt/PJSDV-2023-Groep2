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
    Deur deur("192.168.137.251"); // Object aanmaken met ip
    Client client; // Client object aanmaken

    while (true) {
        // Connect to the Deur device
        client.connecting(deur.geefIp());
        client.receive();
        deur.update(client);
        char doorState = client.getLastReceivedState();
    // hier nog functie voor maken.
    switch (doorState) {
        case '1': 
            deur.openen(client);
        break;
        case '2': 
            deur.belflits(client);
        break;
        case '3': 
            deur.sluiten(client); // Deze case is extra, wordt eigenijk niet echt gebruikt aangezien deuropenen na een clock_timer automatisch de deur sluit
        break;
    // hier eventueel andere cases
}

        // Mini sleep om hoge CPU usage te voorkomen
        sleep(1); // Sleep voor een seconde
    }

    return 0;
}