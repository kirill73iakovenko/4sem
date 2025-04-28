#define _WIN32_WINNT 0x0A00
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <thread>
#include <algorithm>

#pragma comment(lib, "ws2_32.lib")

const int PORT = 8081;
const int BUFFER_SIZE = 1024;

bool validateInput(const std::string& input) {
    if (input.empty()) return false;
    if (!std::all_of(input.begin(), input.end(), ::isdigit)) return false;

    try {
        int num = std::stoi(input);
        return num >= 1 && num <= 100;
    }
    catch (...) {
        return false;
    }
}

void receiverThread(SOCKET sock) {
    char buffer[BUFFER_SIZE]; //A buffer array of BUFFER_SIZE (1024 bytes) is created to store data from the server.
    while (true) {
        memset(buffer, 0, BUFFER_SIZE); //Fills a memory block with 0
        int bytesReceived = recv(sock, buffer, BUFFER_SIZE - 1, 0);
        if (bytesReceived <= 0) {
            if (bytesReceived == 0)
                std::cout << "\nConnection closed by server\n";
            else
                std::cerr << "\nrecv error: " << WSAGetLastError() << std::endl;
            break;
        }
        std::cout << "\nServer response: " << buffer << "> ";
        std::cout.flush();
    }
}

int main() {
    //Initializing Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed\n";
        return 1;
    }

    //Creates a TCP socket for IPv4
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        std::cerr << "Socket creation failed: " << WSAGetLastError() << "\n";
        WSACleanup();
        return 1;
    }

    sockaddr_in serv_addr{};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) != 1) { //converts a text address to a binary format
        std::cerr << "Invalid address\n";
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    //Connects to the server via TCP
    if (connect(sock, (sockaddr*)&serv_addr, sizeof(serv_addr))) {
        std::cerr << "Connection failed: " << WSAGetLastError() << "\n";
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    std::cout << "Connected to server. Enter numbers (1-100) or 'exit' to quit\n> ";
    std::cout.flush();

    // Starting a thread to receive messages
    std::thread(receiverThread, sock).detach();

    // The main data entry loop
    std::string input;
    while (true) {
        std::getline(std::cin, input);

        if (input == "exit") {
            std::cout << "Disconnecting...\n";
            break;
        }

        if (!validateInput(input)) {
            std::cout << "Invalid input! Please enter number 1-100\n> ";
            continue;
        }

        if (send(sock, input.c_str(), input.size(), 0) == SOCKET_ERROR) {
            std::cerr << "send error: " << WSAGetLastError() << std::endl;
            break;
        }

        std::cout << "> ";
        std::cout.flush();
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}
