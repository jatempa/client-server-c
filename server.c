// Server side C/C++ program to demonstrate Socket
// programming
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include "custom_types.h"

#define PORT 8080

void *addition(void *);

int main(int argc, char const *argv[])
{
  int server_fd, new_socket, valread;
  struct sockaddr_in address;
  int opt = 1;
  int addrlen = sizeof(address);
  nums payload;
  pthread_t thid_addition;
  int *addition_result;

  // Creating socket file descriptor
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
  {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  // Forcefully attaching socket to the port 8080
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
  {
    perror("setsockopt");
    exit(EXIT_FAILURE);
  }

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);

  // Forcefully attaching socket to the port 8080
  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
  {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }

  if (listen(server_fd, 3) < 0)
  {
    perror("listen");
    exit(EXIT_FAILURE);
  }

  while (1)
  {
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
    {
      perror("accept");
      exit(EXIT_FAILURE);
    }

    if (read(new_socket, &payload, sizeof(payload)) == 1)
    {
      perror("Error: receiving data\n");
      exit(1);
    }

    printf("Received from client num1 = %d, num2 = %d\n", payload.num1, payload.num2);

    if (pthread_create(&thid_addition, NULL, &addition, &payload))
    {
      printf("Error: Addition Thread was not created");
      exit(-1);
    }

    pthread_join(thid_addition, (void *)&addition_result);

    int addition_r = *(int *)addition_result;

    if (send(new_socket, &addition_r, sizeof(addition_r), 0) == -1)
    {
      printf("Error: Addition result was not sent");
    }

    close(new_socket);
  }

  return 0;
}

void *addition(void *args)
{
  nums *payload = (nums *)args;
  int *result = malloc(sizeof(int));

  *result = payload->num1 + payload->num2;

  return result;
}