#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <time.h> 
#include <stdlib.h>
#include <time.h>

std::map<int,int> nodes;
std::vector<std::pair<int,int>> times;

void create(int id){
    if(nodes.find(id) == nodes.end()){
        std::cout << "Error: Already exists\n";
    }
    else{
        int pid = rand() % 10000 + 2413;
        std::cout << "OK: "  << pid;
        nodes.insert(id, pid);
    }
    return;
}

void exec(int id, std::string subcmd){
    if(nodes.find(id) == nodes.end()){
        std::cout << "Error: id doesn't exist\n";
    }
    else{

    }
    return;
}
void heartbit(int id){

}

void remove(int id){
    
}


void help()
{
	std::cout << "----------------------------------" << std::endl
    << "create [id]" << std::endl
	<< "remove [id]" << std::endl
	<< "exec [id] [cmd - start/stop/time]" << std::endl
    << "heartbeat [time (ms)]" << std::endl
	<< "exit" << std::endl
    << "----------------------------------" << std::endl;
}

int main(){;
    std::string cmd, subcmd;
    int id;
    help();
    while(true) 
    {
        try
        {
            std::cin >> cmd;
            if(cmd == "create") 
            {
               std::cin >> id;
               create(id);
            } 
            else if(cmd == "remove") 
            {
                remove(id);
            } 
            else if(cmd == "exec") 
            {
                std::cin >> id;
                exec(id, subcmd);
            }
            else if(cmd == "heartbit")
            {
                heartbit(id);
            }
            else if(cmd == "help")
            {
                help();
            }
            else if(cmd == "exit") 
            {
                system("killall client");
                break; 
            }
        }
        catch(...)
        {
            std::cout << "something gone wrong\n";
        }
    }
    return 0;
}