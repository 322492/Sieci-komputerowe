#include "utils.hpp"
#include "mysend.hpp"
#include "myreceive.hpp"

int main(int argc, char const *argv[])
{
    if (isInputGood(argc, argv))
    {
        fprintf(stderr, "Błędne dane wejściowe");
        return EXIT_FAIL;
    }

    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sockfd < 0)
    {
        fprintf(stderr, "socket error: %s\n", strerror(errno));
        return EXIT_FAIL;
    }

    int myId = getpid();

    for (int ttl = 1; ttl <= 30; ttl++)
    {
        int code = mySend(&sockfd, ttl, myId, argv[1]);

        if (code != EXIT_SUCCESS)
            return EXIT_FAIL;

        string res = myreceive(&sockfd, ttl, myId);
        cout << res;
    }

    close(sockfd);

    return EXIT_SUCCESS;
}