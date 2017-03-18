#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include "Fact.cpp"
#include "Rule.cpp"
#include "Manipulator.cpp"
#include <signal.h>

using namespace std;


void terminate_with_error (const char * error_msg,int sock)
	{
		perror("Error Binding Socket:");
		if (sock != -1) 
		{
			close (sock);
			exit(1);
		}
	}

int main(int argc, char *argv[]){

	signal(SIGPIPE, SIG_IGN); 
	
	// only one! loads files, does inferences, etc
	Manipulator M;
	// our program info, how to call functionalities
			
			string welcome = "\n-----------Welcome to the SIMPLE RULE-BASE INFERENCE ENGINE!------------\n\n"
					"Let's get started.\n" 
					"Below is a list of user commands & what they do.\n\n"
					"	LOAD - type the file, fact or rule you'd like to load\n"
					"	FACT - define a fact to add to the Knowlegde Base (KB)\n"
					"	RULE - define a rule to add to the Rule Base (RB) \n"
					"	INFERENCE - issue a query\n"
					"	DROP - used to delete a fact from the KB and RB\n"
					"	DUMP - prints the contents of KB and RB to an .sri file\n"
					"	EXIT - exit the SRI\n";
			//cout << welcome <<endl;
	char welcomeArray[welcome.size()+1];
	strcpy(welcomeArray, welcome.c_str());
	//int main (int argc,char ** argv){
		// set up socket connection
		int sock;
		struct sockaddr_in serverAddr;
		struct sockaddr_in clientAddr;
		socklen_t sin_size = sizeof(struct sockaddr_in);

		while(1){
		if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1){
			cout << "1" << endl;
			terminate_with_error("Error Creating Socket",sock);
		}
		int sock_opt = 1;
		setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,(void *)&sock_opt,sizeof (sock_opt));
			serverAddr.sin_family = AF_INET;
			serverAddr.sin_port = htons(9999);
			serverAddr.sin_addr.s_addr = INADDR_ANY;
			bzero(&(serverAddr.sin_zero), 8);
		if (bind(sock, (struct sockaddr *)&serverAddr, sizeof(struct sockaddr)) == -1){
			cout << "11" << endl;
			terminate_with_error("Error Binding",sock);
		}

		if (listen(sock, 10) == -1){
			cout << "111" << endl;
			terminate_with_error("Error Listening: ",sock);
		}
		int newsock = accept(sock, (struct sockaddr *)&clientAddr,&sin_size);
		if ( newsock < 1 ){
			cout << "no" <<endl;
			terminate_with_error("Error Accepting Socket",0);
		}
		else{
			cout << "connected" << endl;
			cout << "welcome: " << welcome;
			//send(sock,welcome,strlen(welcome),0);
			
			while(1){
			//send(sock,welcomeArray,strlen(welcomeArray),0);
			int maxBytes = 1024;
			char buffer[maxBytes];
			memset(buffer,0,maxBytes);
			int bytes_read = recv(newsock,buffer,maxBytes,0);
			if ( bytes_read > 0 ) {
				printf ("Received Message from %s:%d\n%s\n",(char *)inet_ntoa(clientAddr.sin_addr),clientAddr.sin_port,buffer);
				string stringBuffer(buffer);
			}
			string stringBuffer(buffer);
			//bool done = false;
			bool done = false;
			M.LineIn(stringBuffer, &done);
			if(M.outputArray != ""){
				//send(sock,M.outputArray,strlen(M.outputArray),0);
			}
			//while(!done){
				//string inp;
				//cout << "Type a command: " << endl;
				//getline(cin, inp);
				
				//M.LineIn(stringBuffer, &done);
			//}
			//close(sock);
			/*
			if(argc == 2){
				string line;
				ifstream readInput;
				readInput.open(argv[1]);
				while(getline(readInput,line)){
					LineIn(&M, line, &done);
				}
			}
			done = false;
			while(!done){
				string inp;
				cout << "Type a command: " << endl;
				getline(cin, inp);
				LineIn(&M, inp, &done);
			}
			*/
		}
	}
		//close(sock);
		//break;
		}

	return 0;
}
	
