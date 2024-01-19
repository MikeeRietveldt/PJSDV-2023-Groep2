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
    Muur muur("192.168.137.248"); // Object creation with IP address
    // Initialize other necessary components

    while (true) {
        // Connect to the Deur device
        client.connecting(deur.geefIp());

        // Receive the current state from the Arduino
        client.receive(); // This updates the lastReceivedState in the client object

        // Retrieve the last received state
        char doorState = client.getLastReceivedState();

        // Handle door state
        switch (doorState) {
            case '0': // Door should be closed
                deur.sluiten(client);
                break;
            case '1': // Door should be opened
                deur.openen(client);
                break;
            case '2': // Start flashing light routine
                
                break;
            // Handle other cases as necessary
        }

        // Similar logic for the Muur device if needed

        // Sleep for a short duration to prevent high CPU usage
        sleep(1); // Sleep for 1 second (adjust as needed)
    }

    // Clean up and close connections if necessary
    // ...

    return 0;
}
