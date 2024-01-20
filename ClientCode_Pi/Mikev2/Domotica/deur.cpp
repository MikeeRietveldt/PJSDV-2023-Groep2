#include <iostream>

#include "deur.h"
#include "../client.h"

//deur openen
void Deur::openen(Client &client){
    char een = '1'; //Karakter om te versturen naar de wemos 
    client.sending(&een, 1); //1 karakter versturen.
    laatstGeopend = clock(); //Zet een timestamp vast.
    deurIsOpen = true; // Deze waarde hebben we nodig voor de timer
    std::cout << "Let op! Deur wordt geopend" << std::endl; //Print de hudige situatie

} 

//deur sluiten
void Deur::sluiten(Client &client){
    char drie = '3'; // Karakter om te versturen naar de wemos
    client.sending(&drie, 1); //1 karakter versturen.
    deurIsOpen = false; // Waarde op false zetten zodat de timer niet blijft spammen
    std::cout << "Deur zou gesloten moeten worden" << std::endl; //Print de huidige situatie
} 

//Lampje 3x flitsen binnen na aanbellen buiten.
//Wanneer er buiten aangebeld wordt, moet het lampje van binnen gaan flitsen
void Deur::belflits(Client &client){
    char twee = '2'; // Karakter wat naar wemos verstuurd zal worden
    client.sending(&twee, 1); //1 karakter versturen.
    laatsteFlits = clock(); //Zet een timestamp genaamd laatsteFlits
}

//Live update functie
void Deur::update(Client &client) {
    if (deurIsOpen) {
        clock_t nu = clock(); //Zet een timestamp genaamd nu
        double deurTijd = double(nu - laatstGeopend) / CLOCKS_PER_SEC; //Bereken de tijd dat de deur geoepend is.

        std::cout << "Door has been open for " << deurTijd << " seconds." << std::endl; //Print de timer waardes tot deze afloopt (gebeurd op achtergrond, programma gaat gewoon door)

        //If statement die de deur laat dicht gaan na ~5 seconde
        if (deurTijd >= 0.0025) {
            sluiten(client); //Deur sluiten
            deurTijd = 0; //Timer resetten
        }
    }
}


Deur::Deur(const char* iptje): Meubel(iptje){

    

}