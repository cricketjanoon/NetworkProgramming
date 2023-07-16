// NetworkServer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <vector>
#include <WinSock2.h>
#include <WS2tcpip.h>

using namespace std;

class Person {
public:
    string name;
    int age;
    char gender;
};

int main() {

    std::cout << "** == SERVER APPLICATION == **\n\n";

    std::cout << "== STEP 1: Set up DLL ==\n\n";

    SOCKET server_socket, accept_socket;
    int port = 55555;
    WSADATA wsa_data;
    int wsa_err;

    WORD w_version_required = MAKEWORD(2, 2);
    wsa_err = WSAStartup(w_version_required, &wsa_data);
    if (wsa_err != 0) {
        cout << "The winsock dll not found." << endl;
        return 0;
    }
    else {
        cout << "The winsock dll found." << endl;
        cout << "STATUS: " << wsa_data.szSystemStatus << endl;
    }

    std::cout << "\n\n== STEP 2: Set up Server Socket ==\n\n";

    server_socket = INVALID_SOCKET;
    server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_socket == INVALID_SOCKET) {
        cout << "Error at socket: " << WSAGetLastError() << endl;
        WSACleanup();
        return 0;
    }
    else {
        cout << "Scoket is Okay!" << endl;
    }

    std::cout << "\n\n== STEP 3: Bind socket ==\n\n";

    sockaddr_in service;
    service.sin_family = AF_INET;

    service.sin_addr.s_addr = inet_addr("127.0.0.1");
    //InetPton(AF_INET, _T("127.0.0.1"), &service.sin_addr.s_addr);
    service.sin_port = htons(port);

    if (bind(server_socket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR) {
        cout << "bind() failed: " << WSAGetLastError() << endl;
        closesocket(server_socket);
        WSACleanup();
        return 0;
    }
    else {
        cout << "bind() is Okay!" << endl;
    }

    std::cout << "\n\n== STEP 4: Listen Socket ==\n\n";

    if (listen(server_socket, 1) == SOCKET_ERROR) {
        cout << "listen(): Error listening at socket: " << WSAGetLastError() << endl;
    }
    else {
        cout << "listen() is Okay. I'm waiting for connections. \n";
    }

    std::cout << "\n\n== STEP 5: Accept Connection ==\n\n";

    accept_socket = accept(server_socket, nullptr, nullptr);
    if (accept_socket == INVALID_SOCKET) {
        cout << "accept failed: " << WSAGetLastError() << endl;
        WSACleanup();
        return -1;
    }

    cout << "Accepted connection. " << endl;
   
    std::cout << "\n\n== STEP 6: Receive Data ==\n\n";

    std::vector<Person> database = { {"john", 27, 'm'}, {"jimmy", 24, 'm'}, {"emily", 19, 'f'}};

    char receive_buffer[200];
    int  byte_received = recv(accept_socket, receive_buffer, 200, 0);
    if (byte_received < 0) {
        printf("Clinet error: %ld.\n", WSAGetLastError());
        return 0;
    }
    else {
        printf("Received data: %s \n", receive_buffer);

        Person data_to_send;
        bool found = false;
        for (int i = 0; i < database.size(); i++) {
            if (database[i].name == receive_buffer) {
                data_to_send = database[i];
                found = true;
                break;
            }
        }

        int byte_sent = send(accept_socket, (char*)&data_to_send, sizeof(data_to_send), 0);
        if (byte_sent == SOCKET_ERROR) {
            printf("Client send error: %d", WSAGetLastError());
            return -1;
        }
        else {
            printf("Sent %ld bytes.\n", byte_sent);
        }

    }
    
    system("pause");

    WSACleanup();

    return 0;
}
