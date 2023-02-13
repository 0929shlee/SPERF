gcc testbeds.c -c
gcc sender.c -c
gcc receiver.c -c
gcc test.c -c
gcc sender.o testbeds.o -o sender
gcc receiver.o testbeds.o -o receiver -pthread
gcc test.o testbeds.o -o test
rm -rf sender.o testbeds.o receiver.o test.o
