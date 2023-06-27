#include "utils.hpp"
#include "mysend.hpp"

icmphdr initHeader(int myId, int nr)
{
    icmphdr icmp_header;
    icmp_header.type = ICMP_ECHO;
    icmp_header.code = 0;
    icmp_header.un.echo.id = myId;
    icmp_header.un.echo.sequence = nr;
    icmp_header.checksum = 0;
    icmp_header.checksum = compute_icmp_checksum((u_int16_t *)&icmp_header, sizeof(icmp_header));
    return icmp_header;
}

int mySend(int *sockfd, int ttl, int myId, const char *ip)
{
    icmphdr icmp_header = initHeader(myId, ttl);

    sockaddr_in recipient;
    bzero(&recipient, sizeof(recipient));
    recipient.sin_family = AF_INET;

    inet_pton(AF_INET, ip, &(recipient.sin_addr));

    int setSocket = setsockopt(*sockfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(int));
    if (setSocket < 0)
    {
        fprintf(stderr, "Wystąpił błąd podczas zmiany TTL: %s\n", strerror(errno));
        return EXIT_FAIL;
    }

    ssize_t bytes_sent;
    for (int i = 0; i < 3; i++)
    {
        bytes_sent = sendto(*sockfd, &icmp_header, sizeof(icmp_header), 0, (sockaddr *)&recipient, sizeof(recipient));

        if (bytes_sent <= 0)
        {
            fprintf(stderr, "Signal not send!: %s\n", strerror(errno));
            return EXIT_FAIL;
        }
    }

    return EXIT_SUCCESS;
}