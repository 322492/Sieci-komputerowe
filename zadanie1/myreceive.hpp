#ifndef MYRECEIVER_HPP
#define MYRECEIVER_HPP 1

int getIdFromHeader(icmphdr *icmp_header);
int getSeqFromHeader(icmphdr *icmp_header);

string myreceive(int *sockfd, int ttl, int myId);

#endif