#ifndef RECEIVING_HPP
#define RECEIVING_HPP 1

int findDistanceToVia(IPAddress ip, vector<IPAddress> *addrTable, vector<routeRecord> *routeTable);
void updateTable(vector<IPAddress> *addrTable, routeRecord *record, vector<routeRecord> *routeTable);
int listenToAndUpdate(vector<IPAddress> *addrTable, int *sockfd, vector<routeRecord> *routeTable, fd_set *descriptors);

#endif