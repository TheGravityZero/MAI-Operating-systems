#include <iostream>
#include <zmq.h>
#include <string>
#include <vector>
#include <signal.h>
#include <sstream>
#include <algorithm>
#include "f_server.h"
#include "tree.h"

bool is_number(std::string val)
{
    try
    {
        int tmp = std::stoi(val);
        return true;
    }
    catch(std::exception& e)
    {
        std::cout << "Error: " << e.what() << "\n";
        return false;
    }
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

bool vct_contains(std::vector<int>& vct, int val)
{
    for(size_t i = 0; i < vct.size(); ++i)
    {
        if(vct[i] == val) { return true; }
    }
    return false;
}

void create(IdTree& ids, zmq::socket_t& main_socket, int& port, 
            size_t& child_pid, int& child_id)
{
    size_t node_id = 0;
    std::string str = "";
    std::string result = "";
    std::cin >> str;
    if(!is_number(str)) { return; }
    node_id = std::stoi(str);
    if(child_pid == 0) 
    {
        child_pid = fork();
        if(child_pid == -1) 
        {
            std::cout << "Unable to create first worker node\n";
            child_pid = 0;
            exit(1);
        } 
        else if(child_pid == 0) 
        {
            create_node(node_id, port);
        }
        else 
        {
            child_id = node_id;
            send_message(main_socket,"pid");
            result = recieve_message(main_socket);
        }
    } 
    else 
    {
        std::string msg_s = "create " + std::to_string(node_id);
        send_message(main_socket, msg_s);
        result = recieve_message(main_socket);
    }

    if (result.substr(0,2) == "Ok") { ids.insert(node_id); }
    std::cout << result << "\n";
}
//?????
void remove(IdTree& ids, zmq::socket_t& main_socket, int& port, 
            size_t& child_pid, int& child_id)
{
    size_t node_id = 0;
    std::string str = "";
    std::cin >> str;
    if(!is_number(str)) { return; }
    node_id = std::stoi(str);
    if(ids.find(node_id) == nullptr)
    {
        std::cout << "Error: id doesn't exist\n";
        return;
    }
    if(child_pid == 0) 
    {
        std::cout << "Error:Not found\n";
        return;
    }
    if(node_id == child_id) 
    {
        //Нормальное завершнение процесса
        kill(child_pid, SIGTERM);
        // Фатальное завершение процесса
        kill(child_pid, SIGKILL);
        child_id = 0;
        child_pid = 0;
        ids.erase(node_id);
        std::cout << "Ok\n";
        return;
    }
    std::string message_string = "remove " + std::to_string(node_id);
    send_message(main_socket, message_string);
    std::string recieved_message = recieve_message(main_socket);
    if(recieved_message.substr(0, std::min<int>(recieved_message.size(), 2)) == "Ok") 
    {
        ids.erase(node_id);
    }
    std::cout << recieved_message << "\n";
}

void exec(IdTree& ids, zmq::socket_t& main_socket, int& port, 
          size_t& child_pid, int& child_id)
{
    std::string id_str = "";
    std::string subcmd = "";
    int id = 0; 
    std:: cin >> id_str >> subcmd;
    if(!is_number(id_str) ) { return; }
    //std::cout << subcmd << '\n';
    if((subcmd != "start") && (subcmd != "stop") && (subcmd != "time")) {
        std::cout << "Error: invalid write subcommand\n";
        return;
    }
    id = std::stoi(id_str);
    if(ids.find(id) == nullptr)
    {
        std::cout << "Please\n";
        std::cout << "Error: id doesn't exist\n";
        std::cout << "Please\n";
        return;
    }
    std::string message_string = "exec " + std::to_string(id) + " ";
    message_string += subcmd;
    send_message(main_socket, message_string);
    std::string recieved_message = recieve_message(main_socket);
    std::cout << recieved_message << "\n";
}

void heartbit(IdTree& ids, zmq::socket_t& main_socket, int& port, 
          size_t& child_pid, int& child_id)
{
    int time = 0;
    std::string str_time = "";
    bool all_available = true; 
    int rec_id = 0;
    std::cin >>  str_time;
    if(!is_number(str_time)) { return; }
    time = std::stoi(str_time);
    if(time <= 0)
    {
        std::cout << "Error: time must be > 0\n";
        return;
    }
    std::vector<int> from_tree = ids.get_nodes();
    std::vector<int> recieved_ids;
    for(size_t i = 0; i < from_tree.size(); ++i)
    {
        std::string msg_tree = "heartbit " + std::to_string(time);
        msg_tree += " " + std::to_string(from_tree[i]);
        send_message(main_socket, msg_tree);
        std::string recieved = recieve_message(main_socket);
        if(recieved != "Error: Node is not available")
        {
            try { recieved_ids.push_back(std::stoi(recieved)); }
            catch(...) { std::cout << "Error: stoi\n"; }
        }
    }
    for(size_t i = 0; i < from_tree.size(); ++i)
    {
        if(!vct_contains(recieved_ids, from_tree[i]))
        {
            all_available = false;
            std::cout << "Heartbit: node " << from_tree[i] << " is unavailable\n";
            ids.erase(from_tree[i]);
        }
    }
    if(all_available) { std::cout << "Ok\n"; }
}

int main() 
{
    IdTree ids;
    std::string cmd;
    size_t child_pid = 0;
    int child_id = 0;
    int linger = 0;
    zmq::context_t context(1);
    zmq::socket_t main_socket(context, ZMQ_REQ);
    main_socket.setsockopt(ZMQ_SNDTIMEO, 2000);
    // Задержка, как долго находится сообщение для узла после закрытия узла.
    main_socket.setsockopt(ZMQ_LINGER, &linger, sizeof(linger));
    int port = bind_socket(main_socket);

    help();
    while(true) 
    {
        try
        {
            std::cin >> cmd;
            if(cmd == "create") 
            {
                create(ids, main_socket, port, child_pid, child_id);
            } 
            else if(cmd == "remove") 
            {
                remove(ids, main_socket, port, child_pid, child_id);
            } 
            else if(cmd == "exec") 
            {
                exec(ids, main_socket, port, child_pid, child_id);
            }
            else if(cmd == "heartbit")
            {
                heartbit(ids, main_socket, port, child_pid, child_id);
            }
            else if(cmd == "help")
            {
                help();
            }
            else if(cmd == "exit") 
            {                    
                system("killall -9 client");
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
