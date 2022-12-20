//Author:Thomas Curtis
//Homework 3: Client-Server communication

//Client: The client connects to the server and sends a string to the host, i.e. "Skyflyer". The
//client output the processed data from the server.

//Server: The server connects with the client and revieves a string, i.e "Skyfler. The server then
// reverses the string and send it back to the client. "Skyflyer" ->"reylfykS"

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <iostream>
#include <string>
#include <Ws2tcpip.h>
#pragma comment(lib,"WS2_32")

using namespace std;

int buildServer(int socketNO, int portNO);
SOCKET hookupWithClient(int socketNO);
int connectToHost(int socketNO, char* serverMachineName, int serverPortNO);
string reverseString(string character);


int main(int argc, char** argv) {


        //Checks WinSock module in the MS world is all good to go
        WSADATA wdata;
        if (WSAStartup(MAKEWORD(2, 2), &wdata) != 0) {
            printf("The Winsock dll not found!\n");
            exit(1);  //bail out
        }

        //Note return value of type SOCKET in the MS world!
        SOCKET socketNO;
        if ((socketNO = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            perror("socket");
            exit(1);
        }


        const int bufferSize = 64;
        char buffer[bufferSize] = "";

        //Building the Host Server
        if (atoi(argv[1]) == 1) {

            buildServer(socketNO,0); // builds server --> socket and bind
            listen(socketNO, 123); //listen primative
            int thisClientsEndPoint = hookupWithClient(socketNO);

            recv(thisClientsEndPoint, buffer, bufferSize, 0); //packet from client
            cout << "The server recives " << buffer << "from client " << endl;
            
            const char* data = buffer;
            string message = buffer;
            message = reverseString(message); //server processing the data from the client
            char secondBuffer[bufferSize] = "";
            for (int i = 0; i < message.length(); i++) {
                secondBuffer[i] = message[i];
            }
            
            send(thisClientsEndPoint,secondBuffer, message.length(), NULL); //sending the processes string back to the client
            cout << "Server sends " << message << " to the client." << endl;

        }

        //Building the Client
        if (atoi(argv[1]) == 0) {

            connectToHost(socketNO, argv[2], atoi(argv[3]));
            const char* dataPacket = argv[4]; 

            send(socketNO, argv[4], sizeof(dataPacket), NULL); //send the packet to the server
            cout << "Client output: " << argv[4] << endl;

            recv(socketNO, buffer, bufferSize, 0); //packet from the server with the processed data 
            cout << "Client receives " << buffer << " from server" << endl;

        }

        closesocket(socketNO);

}//Function that reverses a string
string reverseString(string character) {

    reverse(character.begin(), character.end());
    return character;
}
