// declaratie
#pragma once
#include <cstddef>

class Client {

public: 
void connecting(const char*); // pointer wijst naar eerste byte in het ip.
void sending(const char*, size_t); // pointer wijst naar de data. 
void receive(char*, size_t); // ontvangt de data. 
void disconnect(); // disconnect van de huidige server.
Client(); // construeert een object van de class
~Client(); // destructureert een object van de class

private:
int sock; // de verbinding die er gemaakt wordt.


};
