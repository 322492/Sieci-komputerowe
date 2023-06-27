#ifndef UTILS_HPP
#define UTILS_HPP 1

#include <bits/stdc++.h>

#include <netinet/ip.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <netinet/ip_icmp.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

using namespace std;

#define EXIT_SUCCESS 0
#define EXIT_FAIL -1

bool isInputGood(int &argc, const char *argv[]);

void print_as_bytes(unsigned char *buff, ssize_t length);
u_int16_t compute_icmp_checksum(const void *buff, int length);

#endif
