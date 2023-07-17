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

    SOCKET server_socket;;
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
    server_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (server_socket == INVALID_SOCKET) {
        cout << "Error at socket: " << WSAGetLastError() << endl;
        WSACleanup();
        return 0;
    }
    else {
        cout << "Scoket is Okay!" << endl;
    }

    std::cout << "\n\n== STEP 3: Bind socket ==\n\n";

    sockaddr_in server_addrr;
    server_addrr.sin_family = AF_INET;
    server_addrr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addrr.sin_port = htons(port);

    if (bind(server_socket, (SOCKADDR*)&server_addrr, sizeof(server_addrr)) == SOCKET_ERROR) {
        cout << "bind() failed: " << WSAGetLastError() << endl;
        closesocket(server_socket);
        WSACleanup();
        return 0;
    }
    else {
        cout << "bind() is Okay!" << endl;
    }

    std::cout << "\n\n== STEP 4: Receive Data ==\n\n";

    std::vector<Person> database = { {"john", 27, 'm'}, {"jimmy", 24, 'm'}, {"emily", 19, 'f'} };

    char receive_buffer[200];
    sockaddr_in client_address;
    int client_address_length = (int)sizeof(client_address);
    int  byte_received = recvfrom(server_socket, receive_buffer, strlen(receive_buffer), 0, (struct sockaddr *)&client_address, &client_address_length);
    if (byte_received < 0) {
        printf("Cound not receive datagram.\n");
        WSACleanup();
        return 0;
    }
    else {
        receive_buffer[byte_received] = 0; // adding ednline char after the lsat byte recieved
        printf("Received query: %s \n", receive_buffer);

        // Processing query
        Person data_to_send;
        bool found = false;
        for (int i = 0; i < database.size(); i++) {
            if (database[i].name == receive_buffer) {
                data_to_send = database[i];
                found = true;
                break;
            }
        }

        // Sending response back
        int byte_sent = sendto(server_socket, (const char*)&data_to_send, sizeof(data_to_send), 0, (struct sockaddr*)&client_address, client_address_length);
        if (byte_sent == -1) {
            printf("Error sending response.\n");
            WSACleanup();
            return 0;
        }
        else {
            printf("Requested data sent.\n");
        }
    }

    system("pause");

    WSACleanup();

    return 0;
}
