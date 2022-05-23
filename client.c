#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include "custom_types.h"

#define PORT 8080

student createStudent()
{
  student item;

  printf("Introduce el nombre:\n");
  scanf("%[^\n]%*c", item.firstName);
  printf("Introduce el apellido:\n");
  scanf("%[^\n]%*c", item.lastName);

  return item;
}

int main(int argc, char const *argv[])
{
  int sock = 0, valread;
  struct sockaddr_in serv_addr;

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

  student st = createStudent();

  if (send(sock, &st, sizeof(st), 0) == -1)
  {
    perror("Error: sending two values to server");
  }

  close(sock);

  return 0;
}
