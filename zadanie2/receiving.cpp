#include "utils.hpp"
#include "sending.hpp"

int findDistanceToVia(IPAddress ip, vector<IPAddress> *addrTable, vector<routeRecord> *routeTable)
{
    ip = findMask(ip, addrTable);

    for (routeRecord curr : (*routeTable))
    {
        if (ip.p1 == curr.p1 && ip.p2 == curr.p2 && ip.p3 == curr.p3 && ip.p4 == curr.p4)
        {
            return curr.distance;
        }
    }
    return INF;
}

void updateTable(vector<IPAddress> *addrTable, routeRecord *record, vector<routeRecord> *routeTable)
{
    bool found = false;
    IPAddress aux = {record->via1, record->via2, record->via3, record->via4, "32", "0", "0", "0", "0"};
    int realDistance = record->distance + findDistanceToVia(aux, addrTable, routeTable);

    (*record).distance = realDistance;
    for (vector<routeRecord>::iterator curr = (*routeTable).begin(); curr != (*routeTable).end(); curr++)
    {
        if (record->p1 == curr->p1 && record->p2 == curr->p2 && record->p3 == curr->p3 && record->p4 == curr->p4)
        {
            found = true;
            if (record->distance < curr->distance)
            {
                (*routeTable).erase(curr);
                (*routeTable).push_back(*record);
            }
        }
    }
    if (!found)
    {
        (*routeTable).push_back(*record);
    }
}

int listenToAndUpdate(vector<IPAddress> *addrTable, int *sockfd, vector<routeRecord> *routeTable, fd_set *descriptors)
{
    timeval tv;
    tv.tv_sec = ROUND_TIME / 1000 + 2;
    tv.tv_usec = 0;
    int ready = select(*sockfd + 1, descriptors, NULL, NULL, &tv);

    if (ready == -1)
    {
        fprintf(stderr, "select error: %s\n", strerror(errno));
        return EXIT_FAIL;
    }
    if (ready == 0)
    {
        return EXIT_SUCCESS;
    }

    sockaddr_in sender;
    socklen_t sender_len = sizeof(sender);
    u_int8_t buffer[IP_MAXPACKET + 1];

    ssize_t datagram_len = recvfrom(*sockfd, buffer, IP_MAXPACKET, 0, (sockaddr *)&sender, &sender_len);
    if (datagram_len < 0)
    {
        fprintf(stderr, "recvfrom error: %s\n", strerror(errno));
        return EXIT_FAIL;
    }

    char sender_ip_str[20];
    inet_ntop(AF_INET, &(sender.sin_addr), sender_ip_str, sizeof(sender_ip_str));
    // printf("Received UDP packet from IP address: %s, port: %d\n", sender_ip_str, ntohs(sender.sin_port));

    buffer[datagram_len] = 0;
    // printf("%ld-byte message: +%s+\n", datagram_len, buffer);

    string via1 = to_string(int(sender.sin_addr.s_addr) & 255);
    string via2 = to_string(int(sender.sin_addr.s_addr >> 8) & 255);
    string via3 = to_string(int(sender.sin_addr.s_addr >> 16) & 255);
    string via4 = to_string(int(sender.sin_addr.s_addr >> 24) & 255);
    int dist = (int(buffer[5]) << 24) + (int(buffer[6]) << 16) + (int(buffer[7]) << 8) + (int(buffer[8]) << 0);

    // cout << "MESSAGE FROM: " << via1 + "." + via2 + "." + via3 + "." + via4 << "\n";
    // cout << "MESSAGE: " << to_string(int(buffer[0])) << "." << to_string(int(buffer[1])) << "." << to_string(int(buffer[2])) << "."
    //      << to_string(int(buffer[3])) << "/" << to_string(int(buffer[4])) << " distance " << dist << "\n";

    routeRecord record = {to_string(int(buffer[0])), to_string(int(buffer[1])), to_string(int(buffer[2])), to_string(int(buffer[3])),
                          to_string(int(buffer[4])), dist, via1, via2, via3, via4, to_string(int(buffer[4])), true, 0};

    updateTable(addrTable, &record, routeTable);
    return EXIT_SUCCESS;
}