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
    std::cout << "** == CLIENT APPLICATION == **\n\n";

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
    client_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (client_socket == INVALID_SOCKET) {
        cout << "Error at socket(): " << WSAGetLastError() << endl;
        WSACleanup();
        return 0;
    }
    else {
        cout << "socket() is Okay! " << endl;
    }

    std::cout << "== STEP 3: Connect with server ==\n\n";
    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_address.sin_port = htons(port);

    char buffer[200];
    printf("Enter your request: ");
    cin.getline(buffer, 200);

    int byte_sent = sendto(client_socket, (const char*)buffer, strlen(buffer), 0, (struct sockaddr*)&server_address, sizeof(server_address));

    if (byte_sent == -1) {
        printf("Error transmitting data.\n");
        WSACleanup();
        return 0;
    }
    else {
        printf("Request sent: %s\n", buffer);
    }

    Person receive_data;
    sockaddr_in server_address2;
    int server_address_length = (int)sizeof(server_address2);
    int  byte_received = recvfrom(client_socket, (char *)&receive_data, sizeof(receive_data), 0, (struct sockaddr*)&server_address2, &server_address_length);
    if (byte_received < 0) {
        printf("Cound not receive datagram from server.\n");
        WSACleanup();
        return 0;
    }
    else {
        printf("Server sent following data -> [Name: %s, Age: %d, Gender: %c].\n", receive_data.name.c_str(), receive_data.age, receive_data.gender);
    }

    system("pause");
    WSACleanup();

    return 0;
}
