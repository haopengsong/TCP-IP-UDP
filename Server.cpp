/*
Project
Phase#2
Server
Name:Haopeng Song
ID : 9753089761
Beej, p26, p28, P19
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <cwctype>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <iostream>

using namespace std;

#define PORT "4061" //hardcode port number 3300 + 761
#define MAXDATA 3000 //max data size we can get
#define IPADDR 50 //buffer size for ip
#define LOCAL "localhost" //server IP
#define u1 "22161"
#define u2 "22261"
#define u3 "22361"
#define u4 "22461"
#define u5 "22561"

void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void *_addr_lookup(struct sockaddr *sa)
{

    return &(((struct sockaddr_in*)sa)->sin_addr);
}

void updConnection(string sel,vector<string> vec) {
    string port_sel;
    string foller;
    if(sel == "1") {
        port_sel = u1;
        foller = "Follower1";
    }
    else if(sel == "2") {
        port_sel = u2;
        foller = "Follower2";
    }
    else if(sel=="3") {
        port_sel = u3;
        foller = "Follower3";
    }
    else if(sel == "4") {
        port_sel = u4;
        foller = "Follower4";
    }
    else {
        port_sel = u5;
        foller = "Follower5";
    }
    //set up udp
    int udpsockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // set to AF_INET to force IPv4
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    //create a UDP socket, reference from Beej
    if ((rv = getaddrinfo(LOCAL, port_sel.c_str(), &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        exit(1);
    }
    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((udpsockfd = socket(p->ai_family, p->ai_socktype,
                                p->ai_protocol)) == -1) {
            perror("listener: socket");
            continue;
        }
        if (bind(udpsockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(udpsockfd);
            perror("listener: bind");
            continue;
        }

        break;
    }
    if (p == NULL) {
        fprintf(stderr, "listener: failed to bind socket\n");
        exit(1);
    }
    freeaddrinfo(servinfo);

    char ip_client[IPADDR];
    //print Server IP
    char hostname[128];
    gethostname(hostname, sizeof hostname);
    //printf("My hostname: %s\n", hostname);

    inet_ntop(AF_INET,get_in_addr((struct sockaddr *)p->ai_addr),ip_client,sizeof ip_client);




    cout << "Upon start of Phase 2: The Server has UDP port "<<port_sel<<" and IP address "<<ip_client<<endl;


    //sending tweets to followers
    cout << "Sending tweet information to the followers :";

    //blocking call
    char buf[MAXDATA];
    struct sockaddr_storage their_addr;
    socklen_t addr_len;
    addr_len = sizeof their_addr;

    recvfrom(udpsockfd, buf,MAXDATA,0,(struct sockaddr *)&their_addr, &addr_len);
    //cout << buf << endl;

    string tweetInfo;
    const char * dataSent;
    for(int i=0; i<vec.size(); i++) {

        dataSent = vec[i].c_str();
        // cout << dataSent << endl;
        sendto(udpsockfd,dataSent,sizeof MAXDATA,0,(struct sockaddr *)&their_addr, addr_len);
        cout << "The server has sent <Tweet"<<(vec[i])[0]<<"> to the <"<<foller<<">"<<endl;
    }
}
string checkLiked(string input, string liked, string follower) {
     
    char tweet = towlower(input[input.length()-1]);
   // cout << tweet << endl;
    for(int i=0; i<liked.length(); i++) {
        if(liked[i] == tweet) {
            return "Follower"+follower+"#like";
        }
    }
    return "Follower"+follower;

}
void updater(vector<string> &ta, vector<string> &tb, vector<string> &tc, string follower, string followList,string liked) {
    int pos = 0;
    string toadd;
    pos = followList.find(",");
    if(pos==-1 && !followList.empty()) {
        if(towlower(followList[followList.length()-1]) == 'a') {
            toadd = checkLiked(followList,liked,follower);
            ta.push_back(toadd);
        }
        if(towlower(followList[followList.length()-1]) == 'b') {
            toadd = checkLiked(followList,liked,follower);
            tb.push_back(toadd);
        }
        if(towlower(followList[followList.length()-1]) == 'c') {
            toadd = checkLiked(followList,liked,follower);
            tc.push_back(toadd);
        }
    }

    while(pos!=-1) {
        string following_sub = followList.substr(0,pos);
        followList = followList.substr(pos+1);
        //cout <<"1 :" <<following_sub<< endl;
        if(towlower(following_sub[following_sub.length()-1]) == 'a') {
            toadd = checkLiked(following_sub,liked,follower);
            ta.push_back(toadd);
        }
        if(towlower(following_sub[following_sub.length()-1]) == 'b') {
            toadd = checkLiked(following_sub,liked,follower);
            tb.push_back(toadd);
        }
        if(towlower(following_sub[following_sub.length()-1]) == 'c') {
            toadd = checkLiked(following_sub,liked,follower);
            tc.push_back(toadd);
        }
        pos = followList.find(",");
        if(pos == -1 && !followList.empty())
        {
           // cout <<"2 :" <<followList<< endl;
            if(towlower(followList[followList.length()-1]) == 'a') {
                toadd = checkLiked(followList,liked,follower);
                ta.push_back(toadd);
            }
            if(towlower(followList[followList.length()-1]) == 'b') {
                toadd = checkLiked(followList,liked,follower);
                tb.push_back(toadd);
            }
            if(towlower(followList[followList.length()-1]) == 'c') {
                toadd = checkLiked(followList,liked,follower);
                tc.push_back(toadd);
            }

        }
    }
}

string intochar(int input){
  stringstream ss_;
            ss_ << input;
            string tasize_s = ss_.str();
            return tasize_s;
}
string parseName(string name){
    int pos = name.find("#");
    if(pos == -1){
        return name;
    }
    else {
        return name.substr(0,pos);
    }
}
void feedback(vector<string> vs, int socket, string tweetNum){
    struct sockaddr_storage their_addr;
    socklen_t _size;
    _size = sizeof their_addr;
    char buffer[MAXDATA];
    int pos = 0;
    string tosend;
    string new_name;
    for(int i=0;i<vs.size();i++){
        tosend = "";
        pos = vs[i].find("#");
        if(pos == -1){
            //no like just following
           

            tosend = "<" +  vs[i] + ">" + " is following <Tweet"+ tweetNum+">";
            send(socket, tosend.c_str(), MAXDATA, 0);
        }
        else {
            //liked 
             //refine name
            new_name = parseName(vs[i]);
            tosend ="<" +  new_name + ">" + " is following <Tweet"+ tweetNum+"> \n" + "<" +  new_name  + ">" + " has liked <Tweet"+ tweetNum+">";
            send(socket, tosend.c_str() , MAXDATA , 0);
        }
    }
}
int main(int argc, char const *argv[])
{
    int socket_,socket_new, prep,bind_,followerTCP, socket_sTt;
    struct addrinfo hints, *servinfo, *p; //host info
    struct sockaddr_storage their_addr, stotweet; //connector's info
    int opt = 1;
    char s[MAXDATA]; // data buffer
    char ip_client[IPADDR];
    int chd;

    struct hostent *hostInfo;
    struct in_addr **addrHostInfo;

    string f1_liked, f2_liked, f3_liked, f4_liked, f5_liked;
    vector<string> tA;
    vector<string> tB;
    vector<string> tC;


    //server set up
    memset(&hints, 0, sizeof hints); // make sure struct is empty
    hints.ai_family = AF_INET; //ipv4
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP


    //fill the struct
    prep =  getaddrinfo(LOCAL, PORT, &hints, &servinfo);
    p = servinfo;
    //create socket
    //Beej, p26, p28
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((socket_ = socket(p->ai_family, p->ai_socktype,
                              p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        if (setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR, &opt,
                       sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }

        if (bind(socket_, p->ai_addr, p->ai_addrlen) == -1) {
            close(socket_);
            perror("server: bind");
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo); // all done with this structure


    if (p == NULL)  {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }

    //set up server listening
    listen(socket_, 3);

    //print Server IP
    char hostname[128];
    gethostname(hostname, sizeof hostname);
    //printf("My hostname: %s\n", hostname);

    inet_ntop(AF_INET,get_in_addr((struct sockaddr *)p->ai_addr),ip_client,sizeof ip_client);
    printf("Upon startup of Phase 1: The server has TCP Port %s, and IP addresses: %s \n",PORT, ip_client);

    //create udp connection

    //constantly waiting for connection
    char ip_header[] = "IP:";
    int revc_checker;
    char tweetNum[MAXDATA];
    socklen_t _size, stt_addr_len;



    int counter = 0;
    string str_received = "";
    vector<string> tweets;
    _size = sizeof their_addr;
    stt_addr_len = sizeof stotweet;
    while(1)
    {
        
        socket_new = accept(socket_, (struct sockaddr *)&stotweet, &stt_addr_len);
       
        if(socket_new < 0) {
            printf("error on accept\n");
            return 1;
        }

        //num of packets
        recv(socket_new, s, MAXDATA, 0);
        string num_p_str = (string)s;
        int num_p = stoi(num_p_str);
        //receiving
       // cout <<"size :  " <<  num_p << endl;
        printf("Upon receiving all the tweet information from a Tweet: \n");
        //usleep(300000);

        int re_count = 0;
        while(1){
            recvfrom(socket_new, s, MAXDATA, 0 ,(struct sockaddr *)&stotweet, &stt_addr_len);
            printf("Received tweets list from Tweet%c\n",s[0] );
            re_count++;
            if(re_count == num_p){
                break;
            }
        }

        str_received = (string)s;
        tweets.push_back(str_received);

        if (!fork())
        {   // this is the child process
            close(socket_); // child doesn't need the listener

            close(socket_new);
            exit(0);
        }
       // close(socket_new);
        counter++;
        if(counter == 1) {
            printf("End of Phase 1 :  End of Phase 1 for the Server \n");
            break;
        }

    }
    counter=0;

    //tcp for followers

    while(1) {
        followerTCP = accept(socket_,(struct sockaddr *)&their_addr, &_size);

        if(followerTCP < 0) {
            printf("error on accept\n");
            return 1;
        }

        if ((revc_checker = recv(followerTCP, s, MAXDATA, 0)) == -1) {
            perror("recv");
            return 1;
        }
        //printf("%s\n",s);
        //which follower?
        str_received = (string)s;
        //start of phase 2
        updConnection(str_received.substr(0,1),tweets);


        if (!fork())
        {   // this is the child process
            close(socket_); // child doesn't need the listener

            close(followerTCP);
            exit(0);
        }
        close(followerTCP);



        //start receiving feedback from Follower
        char fbfollower[MAXDATA];
        char fliked[MAXDATA];

        followerTCP = accept(socket_,(struct sockaddr *)&their_addr, &_size);
        if(followerTCP < 0) {
            printf("error on accept\n");
            return 1;
        }
        cout << "Listening to Follower's feedback :";
        printf(" The server has TCP Port %s, and IP addresses: %s \n",PORT, ip_client);
        cout << "Receiving all the packets from a follower: ";
        //void updater(vector<string> ta, vector<string> tb, vector<string> tc, string follower, string followList,string liked)
        recv(followerTCP, fbfollower, MAXDATA, 0);
        usleep(500000);
        recv(followerTCP, fliked, MAXDATA, 0);

        updater(tA,tB,tC,str_received,fbfollower,fliked);

        cout << "Server receive the feedback from <Follower"<<s<<">"<<endl;

        close(followerTCP);
        counter++;
        if(counter == 5) {

            break;
        }

    }
    //     cout << "tA" << endl;
    // for(int i=0; i<tA.size(); i++) {
    //     cout << tA[i] << endl;
    // }
    // cout << endl;

    // cout << "tB" << endl;
    // for(int j=0; j<tB.size(); j++) {
    //     cout << tB[j] << endl;
    // }
    // cout << endl;

    // cout << "tC" << endl;
    // for(int k=0; k<tC.size(); k++) {
    //     cout << tC[k] << endl;
    // }
    // cout << endl;
    //sending message back to tweet
    char ack[]="ack";
    counter = 0;
    while(1) {
        cout << "Upon startup of server-tweet TCP connection ";
        printf("The server has TCP Port %s, and IP addresses: %s \n",PORT, ip_client);
        cout << "Sending feedback result to a Tweet :";

        //sendto(udpsockfd,dataSent,sizeof MAXDATA,0,(struct sockaddr *)&their_addr, addr_len);
        sendto(socket_new, ack, MAXDATA, 0 ,(struct sockaddr *)&stotweet, stt_addr_len);
        //get new connection from a tweet#
        close(socket_new);

        socket_sTt = accept(socket_,(struct sockaddr *)&their_addr, &_size);
        if(socket_sTt < 0) {
            printf("error on accept\n");
            return 1;
        }
        if ((revc_checker = recv(socket_sTt, s, MAXDATA, 0)) == -1) {
            perror("recv");
            return 1;
        }
        //printf("%s\n",s);
        stringstream ss_;
        string sss = (string)s;
        string size_s;
        if(sss == "A"){
            size_s = intochar(tA.size());
            send(socket_new, size_s.c_str(), MAXDATA , 0);
            feedback(tA, socket_sTt, sss);
            cout << "The server has send the feedback result to <TweetA>" << endl;
        }
        else if(sss == "B"){
            size_s = intochar(tB.size());
            send(socket_new, size_s.c_str(), MAXDATA , 0);
            feedback(tB, socket_sTt, sss);
            cout << "The server has send the feedback result to <TweetB>" << endl;
        }
        else {
            size_s = intochar(tC.size());
            send(socket_new, size_s.c_str(), MAXDATA , 0);
            feedback(tC, socket_sTt, sss);
            cout << "The server has send the feedback result to <TweetC>" << endl;
        }



        counter++;
        if(counter == 1) {
            printf("End of Phase 2 :  End of Phase 2 for the Server \n");
            break;
        }
    }


    return 0;
}