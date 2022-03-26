#include "job.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static struct job* current;
static struct job* prev;

static int jobid = 1;
struct job* jobs; /* Head of the job list */

static int find_jobid_from_pgid(int pgid) {
  struct job* entry = jobs;

  do {
    if (!entry) {
      break;
    }
    if (entry->pgid == pgid) {
      return entry->jobid;
    }
    entry = entry->next;
  } while (entry != jobs);

  return -1;
}

static void job_state_to_string(enum job_state state, char* buff) {
  memset(buff, 0, 10);
  switch (state) {
    case JOB_RUNNING:
      strcpy(buff, "Running");
      break;
    case JOB_STOPPED:
      strcpy(buff, "Stopped");
      break;
  }
}

static char* command_to_string(char** command) {
  char* buff = malloc(30);
  char* ret = buff;
  for (int i = 0; command[i]; i++) {
    strcpy(buff, command[i]);
    buff += sizeof(command[i]);
  }
  buff = 0;
  return ret;
}

void init_jobs() {
  jobs = (struct job*)malloc(sizeof(struct job));
  jobs->jobid = -1;
  jobs->next = jobs;
  jobs->prev = jobs;
  jobs->pid = -1;
  jobs->pgid = -1;
}

void add_job(int pid, int pgid, char** command) {
  int id;
  struct job* entry = (struct job*)malloc(sizeof(struct job));

  if ((id = find_jobid_from_pgid(pgid)) == -1) {
    id = jobid++;
  }

  entry->jobid = id;
  entry->command = command_to_string(command);
  entry->pid = pid;
  entry->pgid = pgid;

  entry->next = jobs;
  entry->prev = jobs->prev;
  jobs->prev->next = entry;
  jobs->prev = entry;
}

int delete_job(int pid) {
  struct job* entry;
  for (entry = jobs->next; entry != jobs; entry = entry->next) {
    if (entry->pid == pid) {
      entry->prev->next = entry->next;
      entry->next->prev = entry->prev;
      return 0;
    }
  }
  printf("Failed to delete job\n");
  return -1;
}

int stop_job(int pid) {
  struct job* entry;
  for (entry = jobs->next; entry != jobs; entry = entry->next) {
    if (entry->pid == pid) {
      entry->state = JOB_STOPPED;
      return 0;
    }
  }
  printf("Failed to stop job\n");
  return -1;
}

void show_jobs() {
  struct job* entry;
  char* buff_state = malloc(10);

  printf("%s %10s %20s\n", "[id]", "[state]", "[command]");

  for (entry = jobs->next; entry != jobs; entry = entry->next) {
    job_state_to_string(entry->state, buff_state);
    printf("[%d] %10s %20s\n", entry->jobid, buff_state, entry->command);
  }

  free(buff_state);
}