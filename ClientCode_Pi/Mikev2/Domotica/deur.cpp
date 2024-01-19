#include <iostream>

#include "deur.h"
#include "../client.h"

//deur openen
void Deur::openen(Client &client){
    char een = '1';
    client.sending(&een, 1); //1 karakter versturen.
    laatstGeopend = clock(); //Zet een timestamp vast.
    deurIsOpen = true;
    std::cout << "Deur wordt geopend" << std::endl;

} 

//deur sluiten
void Deur::sluiten(Client &client){
    char drie = '3';
    client.sending(&drie, 1); //1 karakter versturen.
    deurIsOpen = false;
    std::cout << drie << std::endl;
    std::cout << "Deur zou gesloten moeten worden" << std::endl;
} 

//Lampje 3x flitsen binnen na aanbellen buiten.
//Wanneer er buiten aangebeld wordt, moet het lampje van binnen gaan flitsen
void Deur::belflits(Client &client){
    char twee = '2';
    client.sending(&twee, 1); //1 karakter versturen.
    laatsteFlits = clock();
}

//Live update functie
void Deur::update(Client &client) {
    if (deurIsOpen) {
        clock_t nu = clock();
        double deurTijd = double(nu - laatstGeopend) / CLOCKS_PER_SEC;

        std::cout << "Door has been open for " << deurTijd << " seconds." << std::endl;

        if (deurTijd >= 0.0025) {
            sluiten(client);
            deurTijd = 0;
        }
    }
}


Deur::Deur(const char* iptje): Meubel(iptje){

    

}