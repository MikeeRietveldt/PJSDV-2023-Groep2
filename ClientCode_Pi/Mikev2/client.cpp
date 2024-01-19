#include "client.h"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

// Constructor
Client::Client() : sock(-1), lastReceivedState('\0') {
    // Constructor body, if needed
}

// Destructor
Client::~Client() {
    if (sock != -1) {
        disconnect();
    }
}

// Connecting method
void Client::connecting(const char* serverip) {
    struct sockaddr_in server_addr;
    int PORT = 8080;
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

// Sending method
void Client::sending(const char* buffer, size_t length) {
    send(sock, buffer, length, 0);
}

// Receiving method
void Client::receive() {
    char buffer[1];
    int bytesRead = read(sock, buffer, 1);

    if (bytesRead > 0) {
        lastReceivedState = buffer[0];
    } else {
        lastReceivedState = '\0';
    }
}

// Disconnect method
void Client::disconnect() {
    close(sock);
    sock = -1;
}

char Client::getLastReceivedState() const {
    return lastReceivedState;
}