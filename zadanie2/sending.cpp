#include "utils.hpp"
#include "sending.hpp"

void senderToRouter(routeRecord *record, int *sockfd, sockaddr_in *server_address)
{
    char message[9];
    message[0] = char(stoi(record->p1));
    message[1] = char(stoi(record->p2));
    message[2] = char(stoi(record->p3));
    message[3] = char(stoi(record->p4));
    message[4] = char(stoi(record->mask));
    message[5] = char(record->distance >> 24);
    message[6] = char(record->distance >> 16);
    message[7] = char(record->distance >> 8);
    message[8] = char(record->distance >> 0);

    if (sendto(*sockfd, message, sizeof(message), 0, (sockaddr *)server_address, sizeof(*server_address)) != (int)sizeof(message))
    {
        // sendto error - assume disabled interface and set network to unreachable and distance to infinity
        record->reachable = false;
        record->distance = INF;
    }
    else
    {
        // sendto success
    }
}

void sendTable(vector<IPAddress> *addrTable, vector<routeRecord> *routeTable, int *sockfd, sockaddr_in *server_address)
{
    for (IPAddress ip : (*addrTable))
    {
        string broadcast = ip.brd1 + "." + ip.brd2 + "." + ip.brd3 + "." + ip.brd4;
        const char *broadcastIP = broadcast.c_str();
        inet_pton(AF_INET, broadcastIP, &(*server_address).sin_addr);

        for (routeRecord record : (*routeTable))
        {
            senderToRouter(&record, sockfd, server_address);
        }
    }
}