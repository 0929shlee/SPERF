#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include "testbeds.h"

void* socket_thread(void* arg);

int* receiver_sock;
int* sender_sock;

struct sockaddr_in* receiver_addr;
struct sockaddr_in* sender_addr;
socklen_t* sender_addr_size;

char** s_arr;

int main(int argc, char* argv[])
{
    int n_hosts = atoi(argv[1]);
	s_arr = read_metadata();

    receiver_sock = malloc(sizeof(int) * (n_hosts - 1));
    sender_sock = malloc(sizeof(int) * (n_hosts - 1));

    receiver_addr = malloc(sizeof(struct sockaddr_in) * (n_hosts - 1));
    sender_addr = malloc(sizeof(struct sockaddr_in) * (n_hosts - 1));
    sender_addr_size = malloc(sizeof(socklen_t) * (n_hosts - 1));

    pthread_t* thread_ids = malloc(sizeof(pthread_t) * (n_hosts - 1));
    int* thread_nums = malloc(sizeof(int) * (n_hosts - 1));
    for (int i = 0; i < (n_hosts - 1); i += 1)
    {
        thread_nums[i] = i;
        pthread_create(&thread_ids[i], NULL, socket_thread, (void*)&thread_nums[i]);
    }
    printf("pthread_create\n");

    for (int i = 0; i < (n_hosts - 1); i += 1)
    {
        pthread_join(thread_ids[i], NULL);
    }
    printf("pthread_join\n");

    return 0;
}

void* socket_thread(void* arg)
{
    int thread_num = *((int*)arg);
	char** metadata = split(s_arr[thread_num], ' ');

    /* 1. Connect */
    /* 1-1. Socket */
    memset(&receiver_addr[thread_num], 0, sizeof(receiver_addr[thread_num]));
    receiver_addr[thread_num].sin_family = AF_INET;
    receiver_addr[thread_num].sin_addr.s_addr = htonl(INADDR_ANY);
    receiver_addr[thread_num].sin_port = htons(10000 + thread_num);

    receiver_sock[thread_num] = socket(AF_INET, SOCK_STREAM, 0);
    if (receiver_sock[thread_num] == -1)
    {
        printf("socket error: %d\n", thread_num);
        return NULL;
    }
    printf("socket: %d\n", thread_num);
    
    /* 1-2. Bind */
    if (bind(receiver_sock[thread_num], (struct sockaddr*)&receiver_addr[thread_num], sizeof(receiver_addr[thread_num])) == -1)
    {
        printf("bind error: %d\n", thread_num);
        close(receiver_sock[0]);
        return NULL;
    }
    printf("bind: %d\n", thread_num);

    /* 1-3. Listen */
    if (listen(receiver_sock[thread_num], 5) == -1)
    {
        printf("listen error: %d\n", thread_num);
        close(receiver_sock[0]);
        return NULL;
    }
    printf("listen: %d\n", thread_num);
    
    /* 1-4. Accept */
    sender_addr_size[thread_num] = sizeof(sender_addr[thread_num]);
    sender_sock[thread_num] = accept(receiver_sock[thread_num], (struct sockaddr*)&receiver_addr[thread_num], &sender_addr_size[thread_num]);
    if (sender_sock[thread_num] == -1)
    {
        printf("accept error: %d\n", thread_num);
        close(receiver_sock[thread_num]);
        close(sender_sock[thread_num]);
        return NULL;
    }
    printf("accept: %d\n", thread_num);


    /* 2. Receive */

	int i, j, k, l;
    char buf_r[20];

	const int fs = get_file_size_as_byte(metadata[0]);
	const int freq = get_frequency(metadata[1]);
	const int dura = get_duration_as_seconds(metadata[2]);

	const int nf = (fs - 1) / 1024 + 1;

    /* 2-1. Reserve data container */
	char data[1024];
	data[1023] = 0;
    char ack[2];
    ack[1] = 0;

    /* 2-2. Receive data */
	printf("Receiving...: %d\n", thread_num);

    for (i = 0; i < dura; i += 1)
    {
        for (j = 0; j < freq; j += 1)
        {
			for (k = 0; k < nf; k += 1)
            {
    			while (1)
                {
                    read(sender_sock[thread_num], data, sizeof(data));
                    if (data[1020] == (i % 128) &&
                        data[1021] == (j % 128) &&
                        data[1022] == (k % 128))
                    {
                        ack[0] = 1;
                        write(sender_sock[thread_num], ack, sizeof(ack));
                        ack[0] = 0;

                        data[1020] = 0;
                        data[1021] = 0;
                        data[1022] = 0;
                        break;
                    }
                    else
                    {
                        ack[0] = 0;
                        write(sender_sock[thread_num], ack, sizeof(ack));
                    }
                } 
            }
        }
    }

	printf("Receiving complete!: %d                            \n\n", thread_num);


    /* 3. Ending sequence */
    close(receiver_sock[thread_num]);
    close(sender_sock[thread_num]);
    printf("\nclose: %d\n", thread_num);

    return NULL;
}