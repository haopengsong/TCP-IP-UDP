/*
Project
Phase#1
Client
Name:Haopeng Song
Beej, p27
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

#define MAXDATA 3000
#define IPADDR 50
#define PORT "4061" //hardcode port number 3300 + 761
#define LOCAL "localhost" //server IP

void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}
void *get_in_port(struct sockaddr *sa) {

    return &(((struct sockaddr_in*)sa)->sin_port);

}
void tcpConn1(){
        int socket_fd, new_socket, bytesRead,connectInt,bindInt,rv;
    char bufferreader[MAXDATA];
    struct addrinfo hints, *servinfo, *p;
    char s[MAXDATA];
    socklen_t len = sizeof(struct sockaddr);
    struct sockaddr_in addr, foo;
    char tweetNum[MAXDATA] = "A";
    struct sockaddr_storage their_addr;
    socklen_t addr_len;
    addr_len = sizeof their_addr;

    //set up address info
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; //ipv4 or ipv6
    hints.ai_socktype = SOCK_STREAM;
    if ((rv = getaddrinfo(LOCAL, PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        exit(1);
    }

    //Beej, p26, p28
    // loop through all the results and connect to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((socket_fd = socket(p->ai_family, p->ai_socktype,
                                p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }

        if (connect(socket_fd, p->ai_addr, p->ai_addrlen) == -1) {
            perror("client: connect");
            close(socket_fd);
            continue;
        }
        bind(socket_fd,p->ai_addr,p->ai_addrlen);

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        exit(1);
    }

    getsockname(socket_fd, (struct sockaddr *) &foo, &len);
    //send(socket_fd,"A",MAXDATA,0);

    printf("Upon startup of Phase 1: TweetA has TCP port %d and IP address %s for Phase 1 \n",ntohs(foo.sin_port), inet_ntoa(foo.sin_addr));

    printf("Upon establishing a TCP connection to the server: TweetA is now connected to the Server\n");


    //retrive tweet from tweet#.txt
    vector<string> tweets;
    ifstream ifile("TweetA.txt");
    string fileLine;
    while(getline(ifile,fileLine)){
        fileLine = tweetNum + fileLine;
        tweets.push_back(fileLine);
    }

    //tell server how many packets are coming
    stringstream ss;
    int num_packets = tweets.size();
    ss << num_packets;
    string num_packets_str = ss.str();
    send(socket_fd,num_packets_str.c_str(),MAXDATA,0);

    string tosend;
    //cout << "size  : " << tweets.size() << endl;
    for(int m=0;m<tweets.size();m++){
        tosend = tweets[m];
        sendto(socket_fd,tosend.c_str(),MAXDATA,0,p->ai_addr, p->ai_addrlen);
       
        cout << "Sending tweet information to the server: TweetA has sent "<< tosend.substr(1) << " to server \n";
    }

    
    printf("Upon sending all the tweets’ information to the server: Updating the server is done for TweetA  \n");
    printf("End of Phase 1: End of Phase 1 for TweetA\n");

    //blocking call

    recvfrom(socket_fd, s, MAXDATA, 0, (struct sockaddr *)&their_addr, &addr_len);
    //cout << "ack " << s << endl;



}

void tcpConn2(){
        int socket_fd, new_socket, bytesRead,connectInt,bindInt,rv;
    char bufferreader[MAXDATA];
    struct addrinfo hints, *servinfo, *p;
    char s[MAXDATA];
    socklen_t len = sizeof(struct sockaddr);
    struct sockaddr_in addr, foo;
    char tweetNum[MAXDATA] = "A";

    //set up address info
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; //ipv4 or ipv6
    hints.ai_socktype = SOCK_STREAM;
    if ((rv = getaddrinfo(LOCAL, PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        exit(1);
    }

    //Beej, p26, p28
    // loop through all the results and connect to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((socket_fd = socket(p->ai_family, p->ai_socktype,
                                p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }

        if (connect(socket_fd, p->ai_addr, p->ai_addrlen) == -1) {
            perror("client: connect");
            close(socket_fd);
            continue;
        }
        bind(socket_fd,p->ai_addr,p->ai_addrlen);

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        exit(1);
    }
    
    send(socket_fd,tweetNum,MAXDATA,0);
    getsockname(socket_fd, (struct sockaddr *) &foo, &len);
    //send(socket_fd,"A",MAXDATA,0);
    char sizenum[MAXDATA];

    printf("Upon startup of Phase 2: TweetA has TCP port %d and IP address %s for Phase 2 \n",ntohs(foo.sin_port), inet_ntoa(foo.sin_addr));
    cout << "Receiving one follower feedback information " << endl;

    recv(socket_fd, sizenum , MAXDATA , 0 );
    string sizestr =(string)sizenum;
    int sizeint = stoi(sizestr);
    int countt2 = 0;

    //cout <<"final : " << sizenum << endl;

    while(1){
        recv(socket_fd, s , MAXDATA , 0 );
        cout << s << endl;
        countt2++;
        if(countt2 == sizeint){
            break;
        }

    }
    // recvfrom(udpsockfd, buf,MAXDATA,0,(struct sockaddr *)&their_addr, &addr_len);
    // recvfrom(socket_fd, )

    cout << "End of phase 2 End of phase 2 for TweetA" << endl;
}
int main(int argc, char const *argv[])
{
    tcpConn1();
    tcpConn2();
    return 0;
}
