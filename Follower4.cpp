#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

#define PORT "4061" //hardcode port number 3300 + 761
#define LOCAL "localhost" //server IP
#define u4 "22461"
#define MAXDATA 3000
#define IPADDR 50

string removeSpaces(string input)
{
  input.erase(std::remove(input.begin(),input.end(),' '),input.end());
  return input;
}
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

//read the first line of input file and store following info to a
//vector, if no following exists return empty vector
string isFollowing(char const * filename) {
    ifstream ifile(filename);
    string firstLine;
    string followList;
    int pos = -1;

    getline(ifile,firstLine);
    //cout << firstLine << endl;
    pos = firstLine.find(":");

    followList = firstLine.substr(pos+1);
    //cout << followList << endl;
    //vector<string> following;


    return removeSpaces(followList);
}
//check which 
string whichTweet(string input){
    if(input[input.length()-1] == 'A'){
        return "a";
    }
    else if(input[input.length()-1] == 'B'){
        return "b";
    }
    else if(input[input.length()-1] == 'C'){
        return "c";
    }
    return "";
}
//store tweet number which follower has liked
string followerLiked(char const * filename){
    ifstream ifile(filename);
    string secondLine;
    string likedWhich;
    int counter = 0;
    string tweet;
    while(getline(ifile,secondLine)){
        int pos = 0;
        //cout << "ss " << secondLine << endl;
        if(counter == 0){
            counter++;
            continue;
        }
        else {
            secondLine = removeSpaces(secondLine);
           // cout << "ss " << secondLine << endl;
            pos = secondLine.find(":");
            //cout << "ll " << pos << endl;
            //cout << "lll " << secondLine.length() << endl;
            if(pos == secondLine.length()-1){
                //cout << "ss s" << endl;
                continue;
            }
            else {

                tweet = whichTweet(secondLine.substr(0,pos)) + tweet;
               // cout << tweet << endl;
            }
        }
        
    }
   //counter = 0;
    return tweet;
}
void tcpConn2() {
    int socket_fd, new_socket, bytesRead,connectInt,bindInt,rv;
    char bufferreader[MAXDATA];
    struct addrinfo hints, *servinfo, *p;
    char s[MAXDATA];
    socklen_t len = sizeof(struct sockaddr);
    struct sockaddr_in addr, foo;

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
    for(p = servinfo; p != NULL; p = p->ai_next)
    {
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

    //struct sockaddr_in addr, foo;
    //socklen_t len = sizeof(struct sockaddr);
    getsockname(socket_fd, (struct sockaddr *) &foo, &len);
    printf("Upon sending all packets to the server: Follower4 has TCP port %d and IP address %s for Phase 2 \n",ntohs(foo.sin_port), inet_ntoa(foo.sin_addr));


    //sending packets to server
    cout << "Sending feedback information to the server \n";
    string following;
    string f1liked;
    following = isFollowing("Follower4.txt");
    f1liked = followerLiked("Follower4.txt");

    send(socket_fd,following.c_str(),MAXDATA,0);

    send(socket_fd,f1liked.c_str(),MAXDATA,0);

    cout << "Completed sending feedback for <Follower4>" << endl;
    cout << "End of Phase 2 : End of Phase 2 for <Follower4>" << endl;





}
void tcpConn() {
    int socket_fd, new_socket, bytesRead,connectInt,bindInt,rv;
    char bufferreader[MAXDATA];
    struct addrinfo hints, *servinfo, *p;
    char s[MAXDATA];
    socklen_t len = sizeof(struct sockaddr);
    struct sockaddr_in addr, foo;

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
    send(socket_fd,"4",MAXDATA,0);



}
void udpConn() {
    //create UDP , reference Beej's
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    int numbytes;
    struct sockaddr_storage their_addr;
    socklen_t addr_len;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    if ((rv = getaddrinfo(LOCAL, u4, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        exit(1);
    }
    // loop through all the results and make a socket
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                             p->ai_protocol)) == -1) {
            perror("talker: socket");
            continue;
        }
        break;
    }
    if (p == NULL) {
        fprintf(stderr, "talker: failed to create socket\n");
        exit(1);
    }

    char ip_client[IPADDR];
    //print Server IP
    char hostname[128];
    gethostname(hostname, sizeof hostname);
    //printf("My hostname: %s\n", hostname);

    inet_ntop(AF_INET,get_in_addr((struct sockaddr *)p->ai_addr),ip_client,sizeof ip_client);

    cout << "Upon start of Phase 2: <Follower4> has UDP port "<<u4<<" and IP address "<<ip_client<<endl;


    char rece[]="receive";
    sendto(sockfd, rece, strlen(rece), 0 ,p->ai_addr, p->ai_addrlen);


    addr_len = sizeof their_addr;

    char data_rev[MAXDATA];
    cout << "Receiving all the packets from Server: ";
    recvfrom(sockfd,data_rev,sizeof data_rev,0,(struct sockaddr *)&their_addr, &addr_len);
    cout << "<Follower4> has received <Tweet"<<data_rev[0]<<">"<< endl;
    // recvfrom(sockfd,data_rev,sizeof data_rev,0,(struct sockaddr *)&their_addr, &addr_len);
    // cout << "<Follower4> has received <Tweet"<<data_rev[0]<<">"<< endl;
    // recvfrom(sockfd,data_rev,sizeof data_rev,0,(struct sockaddr *)&their_addr, &addr_len);
    // cout << "<Follower4> has received <Tweet"<<data_rev[0]<<">"<< endl;
}
int main(int argc, char const *argv[])
{

    tcpConn();
    usleep(500000);
    udpConn();
    usleep(500000);
    tcpConn2();
    return 0;
}