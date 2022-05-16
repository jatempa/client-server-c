// Client side C/C++ program to demonstrate Socket
// programming
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#define PORT 8080

typedef struct
{
  int num1;
  int num2;
} nums;

int main(int argc, char const *argv[])
{
  int sock = 0, valread;
  struct sockaddr_in serv_addr;
  nums payload;

  if (argc != 3)
  {
    printf("You need to execute like this: ./client num1 num2\n");
    exit(1);
  }

  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    printf("\n Socket creation error \n");
    return -1;
  }

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);

  // Convert IPv4 and IPv6 addresses from text to binary
  // form
  if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
  {
    printf(
        "\nInvalid address/ Address not supported \n");
    return -1;
  }

  if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
  {
    printf("\nConnection Failed \n");
    return -1;
  }

  payload.num1 = atoi(argv[1]);
  payload.num2 = atoi(argv[2]);

  printf("Send to server num1 = %d, num2 = %d\n", payload.num1, payload.num2);

  if (send(sock, &payload, sizeof(payload), 0) == -1)
  {
    perror("Error: sending two values to server");
  }

  close(sock);

  return 0;
}
