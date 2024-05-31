#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QTcpSocket>
#include <QRegularExpression>
#include "server.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Server *chat_server;

private slots:
    void displayMessage(const QString &message);
    void upgradeClient(const QString &clientInfo);
    void on_pushButton_open_service_clicked();
    void on_pushButton_disconnect_clicked();

signals:
    void close_client(int index);
};
#endif // MAINWINDOW_H
