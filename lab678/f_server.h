#pragma once

#include <string>
//#include <zconf.h>
#include <zmq.hpp>
#include <unistd.h>

#ifndef PORT 
#define PORT 1234
#endif

#ifndef CLIENTNAME
#define CLIENTNAME "./client"
#endif

int send_message(zmq::socket_t&,const std::string&);
std::string recieve_message(zmq::socket_t&);
std::string get_port_name(int);
int bind_socket(zmq::socket_t&);
void create_node(int,int);