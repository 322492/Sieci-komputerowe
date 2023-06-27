#include "utils.hpp"
#include "sending.hpp"
#include "receiving.hpp"

void readInput(vector<IPAddress> *myIPs, vector<routeRecord> *routeTable)
{
    int neighboursNumber = 0;
    cin >> neighboursNumber;
    for (int i = 0; i < neighboursNumber; i++)
    {
        string ip, d, dist;
        cin >> ip >> d >> dist;
       // cout << "IP = " << ip << "\n";
        string p1, p2, p3, p4, mask;
        p1 = ip.substr(0, ip.find('.'));
        ip.erase(0, ip.find('.') + 1);

        p2 = ip.substr(0, ip.find('.'));
        ip.erase(0, ip.find('.') + 1);

        p3 = ip.substr(0, ip.find('.'));
        ip.erase(0, ip.find('.') + 1);

        p4 = ip.substr(0, ip.find('/'));
        ip.erase(0, ip.find('/') + 1);

        mask = ip;
        IPAddress ipaddr = {p1, p2, p3, p4, mask, "0", "0", "0", "0"};
        ipaddr = findBroadcast(ipaddr);
        myIPs->push_back(ipaddr);
        routeRecord rr = masked(ipaddr, stoi(dist));
        routeTable->push_back(rr);
    }
    return;
}

int initSocket(sockaddr_in *server_address)
{
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        fprintf(stderr, "socket error: %s\n", strerror(errno));
        return EXIT_FAIL;
    }

    bzero(server_address, sizeof(*server_address));
    server_address->sin_family = AF_INET;
    server_address->sin_port = htons(PORT);
    server_address->sin_addr.s_addr = htonl(INADDR_ANY);

    int broadcastPermission = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, (void *)&broadcastPermission, sizeof(broadcastPermission));
    if (bind(sockfd, (sockaddr *)server_address, sizeof(*server_address)) < 0)
    {
        fprintf(stderr, "bind error: %s\n", strerror(errno));
        return EXIT_FAIL;
    }
    return sockfd;
}

int main()
{
    vector<IPAddress> addrTable;
    vector<routeRecord> routeTable;
    readInput(&addrTable, &routeTable);
    sockaddr_in server_address;
    int sockfd = initSocket(&server_address);
    cout << sockfd;
    if (sockfd == EXIT_FAIL)
    {
        cout << "initSocket failed :(";
        return EXIT_FAIL;
    }

    fd_set descriptors;
    FD_ZERO(&descriptors);
    FD_SET(sockfd, &descriptors);

    int round = 0;
    for (;; round++)
    {
        //cout << "ROUND: " << round << "\n";
        sendTable(&addrTable, &routeTable, &sockfd, &server_address);
        auto startTime = chrono::steady_clock::now();
        while (getTime(&startTime) < ROUND_TIME)
        {
            int success = listenToAndUpdate(&addrTable, &sockfd, &routeTable, &descriptors);
            if (success == EXIT_FAIL)
            {
                cout << "listenToAndUpdate failed :(";
                return EXIT_FAIL;
            }
        }
        printTable(&routeTable);
    }

    return EXIT_SUCCESS;
}
