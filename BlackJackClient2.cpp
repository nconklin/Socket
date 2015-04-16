#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <iostream>
#include <map>
#include <vector>

using namespace std;

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n, server_sum;//Added var for server sum.
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    server_sum = 0;
    
    // srand(time(NULL));

     std::vector<string> card_vec;
     std::map<string,int> card_map;
     //std::map<string,int>::iterator it;   
   
     card_vec.push_back("2");
     card_vec.push_back("3");
     card_vec.push_back("4");   
     card_vec.push_back("5");
     card_vec.push_back("6");
     card_vec.push_back("7");
     card_vec.push_back("8");
     card_vec.push_back("9");
     card_vec.push_back("10");
     card_vec.push_back("J");
     card_vec.push_back("K");
     card_vec.push_back("Q");
     card_vec.push_back("A");


     card_map["2"] = 2;
     card_map["3"] = 3;
     card_map["4"] = 4;
     card_map["5"] = 5;
     card_map["6"] = 6;
     card_map["7"] = 7;
     card_map["8"] = 8;
     card_map["9"] = 9;
     card_map["10"] =10;
     card_map["J"] = 10;
     card_map["K"] = 10;
     card_map["Q"] = 10;
     card_map["A"] = 1; 

    // it = card_map.find("2");
     
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    
    serv_addr.sin_family = AF_INET;
    
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    
    serv_addr.sin_port = htons(portno);
    
   if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");

    while(1){
    //printf("Please enter the message: ");
    //cout<<"Enter the message:";
    bzero(buffer,256);//save
    //fgets(buffer,255,stdin);
    //cin.getline(buffer,256);//? for console input
    
    //My stuff. Pretty much the same for server here.
    srand(time(NULL));//?
    int randIndex = rand()%card_map.size();//save
    std::map<string,int>::iterator it;//?  
    it = card_map.find(card_vec.at(randIndex));//save

    char *temp;
    
    n = read(sockfd,buffer,255);
    if (n < 0) 
         error("ERROR reading from socket");
    //printf("%s\n",buffer);
     cout<<buffer<<"\n";//? Just to show?
     
    server_sum += atoi(buffer);//continually sum up server from buffer.
     
     if(server_sum > 21)//If server goes over 21 they bust.
     {
         cout << "The player wins\n";
         break;//stop the loop of game
     }
     else
     {
         cout << "The casino sum is: " << server_sum << "\n";//Just show sum.
     }
     cout<<it->first.c_str();//? just to print out selected?  
     temp = (char *)it->first.c_str();//? selected card?
     
     card_vec.erase(card_vec.begin()+randIndex);//save
     card_map.erase (it);
     
    n = write(sockfd,temp,strlen(temp));
    if (n < 0) 
         error("ERROR writing to socket");
    
    //bzero(buffer,256);//? Why do this again.
    
    }//end while
    
    close(sockfd);
    return 0;
}
