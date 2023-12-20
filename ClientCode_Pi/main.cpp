//Client test
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <wiringPi.h>
#define PORT 80 // De poort waar naar verbonden wordt

int main(int argc, char const* argv[])
{
	int status, valread, client_fd;
	struct sockaddr_in serv_addr;
	//char* hello = "Hallo, de bewaker is verbonden!";
	char buffer[1024] = { 0 };
	char open = 1;
	char dicht = 2;
	
	while (1) {

	if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("\n Socket creation error \n");
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	// Convert IPv4 and IPv6 addresses from text to binary
	// form
	if (inet_pton(AF_INET, "192.168.137.183", &serv_addr.sin_addr)
		<= 0) {
		printf(
			"\nInvalid address/ Address not supported \n");
		return -1;
	}


		if ((status
			= connect(client_fd, (struct sockaddr*)&serv_addr,
				sizeof(serv_addr)))
			< 0) {
			printf("\nConnection Failed \n");
			return -1;
		}


		// Zorgen dat we berichten kunnen versturen.


			// Print de server reactie
		valread = read(client_fd, buffer, sizeof(buffer) - 1);
		printf("Server message to bewaker: %s\n", buffer);
		buffer[strcspn(buffer, "\n")] = '\0'; // Verwijderen van de gebruikte byte(s)

		//Reactie aan client

		printf("Bewaker opties: 'openDeur' of 'sluitDeur' ");
		fgets(buffer, sizeof(buffer), stdin); // input van user aannemen
		buffer[strcspn(buffer, "\n")] = '\0'; // Verwijderen van de gebruikte byte(s)

		// Deur open
		if (strcmp(buffer, "openDeur") == 0) {
			printf("Deur wordt geopend...\n");
			send(client_fd, &open, sizeof(open), 0);
		}
		// Deur sluiten
		else if (strcmp(buffer, "sluitDeur") == 0) {
			printf("Deur wordt gesloten...\n");
			send(client_fd, &dicht, sizeof(dicht), 0);
		}
		// Foute input aangeven
		else {
			printf("Deze input is ongeldig. \n");
		}

		// Buffer resetten zodat deze leeg is
		buffer[valread] = '\0';

		// closing the connected socket
		//printf("Bewaker zijn actie verstuurd.\n");
		close(client_fd);
		//test commit
	}
	return 0;
}
