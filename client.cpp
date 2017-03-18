#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <iostream>
#include <stdio.h>
#include <string.h>

using namespace std;
void terminate_with_error (int sock)
{
	close (sock);
	perror("Error Binding Socket:");
	exit(1);

}

int main (int argc,char ** argv)
{
	if ( argc != 2) 
	{
		printf ("Usage: ./client <server-address>\n");
		exit(1);
	}
	cout << "anything?" << endl;
	int sock;
	struct sockaddr_in serverAddr;
	socklen_t sin_size = sizeof(struct sockaddr_in);
	cout << "anything?" << endl;
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("Error Creating Socket");
		exit(1);
	}
	cout << "anything?" << endl;
	memset((char *) &serverAddr, 0,sizeof(serverAddr));
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(9999);
        struct hostent *server = gethostbyname(argv[1]);
	if ( server == NULL ){ terminate_with_error(sock);}
	memcpy((char *)&serverAddr.sin_addr.s_addr,(char *)server->h_addr, server->h_length);
        memset(&(serverAddr.sin_zero), 0, 8);
	cout << "anything?" << endl;
	if (connect(sock,(sockaddr *)&serverAddr,sizeof(serverAddr)) == -1 ) {
		cout << "o no"<<endl;
		terminate_with_error(sock);
		}
	cout << "gootta go into loop :(" << endl;
	
	
	//memset(buffer,0,maxBytes);
	
	
	//int bytes_read = recv(newsock,buffer,maxBytes,0);
	//		if ( bytes_read > 0 ) {
	//			printf ("Received Message from %s:%d\n%s\n",(char *)inet_ntoa(clientAddr.sin_addr),clientAddr.sin_port,buffer);
	
	
		//cout << "buffer: " <<  buffer << endl;
		
		
	while(1){
	
	//int bytes_read = recv(sock,buffer,maxBytes,0);
		//if ( bytes_read > 0 ) {
		//	printf ("Received Message from %s:%d\n%s\n",(char //*)inet_ntoa(serverAddr.sin_addr),serverAddr.sin_port,buffer);
		//} else { cout << "buffer empty?" << endl;}
	
	//cout << buffer << endl;
	// getting the the greeting
	//cout << "in lop :)?" << endl;
	
	//cout << "gootta go :(" << endl;
	//int bytes_read = recv (sock,buffer,maxBytes,0);
	//cout << "gotta here" << endl;
	
	char put[] = "";	// varaibles for user input
	string stringInput = "";
	put[0] = '\0';
	//getline(cin,line)
	//cin >> put;			// take user input // dont use >> operator!
	//getline(cin,put);
	getline(cin,stringInput);
	
	//string myWord = "myWord";
	char inputArray[stringInput.size()+1];//as 1 char space for null is also required
	strcpy(inputArray, stringInput.c_str());
	//cout << "got input" << endl;
	//char newline[] = "";
	//char * newArray = new char[strlen(put)+strlen(newline)+1];
   // strcpy(newArray,put);
   // strcat(newArray,newline);	
	//send (sock,newArray,strlen(newArray),0);
	//if (strstr(newArray, "sleep")) {close(sock); break;}
	if (inputArray[0] != '\0'){
		cout << "gonna try to send" << endl;
		send (sock,inputArray,strlen(inputArray),0); 
		cout << inputArray << "SENT" << endl; 
	}
	//memset(put, 0, sizeof put);
	memset(inputArray, 0, sizeof put);
	/*
	int maxBytes = 1024;
	char buffer[maxBytes];
	int bytes_read = recv(sock,buffer,maxBytes,0);
			if ( bytes_read > 0 ) {
				printf ("Received Message from %s:%d\n%s\n",(char *)inet_ntoa(serverAddr.sin_addr),serverAddr.sin_port,buffer);
			}
	*/
	//delete [] newArray;
	//put = "";
	//delete [] put;
	//send (sock,"Hello",strlen("Hello"),0);
	//close(sock);
	}
}

