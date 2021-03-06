// Client side C/C++ program to demonstrate Socket
// programming
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include "custom_types.h"

#define PORT 8080

int main(int argc, char const *argv[])
{
  int sock = 0, valread;
  struct sockaddr_in serv_addr;
  nums payload;
  int addition_result, subtraction_result, multiplication_result;
  float division_result;

  if (argc != 3)
  {
    printf("usage: ./client <first_number> <second_number>\n");
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

  if (recv(sock, &addition_result, sizeof(int), 0) == -1)
  {
    perror("Error: receiving two values from server with addition result");
  }

  if (recv(sock, &subtraction_result, sizeof(int), 0) == -1)
  {
    perror("Error: receiving two values from server with subtraction result");
  }

  if (recv(sock, &multiplication_result, sizeof(int), 0) == -1)
  {
    perror("Error: receiving two values from server with multiplication result");
  }

  if (recv(sock, &division_result, sizeof(int), 0) == -1)
  {
    perror("Error: receiving two values from server with division result");
  }

  printf("Addition: %d + %d = %d\n", payload.num1, payload.num2, addition_result);
  printf("Subtraction: %d - %d = %d\n", payload.num1, payload.num2, subtraction_result);
  printf("Multiplication: %d x %d = %d\n", payload.num1, payload.num2, multiplication_result);
  printf("Division: %d / %d = %.4f\n", payload.num1, payload.num2, division_result);

  close(sock);

  return 0;
}
