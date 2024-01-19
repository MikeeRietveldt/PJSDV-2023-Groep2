#define PORT 1234
#define SERVER_IP "192.168.137.248"
#define MIK_PORT 80
#define MIK_IP "192.168.137.251"

int sock = 0;
int ldr;
int id;
struct sockaddr_in server_addr;
struct sockaddr_in serv_addr;
char open = 1;
char dicht = 2;
int status, client_fd; //exc valread

char buffer[1024] = { 0 };

char Mbuffer [64];