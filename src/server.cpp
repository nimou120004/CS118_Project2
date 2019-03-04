#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <thread>
#include <cstring>
#include <deque>

#include "packet.hpp"

using namespace std;

#define DATA_BUFFER_SIZE 512
#define TOTAL_BUFFER_SIZE 524
int thread_id = 0;
static const int num_threads = 11;
int SS_THRESH = 10000;
int cgwn_size = 1; //should be 512, will change later

void sig_quit_handler(int s)
{
  cout << "Interrupt signal (SIGQUIT) received.\n";
  exit(0);
}

void sig_term_handler(int s)
{
  cout << "Interrupt signal (SIGTERM) received.\n";
  exit(0);
}



int main(int argc, char* argv[]){

    signal(SIGQUIT, sig_quit_handler);
    signal(SIGTERM, sig_term_handler);

    if (argc != 3) {
        cerr << "ERROR: Number of arguments incorrect. Try ./server <PORT> <FILE-DIR>\n";
        exit(EXIT_FAILURE);
    }

    int port = atoi(argv[1]);
    char *save_directory = argv[2];

    struct stat st = {0};
    if (stat(save_directory, &st) == -1) {
        mkdir(save_directory, 0700);
    }

    if (port < 1024 || port > 65535) {
        cerr << "ERROR: Port numbers incorrect. Try another one.\n";
        exit(EXIT_FAILURE);
    }



  int udpSocket, nBytes;
  char buffer[TOTAL_BUFFER_SIZE];
  struct sockaddr_in serverAddr, clientAddr;
  struct sockaddr_storage serverStorage;
  socklen_t addr_size, client_addr_size;

  /*Create UDP socket*/
  udpSocket = socket(PF_INET, SOCK_DGRAM, 0);

  /*Configure settings in address struct*/
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(port);
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

  /*Bind socket with address struct*/
  bind(udpSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

  /*Initialize size variable to be used later on*/
  addr_size = sizeof serverStorage;


// save to file
  char file_name[30];
  sprintf(file_name,"%s/%d.file", save_directory, 1);

  ofstream output(file_name, ios::out | ios::trunc | ios::binary);


  while(1){
    memset(buffer, '\0', sizeof(buffer));
    nBytes = recvfrom(udpSocket,buffer,sizeof(buffer),0,(struct sockaddr *)&serverStorage, &addr_size);

    // cout << buffer << endl;
    packet recv_pack(buffer);
  
    cout << "received byte " << nBytes << endl;
    cout << "recv_pack.header.seq_num " << ntohl(recv_pack.header.seq_num) << endl;
    cout << "recv_pack.header.ack_num " << ntohl(recv_pack.header.ack_num) << endl;
    cout << "recv_pack.header.ID " << ntohs(recv_pack.header.ID) << endl;
    cout << "recv_pack.header.flag " << ntohs(recv_pack.header.flag) << endl;
    cout << "recv_pack.data " << recv_pack.data << endl;

    if (nBytes == -1){
        cerr << "ERROR: byte receive error";
        output.close();
        close(udpSocket);
    }


    if (nBytes == 12){
        break;
    }

    output.write(recv_pack.data, nBytes-12);
    printf("server received %d bytes\n", nBytes);

//    sendto(udpSocket,buffer,nBytes,0,(struct sockaddr *)&serverStorage,addr_size);
  }

 output.close();
//  close(udpSocket);
  return 0;
}