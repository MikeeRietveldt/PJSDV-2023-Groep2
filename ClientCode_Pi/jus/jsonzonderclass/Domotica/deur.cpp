#include <iostream>

#include "deur.h"
#include "../client.h"

//deur openen
void Deur::openen(Client &client){
    char een = 1;
    client.sending(&een, 1); //1 karakter versturen.

    laatstGeopend = clock(); //Zet een timestamp vast.
    deurIsOpen = true;
    std::cout << "Deur wordt geopend" << std::endl;

} 

//deur sluiten
void Deur::sluiten(Client &client){
    char nul = 0;
    client.sending(&nul, 1); //1 karakter versturen.
    deurIsOpen = false;
    std::cout << "Deur wordt gesloten" << std::endl;
} 

//Lampje 3x flitsen binnen na aanbellen buiten.
void Deur::belflits(Client &client){


}

//Live update functie
void Deur::update(Client &client){
    clock_t nu = clock(); // huidige tijd dat het programma draait.
    clock_t deurTijd = (nu - laatstGeopend) / CLOCKS_PER_SEC; // in seconde berekenen hoelang deur open is.
    
    if(deurTijd >= 5){
        sluiten(client); // Sluit de deur na 5 seconde
    }


}
