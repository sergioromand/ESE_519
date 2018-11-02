/* WiFi Example
 * Copyright (c) 2016 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
//#include "EthernetInterface.h"
#include "wifiGETWrapper.h"
#include "mbed.h"
#include "TCPSocket.h"

#define WIFI_SSID "Hydroguy45"
#define WIFI_PASSWORD "segfault"
#define PORT 8081

ESP8266Interface wifi(p13, p14);
/*void startHTTPServer(ESP8266Interface * wifi){
    printf("Starting HTTP server");
    TCPServer server;
    server.open(wifi);
    server.bind(wifi->get_ip_address(),PORT);
    server.listen();
    while(true){
        SocketAddress addr;
        TCPSocket client;
        printf("\n\n\r***Waiting for a socket connection***\n\n\r");
        int err = server.accept(&client, &addr);
        if(err  < 0){
            printf("Ran into an error %d", err);
            break;    
        }
        printf("----------------\n\rConnection created with %s%d\n\r", addr.get_ip_address(), addr.get_port());
        client.set_blocking(true);
        client.set_timeout(1500);
        char buff[256];
        while (true){
            int n=client.recv(buff, sizeof(buff));
            if(n<=0) break;
            buff[n] = '\0';
            printf("Received from client: %s\n", buff);
            client.send(buff, n);
            printf("%s\n",buff);
            if( n<=0)break;
        }
        client.close(); 
        printf("Connection Terminated\n\r--------------------\n\n\r");
    }
    server.close();
    
    TCPSocketServer server;
    
    server.listen();
    while(true){
        TCPSocketConnection connection;
        server.accept(connection);
        connection.set_blocking(false, 1500);
        printf("Connection from %s\n", connection.get_address());
        char buff[256];
        while (true){
            int n=client.receive(buff, sizeof(buff));
            if(n<=0) break;
            buff[n] = '\0';
            printf("Received from client: %s\n", buff);
            client.send_all(buffer, n);
            if( n<=0)break;
        }
        client.close()
    }
    server.close();
}*/
const char *sec2str(nsapi_security_t sec)
{
    switch (sec) {
        case NSAPI_SECURITY_NONE:
            return "None";
        case NSAPI_SECURITY_WEP:
            return "WEP";
        case NSAPI_SECURITY_WPA:
            return "WPA";
        case NSAPI_SECURITY_WPA2:
            return "WPA2";
        case NSAPI_SECURITY_WPA_WPA2:
            return "WPA/WPA2";
        case NSAPI_SECURITY_UNKNOWN:
        default:
            return "Unknown";
    }
}

void scan_demo(WiFiInterface *wifi)
{
    WiFiAccessPoint *ap;

    printf("Scan:\r\n");

    int count = wifi->scan(NULL,0);

    /* Limit number of network arbitrary to 15 */
    count = count < 15 ? count : 15;

    ap = new WiFiAccessPoint[count];
    count = wifi->scan(ap, count);
    for (int i = 0; i < count; i++)
    {
        printf("Network: %s secured: %s BSSID: %hhX:%hhX:%hhX:%hhx:%hhx:%hhx RSSI: %hhd Ch: %hhd\r\n", ap[i].get_ssid(),
               sec2str(ap[i].get_security()), ap[i].get_bssid()[0], ap[i].get_bssid()[1], ap[i].get_bssid()[2],
               ap[i].get_bssid()[3], ap[i].get_bssid()[4], ap[i].get_bssid()[5], ap[i].get_rssi(), ap[i].get_channel());
    }
    printf("%d networks available.\r\n", count);

    delete[] ap;
}

void GET(NetworkInterface *net, const char * address, const char * argument)
{
    TCPSocket socket;

    printf("Sending HTTP request to %s/?result=%s\r\n", address, argument);

    // Open a socket on the network interface, and create a TCP connection to www.arm.com
    socket.open(net);
    socket.connect(address, 8081);

    // Send a simple http request
    char sbuffer[1024]; //= "GET /?result=1 HTTP/1.1\r\nHost: upenn.pennpacapp.com\r\n\r\n");
    sprintf(sbuffer, "GET /?result=%s HTTP/1.1\r\nHost: %s\r\n\r\n", argument, address);
    //strcat(sbuffer, "upenn.pennpacapp.com");
    //strcat(sbuffer, "\r\n\r\n");
    //strcat(sbuffer, address);
    int scount = socket.send(sbuffer, sizeof sbuffer);
    printf("sent %d [%.*s]\r\n", scount, strstr(sbuffer, "\r\n")-sbuffer, sbuffer);
    printf("-->%s\r\n", sbuffer);
    // Recieve a simple http response and print out the response line
    char rbuffer[1024];
    int rcount = socket.recv(rbuffer, sizeof rbuffer);
    printf("recv %d [%.*s]\r\n", rcount, strstr(rbuffer, "\r\n")-rbuffer, rbuffer);

    // Close the socket to return its memory and bring down the network interface
    socket.close();
}

int initConnection(const char * SSID, const char * password)
{
    printf("WiFi example\r\n\r\n");

    // Scan for available access points 
    scan_demo(&wifi);

    printf("\r\nConnecting...\r\n");
    int ret = wifi.connect(SSID, password, NSAPI_SECURITY_WPA_WPA2);
    if (ret != 0) {
        printf("\r\nConnection error\r\n");
        return -1;
    }

    printf("Success\r\n\r\n");
    printf("MAC: %s\r\n", wifi.get_mac_address());
    printf("IP: %s\r\n", wifi.get_ip_address());
    printf("Netmask: %s\r\n", wifi.get_netmask());
    printf("Gateway: %s\r\n", wifi.get_gateway());
    printf("RSSI: %d\r\n\r\n", wifi.get_rssi());

    //GET(&wifi, "upenn.pennpacapp.com", "1");
    //GET(&wifi, "192.168.8.1", "Testing|testing|1|2|3");
}
int sendGET(const char * args){
    GET(&wifi, "192.168.8.1", args); 
}