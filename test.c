#include "testbeds.h"

int main()
{
	/*
	printf("test1: get_file_size_as_byte(char*)\n");
	char* size1 = "100mb";
	printf("%s -> %d should be %d\n", size1, get_file_size_as_byte(size1), 100 * 1024 * 1024 / 8);
	char* size2 = "100b";
	printf("%s -> %d should be %d\n", size2, get_file_size_as_byte(size2), 100 / 8);
	char* size3 = "100KB";
	printf("%s -> %d should be %d\n", size3, get_file_size_as_byte(size3), 100 * 1024);

	printf("\ntest2: get_current_time_as_milliseconds()\n");
	printf("current: %d\n", get_current_time_as_milliseconds());
	usleep(100 * 1000);
	printf("after 100ms: %d\n", get_current_time_as_milliseconds());
	*/

	char** s_arr;
	s_arr = read_metadata();
	printf("%s\n", s_arr[0]);
	printf("%s\n", s_arr[1]);

	char** metadata = split(s_arr[0], ' ');
	printf("%s\n", metadata[0]);
	printf("%s\n", metadata[1]);
	printf("%s\n", metadata[2]);

	printf("%d\n", get_file_size_as_byte(metadata[0]));
	printf("%d\n", get_frequency(metadata[1]));
	printf("%d\n", get_duration_as_seconds(metadata[2]));

	
	return 0;
}
