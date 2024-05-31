#ifndef SERVER_H
#define SERVER_H
// Server.h

#include <QTcpServer>
#include <QDebug>
#include <QVector>

class Server : public QTcpServer {
    Q_OBJECT

public:
    int ClinetNums; // 客户端数量，同时也是客户端标记
    bool isOpen;

    explicit Server(QObject *parent = nullptr);

private:
    QVector<bool> bool_connect_list;

protected:
    void incomingConnection(qintptr socketDescriptor) override;

signals:
    void newMessage(const QString &message);
    void newClient(const QString &newClientInfo);
    void close_client(int index);   //
};

#endif // SERVER_H
