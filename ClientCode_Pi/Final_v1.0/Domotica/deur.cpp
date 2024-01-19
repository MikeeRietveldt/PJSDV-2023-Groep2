#include <iostream>

#include "deur.h"
#include "../client.h"
// #include "meubel.h"


//deur openen
void Deur::openen(Client &client){
    const char openCommando = '1';
    client.sending(&openCommando, sizeof(openCommando)); //1 karakter versturen.

    laatstGeopend = clock(); //Zet een timestamp vast.
    deurIsOpen = true;
    std::cout << "Deur wordt geopend" << std::endl;

} 

//deur sluiten
void Deur::sluiten(Client &client){
    char sluitCommando = '0';
    client.sending(&sluitCommando, sizeof(sluitCommando)); //1 karakter versturen.
    deurIsOpen = false;
    std::cout << "Deur wordt gesloten" << std::endl;
} 

//Lampje 3x flitsen binnen na aanbellen buiten.
void Deur::belflits(Client &client){
    const char flitsCommando = '2'; 
    client.sending(&flitsCommando, sizeof(flitsCommando));
}

//Live update functie
void Deur::update(Client &client){
    clock_t nu = clock(); // huidige tijd dat het programma draait.
    clock_t deurTijd = (nu - laatstGeopend) / CLOCKS_PER_SEC; // in seconde berekenen hoelang deur open is.

    if(deurTijd >= 5){
        sluiten(client); // Sluit de deur na 5 seconde
    }


}

void Deur::set_deur(std::string commando, Deur &deur, Client &client){
     // Open de deur
     char dicht = 0;
     char open = 1;
    char sendBuffer[64];
    if(commando == "openDeur"){
        deur.openen(client);
    }
    // sluit de deur
    else if(commando == "sluitDeur"){
        client.sending(&dicht, sizeof(dicht));
    }

}