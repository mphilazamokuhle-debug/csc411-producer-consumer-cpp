#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "ITStudent.h"
#include "XMLHelper.h"

#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2,2), &wsaData);
    
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(8080);

    bind(serverSocket, (sockaddr*) &serverAddr, sizeof(serverAddr));
    listen(serverSocket, SOMAXCONN);
    std:: cout << "Server listening on port 8080..." << std::endl;

    sockaddr_in clientAddr;
    int clientSize = sizeof(clientAddr);
    SOCKET clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientSize);

    char buffer[2048];
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';
        std::string xmlData(buffer);

        ITStudent student = parseXML(xmlData);

        std::cout<< "Student Name: " << student.name << std::endl;
        std::cout<< "Student ID: " << student.id << std::endl;
        std::cout<< "Student Programme: " << student.programme << std::endl;

        for (auto &c : student.courses) {
            std::cout << "Course: " << c.name << " | Mark: " << c.mark << std::endl;
        }

        double avg = student.calculateAverage();
        std::cout << "Average: " << avg << std::endl;
        std::cout << "Result: " << student.passOrFail() << std::endl;
    }

    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();
    return 0;
        
}