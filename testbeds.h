#ifndef _TESTBEDS_H_
#define _TESTBEDS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/timeb.h>
#include <unistd.h>

#define SERVER_IP "10.0.0.1"

int get_file_size_as_byte(char*);
int get_current_time_as_milliseconds();
int get_frequency(char*);
int get_duration_as_seconds(char*);
int max(int, int);
void read_file(char*, char*);
char** read_metadata();
char** split(char*, char);
int count_c(char*, char);

#endif
