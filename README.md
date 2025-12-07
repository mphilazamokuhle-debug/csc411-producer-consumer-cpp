csc411-producer-consumer

Zamokuhle Mphila (202202813)

This project implements a producer-consumer system using TCP sockets on Windows. The consumer listens on a port and processes incoming XML messages from the producer. Buffer rules and XML parsing are applied via XMLHelper.

Requirements
- Windows (tested on Windows PowerShell)
- C++ compiler (Visual Studio / MSVC or MinGW g++)
- Winsock2 (included with Visual Studio; link with Ws2_32.lib)

Build
1. Navigate to the src folder:
   cd src
2. Compile consumer:
   cl consumersocket.cpp XMLHelper.cpp /EHsc /Fe:consumer.exe Ws232.lib
3. Compile producer:
   cl producersocket.cpp XMLHelper.cpp /EHsc /Fe:producer.exe Ws232.lib

Build (MinGW g++)
1. Navigate to the src folder:
   cd src
2. Compile consumer:
   g++ consumersocket.cpp XMLHelper.cpp -lws232 -o consumer.exe
3. Compile producer:
   g++ producersocket.cpp XMLHelper.cpp -lws232 -o producer.exe

Run the demo
1. Open two PowerShell windows side by side.
2. In the left window (consumer):
   cd path\to\Product-Consumer-Sockets\src
   .\consumer.exe
3. In the right window (producer):
   cd path\to\Product-Consumer-Sockets\src
   .\producer.exe
4. Expected behavior:
   - Consumer starts and listens (e.g., on port 8080 or your configured port).
   - Producer connects and sends XML payloads.
   - Consumer applies buffer rules and displays parsed results.

Configuration
- Ports, IP, and buffer sizes are set in code (see consumersocket.cpp / producersocket.cpp).
- XML parsing helpers live in XMLHelper.cpp/.h.

Demo Video
Get the demo video here: https://youtu.be/ycHRUCLUNp0?si=YnfdbUk_7xoZgTOh
