#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 1234
#define SERVER_IP "192.168.137.248"
int sock = 0;
int ldr;
int id;
struct sockaddr_in server_addr;
// Communication with Arduino server
char buffer[1024] = { 0 };
ssize_t valread;


void setServer(int led, int rgb){
    
    // Send the extracted values to the server
    char sendBuffer[64];
    snprintf(sendBuffer, sizeof(sendBuffer), "%d,%d,%d", ldr, led, rgb);
    //printf("The sent buffer is %c", sendBuffer);
    send(sock, sendBuffer, strlen(sendBuffer), 0);   

    

}


int main() {
    printf("ID=0,1,2 Strip=0,1 RGB 0..4\n");
    printf("Enter choice followed by values (e.g., 0,2): \n");
    fgets(buffer, sizeof(buffer), stdin);

    // Remove newline character from the input
    buffer[strcspn(buffer, "\n")] = 0;

    // Validate user input
    if (strlen(buffer) < 3 || (buffer[1] != ',' && buffer[2] != ',')) {
        printf("Invalid input. Please enter a valid choice followed by values.\n");
    }

    // Extract values using sscanf
    int strip, rgbwaarde;
    sscanf(buffer, "%d,%d,%d", &id, &strip, &rgbwaarde);

    if (id = 1){
        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        return -1;
        }

        memset(&server_addr, '0', sizeof(server_addr));

        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(PORT);

        if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
            perror("Invalid address/ Address not supported");
            return -1;
        }

        if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
            perror("Connection Failed");
            return -1;
        }
        valread = read(sock, buffer, sizeof(buffer));
        unsigned int anin0 = atoi(buffer);
        printf("ldr value: %d\n", anin0);
        usleep(500000);
            if (anin0 < 180) {
                //printf("Dark, Wall ON\n");
                ldr = 1;
            } else {
                //printf("Bright, Wall OFF\n");
                ldr = 0;
            }
        // Clear buffer
        memset(buffer, 0, sizeof(buffer));
    }
    setServer(strip,rgbwaarde);

    close(sock);

    return 0;
}

