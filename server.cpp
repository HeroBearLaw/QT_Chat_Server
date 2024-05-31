// Server.cpp
#include "server.h"
#include "ClientThread.h"

Server::Server(QObject *parent) : QTcpServer(parent) , ClinetNums(1), isOpen(0), bool_connect_list(256, false){}

void Server::incomingConnection(qintptr socketDescriptor)
{
    // select index
    int client_index = 1;
    for (bool i: bool_connect_list) {
        if (i) {
            client_index++;
        } else {
            bool_connect_list[client_index - 1] = true;
            break;
        }
    }

    // 创建线程
    ClientThread *thread = new ClientThread(socketDescriptor, client_index, this);

    connect(thread, &ClientThread::messageReceived, this, &Server::newMessage);
    //connect(thread, &ClientThread::disconnected, thread, &ClientThread::deleteLater);
    connect(thread, &ClientThread::newconnect, this, &Server::newClient);
    connect(this, &Server::close_client, thread, &ClientThread::check_ifClose);
    thread->start();
}
