#define _POSIX_C_SOURCE 200809L
#include <assert.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 5

char *queue[MAX_SIZE] = {NULL};
int size = 0;

// Function prototypes just so we can match logical flow of functions visually
void process_input();
void add_to_queue(char *str);
void print_queue();
void clear_queue();
void handle_sigint(int sig);

// We don't really have a way to terminate the program and make sure memory is
// freed Assuming user terminates with CTRL+C, I wanted to make sure memory is
// cleared, hence the handler and calling clear_queue()
void handle_sigint(int sig) {
  (void)sig;
  printf("\n");
  clear_queue();
  exit(0);
}

// Process users input checking for commands and adding to queue
void process_input() {
  char *line = NULL;
  size_t len = 0;
  ssize_t nread;

  printf("Enter Input: ");

  while ((nread = getline(&line, &len, stdin)) != -1) {
    assert(nread > 0);

    if (line[nread - 1] == '\n') {
      line[nread - 1] = '\0';
    }

    // Ignore empty input
    if (line[0] == '\0') {
      printf("Enter Input: ");
      continue;
    }

    add_to_queue(line);
    // Check if input is either of our commands
    if (strcmp(line, "print") == 0) {
      print_queue();
    } else if (strcmp(line, "clear") == 0) {
      clear_queue();
    }

    printf("Enter Input: ");
  }

  free(line);
}

// Add users input to the queue
void add_to_queue(char *str) {
  assert(str != NULL);
  assert(size >= 0 && size <= MAX_SIZE);

  if (size == MAX_SIZE) {
    free(queue[0]);
    for (int i = 0; i < MAX_SIZE - 1; i++) {
      queue[i] = queue[i + 1];
    }
    size--;
  }
  queue[size++] = strdup(str);
}

// Print all items in the queue
void print_queue() {
  assert(size >= 0 && size <= MAX_SIZE);

  for (int i = 0; i < size; i++) {
    assert(queue[i] != NULL);
    printf("%s\n", queue[i]);
  }
}

// Clear all items from the queue
void clear_queue() {
  for (int i = 0; i < size; i++) {
    free(queue[i]);
    queue[i] = NULL;
  }
  size = 0;
}

int main() {
  signal(SIGINT, handle_sigint);

  process_input();

  clear_queue();

  return 0;
}
