#ifndef QTHREADTCPH_H
#define QTHREADTCPH_H

#include <QThread>
#include <QTcpSocket>
#include <QHostAddress>


class ClientThread: public QThread {
    Q_OBJECT

public:
    explicit ClientThread(int socketDescriptor, int client_index, QObject *parent = nullptr);
    void run() override;

signals:
    void messageReceived(const QString &message);   // 收到消息
    void disconnected();        // 关闭线程
    void startSocket();         // 创建新socket连接
    void newconnect(const QString &message);        // 新连接建立

private slots:
    void readMessage();         // 读socket消息
    void clientDisconnected();  // 关闭连接，退出线程
    void createSocket();        // 创建socket

public slots:
    void check_ifClose(int index);       // 判断是否客户端关闭了该socket连接

private:
    int index;
    int socketDescriptor;
    QTcpSocket *tcpSocket;
    QString client_ip;
    QString client_port;
};

#endif // QTHREADTCPH_H
