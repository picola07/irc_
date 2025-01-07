#pragma once
#include<iostream>
#include<vector>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<fcntl.h>
#include<arpa/inet.h>
#include<poll.h>
#include<csignal>

#include "client.hpp"

class client;

class server
{
    private:

        int port;
        int ssfd;
        static bool signal;
        std::vector<client> clients;
        std::vector<struct pollfd> fds;
    public:
        server();
        ~server();
        // server(){};
        void sinit();
        void ssocket();
        void accnewclient();
        void accnewdata(int fd);

        static void shandler(int fd);
        void  closefds();
        void clearclients(int fd);

};