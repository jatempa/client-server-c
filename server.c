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
#define MAX_THREADS 50

void *addition(void *);
void *subtraction(void *);
void *multiplication(void *);
void *division(void *);

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char const *argv[])
{
  int server_fd, new_socket, valread;
  struct sockaddr_in address;
  int opt = 1;
  int addrlen = sizeof(address);
  nums payload;
  pthread_t thid_addition[MAX_THREADS], thid_subtraction[MAX_THREADS],
      thid_multiplication[MAX_THREADS], thid_division[MAX_THREADS];
  int *addition_result, *subtraction_result, *multiplication_result, *division_result;
  int count_threads = 0;

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

    if (pthread_create(&thid_addition[count_threads], NULL, &addition, &payload))
    {
      printf("Error: Addition Thread was not created");
      exit(-1);
    }

    if (pthread_create(&thid_subtraction[count_threads], NULL, &subtraction, &payload))
    {
      printf("Error: Subtraction Thread was not created");
      exit(-1);
    }

    if (pthread_create(&thid_multiplication[count_threads], NULL, &multiplication, &payload))
    {
      printf("Error: Multiplication Thread was not created");
      exit(-1);
    }

    if (pthread_create(&thid_division[count_threads], NULL, &division, &payload))
    {
      printf("Error: Division Thread was not created");
      exit(-1);
    }

    pthread_join(thid_addition[count_threads], (void *)&addition_result);
    pthread_join(thid_subtraction[count_threads], (void *)&subtraction_result);
    pthread_join(thid_multiplication[count_threads], (void *)&multiplication_result);
    pthread_join(thid_division[count_threads], (void *)&division_result);

    int addition_r = *(int *)addition_result;
    int subtraction_r = *(int *)subtraction_result;
    int multiplication_r = *(int *)multiplication_result;
    float division_r = *(float *)division_result;

    if (send(new_socket, &addition_r, sizeof(addition_r), 0) == -1)
    {
      printf("Error: Addition result was not sent");
    }

    if (send(new_socket, &subtraction_r, sizeof(subtraction_r), 0) == -1)
    {
      printf("Error: Subtraction result was not sent");
    }

    if (send(new_socket, &multiplication_r, sizeof(multiplication_r), 0) == -1)
    {
      printf("Error: Multiplication result was not sent");
    }

    if (send(new_socket, &division_r, sizeof(division_r), 0) == -1)
    {
      printf("Error: Division result was not sent");
    }

    free(addition_result);
    free(subtraction_result);
    free(multiplication_result);
    free(division_result);

    count_threads++;
    if (count_threads > MAX_THREADS)
    {
      count_threads = 0;
    }

    close(new_socket);
  }

  return 0;
}

void *addition(void *args)
{
  nums *payload = (nums *)args;
  int *result = malloc(sizeof(int));

  pthread_mutex_lock(&mutex);
  *result = payload->num1 + payload->num2;
  pthread_mutex_unlock(&mutex);

  return result;
}

void *subtraction(void *args)
{
  nums *payload = (nums *)args;
  int *result = malloc(sizeof(int));

  pthread_mutex_lock(&mutex);
  *result = payload->num1 - payload->num2;
  pthread_mutex_unlock(&mutex);

  return result;
}

void *multiplication(void *args)
{
  nums *payload = (nums *)args;
  int *result = malloc(sizeof(int));

  pthread_mutex_lock(&mutex);
  *result = payload->num1 * payload->num2;
  pthread_mutex_unlock(&mutex);

  return result;
}

void *division(void *args)
{
  nums *payload = (nums *)args;
  float *result = malloc(sizeof(float));

  pthread_mutex_lock(&mutex);
  *result = (float)payload->num1 / payload->num2;
  pthread_mutex_unlock(&mutex);

  return result;
}