#include <arpa/inet.h>
#include <sys/socket.h>
#include "testbeds.h"

int main(int argc, char* argv[])
{
	int host_num = atoi(argv[1]);
	char** s_arr = read_metadata();
	char** metadata = split(s_arr[host_num - 2], ' ');


    /* 1. Connect */

    int receiver_sock;
    struct sockaddr_in receiver_addr;
    int str_len;

    /* 1-1. Socket */
    memset(&receiver_addr, 0, sizeof(receiver_addr));
    receiver_addr.sin_family = AF_INET;
    receiver_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    receiver_addr.sin_port = htons(9998 + host_num);

    receiver_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (receiver_sock == -1)
    {
        printf("socket error \n");
        exit(1);
    }
    //printf("socket\n");

    /* 1-2. Connect */
    if (connect(receiver_sock, (struct sockaddr*)&receiver_addr, sizeof(receiver_addr)) == -1)
    {
        printf("connect error\n");
    	close(receiver_sock);
        exit(1);
    }
    //printf("connect\n");


    /* 2. Send */

	int i, j, k;
	char buf_s[20];
	int sndr_time, res_time, rmdr_time;

	const int fs = get_file_size_as_byte(metadata[0]);
	const int freq = get_frequency(metadata[1]);
	const int dura = get_duration_as_seconds(metadata[2]);

	const int nf = (fs - 1) / 1024 + 1;
	const int us = 1000 * 1000 / freq;
	int* res_time_arr = malloc(sizeof(int) * dura * freq);

    /* 2-1. Data generation */
	char data[1024];
	data[1023] = 0;
	char ack[2];
	ack[1] = 0;

    /* 2-2. Send data */
	rmdr_time = 0;
	for (i = 0; i < dura; i += 1)
	{
		data[1020] = (i % 128);
		for (j = 0; j < freq; j += 1)
		{
			data[1021] = (j % 128);
			sndr_time = get_current_time_as_milliseconds();
			for (k = 0; k < nf; k += 1) 
			{
				data[1022] = (k % 128);
				while (1)
				{
    				write(receiver_sock, data, sizeof(data));
					read(receiver_sock, ack, sizeof(ack));
					if (ack[0] == 1)
					{
						ack[0] = 0;
						break;
					}
				}
				data[1022] = 0;
			}
			
			res_time = get_current_time_as_milliseconds();
			data[1021] = 0;

			if (res_time - sndr_time + rmdr_time <= (1000 / freq))
			{
				res_time_arr[i * freq + j] = res_time - sndr_time + rmdr_time;
				rmdr_time = 0;
				usleep(us - res_time_arr[i * freq + j] * 1000);
			}
			else
			{
				res_time_arr[i * freq + j] = res_time - sndr_time + rmdr_time;
				rmdr_time += (res_time - sndr_time - (1000 / freq));
			}

			printf("%d ", res_time_arr[i * freq + j]);
		}
		data[1020] = 0;
	} 


    /* 4. Ending sequence */
    /* 4-1. Delete the data */
	free(res_time_arr);

    /* 4-2. Close socket */
    close(receiver_sock);
    //printf("\nclose\n");

    return 0;
}
