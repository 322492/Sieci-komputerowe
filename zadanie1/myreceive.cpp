#include "utils.hpp"
#include "myreceive.hpp"

int getIdFromHeader(icmphdr *icmp_header)
{
    if (icmp_header->type == 0)
        return icmp_header->un.echo.id;

    else if (icmp_header->type == 11 && icmp_header->code == 0)
    {
        u_int8_t *point = (u_int8_t *)(icmp_header) + 32;
        return 256 * (int)*(point + 1) + (int)*(point);
    }
    else
        return -1;
}

int getSeqFromHeader(icmphdr *icmp_header)
{
    if (icmp_header->type == 0)
        return icmp_header->un.echo.sequence;

    else if (icmp_header->type == 11 && icmp_header->code == 0)
    {
        u_int8_t *point = (u_int8_t *)(icmp_header) + 32;
        return 256 * (int)*(point + 3) + (int)*(point + 2);
    }
    else
        return -1;
}

string myreceive(int *sockfd, int ttl, int myId)
{
    sockaddr_in sender;
    socklen_t sender_len = sizeof(sender);
    u_int8_t buffer[IP_MAXPACKET];

    fd_set descriptors;
    FD_ZERO(&descriptors);
    FD_SET(*sockfd, &descriptors);

    timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    int itime = 0;

    int ready = 2;
    ssize_t packet_len = 0;
    int packetAmount = 0;

    // otrzymane adresy ip
    string ips[3];
    ips[0] = "";
    ips[1] = "";
    ips[2] = "";

    while (ready > 0 && packetAmount < 3)
    {
        ready = select(*sockfd + 1, &descriptors, NULL, NULL, &tv);
        if (ready == -1)
        {
            fprintf(stderr, "select error: %s\n", strerror(errno));
            return to_string(EXIT_FAIL);
        }
        if (ready == 1)
        {
            packet_len = recvfrom(*sockfd, buffer, MSG_DONTWAIT, 0, (sockaddr *)&sender, &sender_len);

            if (packet_len < 0)
            {
                fprintf(stderr, "recvfrom error: %s\n", strerror(errno));
                return to_string(EXIT_FAIL);
            }
            if (packet_len > 0)
            {
                struct iphdr *ip_header = (struct iphdr *)buffer;
                u_int8_t *icmp_packet = buffer + 4 * ip_header->ihl;
                struct icmphdr *icmp_header = (struct icmphdr *)icmp_packet;

                // sprawdzenie pakietu
                if (getIdFromHeader(icmp_header) == myId && getSeqFromHeader(icmp_header) == ttl)
                {
                    itime += 1000000 - tv.tv_usec;
                    char sender_ip_str[20];
                    inet_ntop(AF_INET, &(sender.sin_addr), sender_ip_str, sizeof(sender_ip_str));
                    ips[packetAmount] = sender_ip_str;
                    packetAmount++;
                }
            }
        }
    }

    float ftime = itime;

    if (ips[0] == "" && ips[1] == "" && ips[2] == "")
        return "*\n";

    string res = "";
    if (ips[0] != "")
        res += ips[0];
    if (ips[1] != "" && ips[1] != ips[0])
        res += " " + ips[1];
    if (ips[2] != "" && ips[2] != ips[0] && ips[2] != ips[1])
        res += " " + ips[2];

    if (ips[0] == "" || ips[1] == "" || ips[2] == "")
        res += " ???\n";
    else
        res += " " + to_string((int)(ftime / 3000)) + "ms\n";

    return res;
}