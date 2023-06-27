#ifndef MYSEND_HPP
#define MYSEND_HPP 1

icmphdr initHeader(int myId, int x);
int mySend(int *sockfd, int ttl, int myId, const char *ip);

#endif