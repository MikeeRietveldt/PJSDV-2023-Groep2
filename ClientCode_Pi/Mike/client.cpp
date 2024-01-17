//definitie
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h> 

#include "client.h"


// pointer wijst naar eerste byte in het ip.
void Client::connecting(const char* serverip) {
    struct sockaddr_in server_addr;
    struct sockaddr_in serv_addr;
    int PORT = 8080; // Port aangeven die je gaat gebruiken. Deze kan hetzelfde blijven voor elk device aangezien er statische ip's zijn.
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
    }

    memset(&server_addr, '0', sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, serverip, &server_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
    }

    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection Failed");
    }
}

// pointer wijst naar de data.
void Client::sending(const char* buffer, size_t length){
    send(sock, buffer, length, 0);
} 

// ontvangt de data.
void Client::receive(char* buffer, size_t size){
     // Zorgen dat read() niet meer blocking is.
    int flags = fcntl(sock, F_GETFL, 0); // flags instellen
    fcntl(sock, F_SETFL, flags | O_NONBLOCK); // non-blocking mode aan zetten

    // Uitlezen van inkomende data
    int valread = read(sock, buffer, size - 1);
    buffer[valread] = '\0';

    if (valread > 0) {
        std::cout << "Server message to bewaker: " << buffer << "\n";
    } else if (valread == 0) {
        std::cout << "Connection closed by the server\n";
    } else if (errno == EWOULDBLOCK || errno == EAGAIN){

    }  // Afhandelen van andere errors
        else{
            perror("read");
            } 
}

// disconnect van de huidige server.
void Client::disconnect(){

    close(sock); // sluit de socket
    sock = -1;
}

// construeert een object van de class.
Client::Client():sock(-1){ // nog geen sock, 1 cpu cycle minder door het hier zo te doen.

}

// destructureert een object van de class.
Client::~Client(){
    if (sock != -1){ // wanneer er een socket is sluiten we deze.
        disconnect();
    }

}

