#include <iostream>
#include <cstdlib> // exit()
#include <cstring> // strcpy(), strlen()
#include <unistd.h> // close()
#include <netinet/in.h>
#include <arpa/inet.h> // inet_addr()
using namespace std;
void errnet(const char * why, const int exitCode = 1)
{
    cerr << why << endl;
    exit(exitCode);
}
int main(int argc, char **argv)
{
    int mySoket, rc;
    sockaddr_in * selfAddr = new(sockaddr_in);
    selfAddr->sin_family = AF_INET;
    selfAddr->sin_port = 0;
    selfAddr->sin_addr.s_addr = 0;
    sockaddr_in * remoteAddr = new(sockaddr_in);
    remoteAddr->sin_family = AF_INET;
    remoteAddr->sin_port = htons(7);
    remoteAddr->sin_addr.s_addr = inet_addr("127.0.0.1");
    char *buf = new char[100];
    strcpy(buf,"Hello im Sergei Maslov\n");
    mySoket=socket(AF_INET, SOCK_DGRAM,0);
    if (mySoket == -1) {
        errnet("Error open socket",11);
    }
    rc=bind(mySoket, (const sockaddr*) selfAddr, sizeof(sockaddr_in));
    if (rc == -1) {
        close(mySoket);
        errnet("Error bind socket local address", 12);
    }
    rc = connect(mySoket, (const sockaddr*) remoteAddr, sizeof(sockaddr_in));
    if (rc == -1) {
        close(mySoket);
        errnet("Error bind socket remote server", 13);
    }
    rc = send(mySoket, buf, 100, 0);
    if (rc == -1) {
    	close(mySoket);
    	errnet("Error send answer", 14);
    }
    rc = recv(mySoket, buf, 100, 0);
    if (rc == -1) {
        close(mySoket);
        errnet("Error receive answer", 15);
    }
    buf[rc]='\0';
    cout << "We receive: " << buf << endl;
    close(mySoket);
    delete selfAddr;
    delete remoteAddr;
    delete[] buf;
    return 0;
}
