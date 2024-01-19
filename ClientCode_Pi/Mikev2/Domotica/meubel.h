// declaratie
#pragma once
#include <cstddef>

class Client;

class Meubel {

public:
virtual void  update(Client &client) = 0; // Hierdoor is het puur virtual en is er geen implementatie.
Meubel(const char*); // construeert een object van de class, geen deconstructor.
const char* geefIp();

private:
const char* ipadressje;
};