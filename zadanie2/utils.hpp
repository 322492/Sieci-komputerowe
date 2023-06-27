#ifndef UTILS_HPP
#define UTILS_HPP 1

#include <bits/stdc++.h>

#include <netinet/ip.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <vector>

#define EXIT_FAIL -1
#define EXIT_SUCCESS 0
#define ROUND_TIME 6000
#define INF INT_MAX // 2^32 - 1
#define PORT 54321
using namespace std;

struct IPAddress
{
    string p1;
    string p2;
    string p3;
    string p4;
    string mask;
    string brd1;
    string brd2;
    string brd3;
    string brd4;
};

struct routeRecord
{
    string p1;
    string p2;
    string p3;
    string p4;
    string mask;
    long int distance;
    string via1;
    string via2;
    string via3;
    string via4;
    string viaMask;
    bool reachable;
    bool direct;
};

routeRecord masked(IPAddress ip, int dist);
IPAddress masked2(IPAddress ip);
IPAddress findBroadcast(IPAddress ip);
int getTime(chrono::steady_clock::time_point *startTime);
IPAddress findMask(IPAddress ip, vector<IPAddress> *addrTable);
void printTable(vector<routeRecord> *routeTable);

#endif
