#define _WIN32_WINNT 0x0A00
#include <winsock2.h> //Winsock API
#include <ws2tcpip.h> // TCP/IP
#include <iostream>
#include <string>
#include <chrono>
#include <unordered_map>
#include <mutex>
#include <atomic>
#include <thread>
#include <sstream>
#include <algorithm>
#include <random>

#pragma comment(lib, "ws2_32.lib") //library

const int PORT = 8081;
const int BUFFER_SIZE = 1024;
const int WAIT_TIME_SECONDS = 10;

std::mutex secretMutex;
std::atomic<int> secretNumber;
std::mutex clientsMutex;
std::unordered_map<std::string, std::chrono::steady_clock::time_point> lastAttemptTime;
std::mt19937 rng;


//find matching digit in client number
bool hasMatchingDigits(int secret, int guess) {
    std::string secretStr = secret < 10 ? "0" + std::to_string(secret) : std::to_string(secret);
    std::string guessStr = guess < 10 ? "0" + std::to_string(guess) : std::to_string(guess);

    for (char s : secretStr) {
        if (guessStr.find(s) != std::string::npos) {
            return true;
        }
    }
    return false;
}

void generateNewNumber(const std::string& clientKey) {
    std::lock_guard<std::mutex> lock(secretMutex);
    std::uniform_int_distribution<int> dist(1, 100);
    secretNumber = dist(rng); 
    std::cout << "New secret number: " << secretNumber
        << " (triggered by " << clientKey << ")" << std::endl;
}

void handleClient(SOCKET clientSocket, sockaddr_in clientInfo) {
    char clientIP[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &clientInfo.sin_addr, clientIP, INET_ADDRSTRLEN); //converts a binary IP address to a string
    std::stringstream clientId;
    clientId << clientIP << ":" << ntohs(clientInfo.sin_port);
    std::string clientKey = clientId.str(); //generating the client's key

    char buffer[BUFFER_SIZE];
    bool clientActive = true;

    std::cout << "Client connected: " << clientKey << std::endl;

    while (clientActive) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);

        if (bytesReceived <= 0) {
            if (bytesReceived == SOCKET_ERROR)
                std::cerr << "recv failed from " << clientKey << ": " << WSAGetLastError() << std::endl;
            break;
        }

        std::string input(buffer, bytesReceived);

        // Processing the exit command
        if (input == "exit") {
            clientActive = false;
            std::string msg = "Connection closed. Goodbye!\n";
            send(clientSocket, msg.c_str(), msg.size(), 0);
            continue;
        }

        try {
            int guessedNumber = std::stoi(input); //Converts a string to a number
            int currentSecret;
            bool isCorrect;

            // Blocking for reading a  number
            {
                std::lock_guard<std::mutex> lock(secretMutex);
                currentSecret = secretNumber;
                isCorrect = (guessedNumber == currentSecret);
            }

            // Checking the time limit
            auto now = std::chrono::steady_clock::now();
            {
                std::lock_guard<std::mutex> lock(clientsMutex);
                if (lastAttemptTime.count(clientKey)) {
                    auto duration = std::chrono::duration_cast<std::chrono::seconds>(
                        now - lastAttemptTime[clientKey]);
                    if (duration.count() < WAIT_TIME_SECONDS) {
                        std::string waitMsg = "Wait " +
                            std::to_string(WAIT_TIME_SECONDS - duration.count()) +
                            " seconds before next attempt.\n";
                        send(clientSocket, waitMsg.c_str(), waitMsg.size(), 0);
                        continue;
                    }
                }
                lastAttemptTime[clientKey] = now;
            }

            // Forming a response
            std::string response;
            if (isCorrect) {
                generateNewNumber(clientKey);
                response = "Correct! New number generated. Keep guessing!\n";
            }
            else {
                response = (guessedNumber < currentSecret) ?
                    "Too low! " : "Too high! ";

                if (hasMatchingDigits(currentSecret, guessedNumber)) {
                    response += "One digit matches!\n";
                }
                else {
                    response += "No matching digits.\n";
                }
            }
            //Sending a response
            if (send(clientSocket, response.c_str(), response.size(), 0) == SOCKET_ERROR) {
                std::cerr << "send failed to " << clientKey << ": " << WSAGetLastError() << std::endl;
                break;
            }

        }
        catch (const std::exception& e) {
            std::string errorMsg = "Invalid input: " + std::string(e.what()) + "\n";
            send(clientSocket, errorMsg.c_str(), errorMsg.size(), 0);
        }
    }

    {
        std::lock_guard<std::mutex> lock(clientsMutex);
        lastAttemptTime.erase(clientKey);
    }
    closesocket(clientSocket);
    std::cout << "Client disconnected: " << clientKey << std::endl;
}
int main() {
    //Initializing Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed\n";
        return 1;
    }

    // Initializing GN
    std::random_device rd;
    rng.seed(rd());
    generateNewNumber("SERVER INIT");

    //Creating a listening socket
    SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    // Configuring Port Reuse
    BOOL optVal = TRUE;
    if (setsockopt(listenSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&optVal, sizeof(optVal)) == SOCKET_ERROR) {
        std::cerr << "setsockopt failed: " << WSAGetLastError() << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    //Binding a socket to an address and port
    sockaddr_in service;
    service.sin_family = AF_INET;
    service.sin_addr.s_addr = INADDR_ANY;
    service.sin_port = htons(PORT);

    if (bind(listenSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR) {
        std::cerr << "bind failed: " << WSAGetLastError() << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "listen failed: " << WSAGetLastError() << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Server started on port " << PORT << std::endl;
    
    //The main connection acceptance cycle
    while (true) {
        sockaddr_in clientInfo;
        int clientSize = sizeof(clientInfo);
        SOCKET clientSocket = accept(listenSocket, (SOCKADDR*)&clientInfo, &clientSize);

        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "accept failed: " << WSAGetLastError() << std::endl;
            continue;
        }

        std::thread(handleClient, clientSocket, clientInfo).detach();
    }

    closesocket(listenSocket);
    WSACleanup();
    return 0;
}
