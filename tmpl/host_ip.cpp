/**
@file: host_ip.cpp
@from: https://www.includehelp.com/c-programs/get-ip-address-in-linux.aspx
C program to get IP Address of Linux Computer System.
clang++ -std=c++11 host_ip.cpp -o tmpl.out && tmpl.out
    g++ -std=c++14 host_ip.cpp -o tmpl.out && tmpl.out
*/

#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>

int main()
{
    unsigned char ip_address[15];
    int fd;
    struct ifreq ifr;

    /*AF_INET - to define network interface IPv4*/
    /*Creating soket for it.*/
    fd = socket(AF_INET, SOCK_DGRAM, 0);

    /*AF_INET - to define IPv4 Address type.*/
    ifr.ifr_addr.sa_family = AF_INET;

    /*eth0 - define the ifr_name - port name
    where network attached.*/
    memcpy(ifr.ifr_name, "eth0", IFNAMSIZ-1);

    /*Accessing network interface information by
    passing address using ioctl.*/
    ioctl(fd, SIOCGIFADDR, &ifr);
    /*closing fd*/
    close(fd);

    /*Extract IP Address*/
    strcpy((char *)ip_address, inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));

    printf("System IP Address is: %s\n",ip_address);

    return 0;
}
