// NetworkClient.cpp : This file contains the 'main' function. Program execution begins and ends there.

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>

using namespace std;

class Person {
public:
    string name;
    int age;
    char gender;
};

int main()
{
    std::cout << "** == CLIENT APPLICATION == **\n";

    std::cout << "== STEP 1: Set up DLL ==\n\n";

    SOCKET client_socket;
    int port = 55555;
    WSADATA wsa_data;
    int wsa_err;
    WORD w_version_requested = MAKEWORD(2, 2);
    wsa_err = WSAStartup(w_version_requested, &wsa_data);
    if (wsa_err != 0) {
        cout << "The winsok dll not found." << endl;
        return 0;
    }
    else {
        cout << "The winsock dll found." << endl;
        cout << "The status: " << wsa_data.szSystemStatus << endl;
    }

    std::cout << "== STEP 2: Set up Client Socket ==\n\n";

    client_socket = INVALID_SOCKET;
    client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (client_socket == INVALID_SOCKET) {
        cout << "Error at socket(): " << WSAGetLastError() << endl;
        WSACleanup();
        return 0;
    }
    else {
        cout << "socket() is Okay! " << endl;
    }

    std::cout << "== STEP 3: Connect with server ==\n\n";
    sockaddr_in client_service;
    client_service.sin_family = AF_INET;
    client_service.sin_addr.s_addr = inet_addr("127.0.0.1");
    client_service.sin_port = htons(port);
    if (connect(client_socket, (SOCKADDR*)&client_service, sizeof(client_service)) == SOCKET_ERROR) {
        cout << "Client: connect() - Failed to connect" << endl;
        WSACleanup();
        return 0;
    }
    else {
        cout << "Client: connect() is okay. " << endl;
        cout << "Can start sending and receiving data ... " << endl;
    }

    std::cout << "\n\n== STEP 4: Sending Data ==\n\n";

    char buffer[200];
    printf("Enter your message: ");
    cin.getline(buffer, 200);

    int byte_sent = send(client_socket, buffer, 200, 0);

    if(byte_sent==SOCKET_ERROR){
        printf("Server send error: %d", WSAGetLastError());
        return -1;
    }
    else {
        printf("Server: sent %ld bytes.\n", byte_sent);
    }
    
    Person receive_data;
    int byte_received = recv(client_socket, (char *)&receive_data, sizeof(receive_data), 0);
    if(byte_received<0){
        printf("Server error: %ld.\n", WSAGetLastError());
        return 0;
    }
    else {
        printf("Server sent following data -> Name: %s, Age: %d, Gender: %c.\n", receive_data.name.c_str(), receive_data.age, receive_data.gender);
    }


    system("pause");
    WSACleanup();

    return 0;
}
