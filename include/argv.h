#ifndef ARGV_H_
#define ARGV_H_

int get_argc(char* argv[]);

int search_argv(char* argv[], char* arg);
void delete_argv(char* argv[], int pos, int n);
void extract_argv(char* dst[], char* src[], int pos, int n);

void dump_argv(char* name, char** argv);

#endif  // ARGV_H_
