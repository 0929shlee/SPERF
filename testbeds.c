#include "testbeds.h"

int get_file_size_as_byte(char* c_arr)
{
	int ein = 0;
	int eim = 0;
	int res = 0;
	int i;

	while ('0' <= c_arr[ein] && c_arr[ein] <= '9')
	{
		ein += 1;
	}

	for (i = 0; i < ein; i += 1)
	{
		res *= 10;
		res += (c_arr[i] - '0');
	}
	
	if (c_arr[ein] == 'k' || c_arr[ein] == 'K')
		res *= 1024;
	else if (c_arr[ein] == 'm' || c_arr[ein] == 'M')
		res *= (1024 * 1024);
	else if (c_arr[ein] == 'g' || c_arr[ein] == 'G')
		res *= (1024 * 1024 * 1024);
	else {}

	eim = (c_arr[ein] == 'b' || c_arr[ein] == 'B') ? ein + 1 : ein + 2;
	if (c_arr[eim - 1] == 'b')
		res /= 8;
	
	return res;
}

int get_current_time_as_milliseconds()
{
	struct timeb tb;
	struct tm tstruct;
	int res = 0;
	
	ftime(&tb);
	if (NULL != localtime_r(&tb.time, &tstruct)) 
	{
		res = tstruct.tm_hour * 60 * 60 * 1000 + 
			tstruct.tm_min * 60 * 1000 + 
			tstruct.tm_sec * 1000 +
			tb.millitm;
    	}
	else
		res = -1;
	
	return res;
}

int get_frequency(char* c_arr)
{
	int ein = 0;
	int res = 0;
	int i;

	while ('0' <= c_arr[ein] && c_arr[ein] <= '9')
	{
		ein += 1;
	}

	for (i = 0; i < ein; i += 1)
	{
		res *= 10;
		res += (c_arr[i] - '0');
	}
	
	if (c_arr[ein] == 'k' || c_arr[ein] == 'K')
		res *= 1000;
	else if (c_arr[ein] == 'g' || c_arr[ein] == 'G')
		res *= (1000 * 1000);
	else {}
	
	return res;
}

int get_duration_as_seconds(char* c_arr)
{
	int ein = 0;
	int res = 0;
	int i;

	while ('0' <= c_arr[ein] && c_arr[ein] <= '9')
	{
		ein += 1;
	}

	for (i = 0; i < ein; i += 1)
	{
		res *= 10;
		res += (c_arr[i] - '0');
	}
	
	if (c_arr[ein] == 'm' || c_arr[ein] == 'M')
	{
		if (c_arr[ein + 1] == 's' || c_arr[ein + 1] == 'S')
			res /= 1000;
		else
			res *= 60;
	}
	else if (c_arr[ein] == 'h' || c_arr[ein] == 'H')
		res *= (60 * 60);
	else if (c_arr[ein] == 's' || c_arr[ein] == 'S') {}
	else {}
	
	return res;
}

int max(int a, int b)
{
	return a > b ? a : b;
}

void read_file(char* file_name, char* str)
{
	const int MAX_LEN = 20;
	char buf[MAX_LEN];

	FILE* fp = fopen(file_name, "r"); 

	int i = 0;
	while (feof(fp) == 0)
	{
		fgets(buf, MAX_LEN, fp);
		for (int j = 0; j < MAX_LEN; j += 1)
		{
			if (buf[j] == 0)
				break;
			str[i] = buf[j];
			i += 1;
		}
	}
	fclose(fp);
}

char** read_metadata()
{
	const int METADATA_MAX_LEN = 200;
	char str[METADATA_MAX_LEN];
	char file_name[] = "metadata.txt";

	read_file(file_name, str);
	return split(str, '\n');
}

char** split(char* str, char c)
{
	const int MAX_LEN = 20;
	int n_row = count_c(str, c) + 1;
	int i, j;

	char** s_arr = malloc(sizeof(char*) * n_row);
	for (i = 0; i < n_row; i += 1)
	{
		s_arr[i] = malloc(sizeof(char) * MAX_LEN);
		for (j = 0; j < MAX_LEN; j += 1)
			s_arr[i][j] = 0;
	}

	int row = 0;
	j = 0;
	for (i = 0; str[i] != 0; i += 1)
	{
		if (str[i] == c)
		{
			row += 1;
			j = 0;
		}
		else
		{
			s_arr[row][j] = str[i];
			j += 1;
		}
	}
	return s_arr;
}

int count_c(char* str, char c)
{
	int res = 0;
	for (int i = 0; str[i] != 0; i += 1)
	{
		if (str[i] == c)
			res += 1;
	}
	return res;
}
