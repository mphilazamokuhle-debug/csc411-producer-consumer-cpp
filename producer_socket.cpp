#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <cstdlib>
#include <ctime>
#include "ITStudent.h"
#include "XMLHelper.h"

#pragma comment(lib, "ws2_32.lib")

ITStudent generateRandomStudent() {
    ITStudent s;
    s.name = "Zamo";
    s.id = "12345678";
    s.programme = "Computer Science";

    Course c1 = {"Programming", rand() % 100};
    Course c2 = {"Networking", rand() % 100};
    Course c3 = {"Databases", rand() % 100};
    s.courses = {c1, c2, c3};

    return s;
}

int main() {
    srand((unsigned)time(0));

    WSADATA wsaData;
    WSAStartup(MAKEWORD(2,2), &wsaData);

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "connection failed" <<std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }
    
    ITStudent student = generateRandomStudent();
    std::string xmlData = toXML(student);

    send(clientSocket, xmlData.c_str(), xmlData.size(), 0);
    std::cout << "Sent XML data to server." << std::endl;

    closesocket(clientSocket);
    WSACleanup();
    return 0;
}






