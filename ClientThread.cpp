// ClientThread.cpp
#include "ClientThread.h"

ClientThread::ClientThread(int socketDescriptor, int client_index, QObject *parent)
    : QThread(parent)
    , index(client_index)
    , socketDescriptor(socketDescriptor) {}

void ClientThread::run() {

    connect(this, &ClientThread::startSocket, this, &ClientThread::createSocket);
    emit startSocket();

    exec();
}

void ClientThread::createSocket() {
    tcpSocket = new QTcpSocket();
    if (!tcpSocket->setSocketDescriptor(socketDescriptor)) {
        // Handle error
        return;
    }

        quint32 clientIP = tcpSocket->peerAddress().toIPv4Address();
        QHostAddress client_ipv4(clientIP);
        quint16 clientPort = tcpSocket->peerPort();

        client_ip = client_ipv4.toString();
        client_port = QString("%1").arg(clientPort);

        QString message = QString("> New connection from IP: %1, Port: %2\n")
                            .arg(client_ip)
                            .arg(client_port);
        emit messageReceived(message);

        //
        QString clientInfomation =  QString("%1 -> %2:%3")
                                        .arg(index)
                                        .arg(client_ip)
                                        .arg(client_port);
        emit newconnect(clientInfomation);

    connect(tcpSocket, &QTcpSocket::readyRead, this, &ClientThread::readMessage);
    connect(tcpSocket, &QTcpSocket::disconnected, this, &ClientThread::clientDisconnected);
}

void ClientThread::readMessage() {
    QByteArray buffer; // 缓冲区

    while (tcpSocket->bytesAvailable()) {
            buffer.append(tcpSocket->readAll()); // 将所有可用数据添加到缓冲区中
    }



    if (!buffer.isEmpty())
    {
        QString message = QString::fromUtf8(buffer); // 将缓冲区中的数据转换为字符串
        emit messageReceived(message); // 发射包含所有可用数据的信号
        tcpSocket->write("Echo: " + message.toUtf8()); // 回送数据
    }
}

void ClientThread::clientDisconnected()
{
    QString message = QString("> Connection disconnect from IP: %1, Port: %2\n")
                        .arg(client_ip)
                        .arg(client_port);
    emit messageReceived(message);

    // 关闭 tcp 连接
    if (tcpSocket->state() == QAbstractSocket::UnconnectedState) {
        tcpSocket->close();
    }

    emit disconnected();
    quit();
}

void ClientThread::check_ifClose(int index)
{
    if (index == this->index) {
        tcpSocket->close();
    }
}
