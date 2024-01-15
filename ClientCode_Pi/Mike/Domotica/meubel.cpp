#include <iostream>
#include <cstring>

#include "meubel.h"
#include "../client.h"


Meubel::Meubel(const char* iptje): ipadressje(iptje){


}

const char* Meubel::geefIp(){

    return ipadressje; //ipp adress terug geven

}