#include "utils.hpp"
#include <regex>

bool isInputGood(int &argc, const char *argv[])
{
	if (argc != 2)
		return false;

	struct sockaddr_in aux;
	return inet_pton(AF_INET, argv[1], &(aux.sin_addr)) != 1;
}

void print_as_bytes(unsigned char *buff, ssize_t length)
{
	for (ssize_t i = 0; i < length; i++, buff++)
		printf("%.2x ", *buff);
}

u_int16_t compute_icmp_checksum(const void *buff, int length)
{
	u_int32_t sum;
	const u_int16_t *ptr = (u_int16_t *)buff;
	assert(length % 2 == 0);
	for (sum = 0; length > 0; length -= 2)
		sum += *ptr++;
	sum = (sum >> 16) + (sum & 0xffff);
	return (u_int16_t)(~(sum + (sum >> 16)));
}
