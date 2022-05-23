#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include "custom_types.h"

#define PORT 8080

void *createStudent(void *);

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char const *argv[])
{
  int server_fd, new_socket, valread;
  struct sockaddr_in address;
  int opt = 1;
  int addrlen = sizeof(address);
  pthread_t thid_addition;
  student st;

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

    if (read(new_socket, &st, sizeof(st)) == 1)
    {
      perror("Error: receiving data\n");
      exit(1);
    }

    if (pthread_create(&thid_addition, NULL, &createStudent, &st))
    {
      printf("Error: Addition Thread was not created");
      exit(-1);
    }

    pthread_join(thid_addition, NULL);

    close(new_socket);
  }

  return 0;
}

void *createStudent(void *args)
{
  FILE *fp;
  student st = *(student *)args;

  if (fp == NULL)
  {
    fp = fopen(studentsFile, "w");
  }
  else
  {
    fp = fopen(studentsFile, "a");
  }

  fprintf(fp, "%d\t\t%s\t\t%s\t\t%d\n", st.id, st.firstName, st.lastName, st.career_id);
  fclose(fp);

  pthread_exit(NULL);
}
