#ifndef SENDING_HPP
#define SENDING_HPP 1

void senderToRouter(routeRecord *record, int *sockfd, sockaddr_in *server_address);
void sendTable(vector<IPAddress> *addrTable, vector<routeRecord> *routeTable, int *sockfd, sockaddr_in *server_address);

#endif