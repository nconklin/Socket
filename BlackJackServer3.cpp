/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <map>
#include <iostream>
#include <time.h>
#include<vector>

using namespace std;

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno, client_sum;//Added var for client sum.
     socklen_t clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     client_sum = 0;//Forgot to set to zero.

     srand(time(NULL));

     std::vector<string> card_vec;
     std::map<string,int> card_map;
     std::map<string,int>::iterator it;   
   
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

     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     
     if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
     
     listen(sockfd,5);
     
     clilen = sizeof(cli_addr);
     newsockfd = accept(sockfd, 
                 (struct sockaddr *) &cli_addr, 
                 &clilen);
     
     if (newsockfd < 0) 
          error("ERROR on accept");
     bzero(buffer,256);
     
     ////////////////////////////////////////////////////////////
     while(1) {
  
     //My stuff to sum up and conditions for win or lose.
     n = read(newsockfd,buffer,255);//needed to add read here
     if (n < 0)
     {
         error("ERROR reading from socket");
     }
     
     client_sum += atoi(buffer);//continually sum up client from buffer.
     
     if(client_sum > 21)//If client goes over 21 they bust.
     {
         cout << "The casino wins\n";
         break;//stop the
     }
     else
     {
         cout << "The player sum is: " << client_sum << "\n";//Just show sum.
     }
     bzero(buffer,256);
     
     char *temp;//save
     int randIndex = rand()%card_map.size();//save
     it = card_map.find(card_vec.at(randIndex));//save
     cout<<it->first.c_str();//? just to print out selected?  
     temp = (char *)it->first.c_str();//? selected card?
     
     card_vec.erase(card_vec.begin()+randIndex);//save
     card_map.erase (it);//save
 
     n = write(newsockfd,temp,sizeof(temp));//save
     
     if (n < 0) error("ERROR writing to socket");//save
      }//end while
     ////////////////////////////////////////////////////////////
     //delete []temp;
     close(newsockfd);
     close(sockfd);
     
     return 0; 
}
