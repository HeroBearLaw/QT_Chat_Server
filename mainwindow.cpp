#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QFont font;
    font.setFamily("Consolas");
    font.setBold(true);
    font.setPointSize(12);

    // line edit
    ui->lineEdit_port->setFont(font);
    ui->lineEdit_port->setText("8844");

    // start close button
    ui->pushButton_open_service->setFont(font);
    ui->pushButton_open_service->setText("OPEN");
    ui->pushButton_open_service->setStyleSheet("background-color: white;");

    // disconnected button
    ui->pushButton_disconnect->setFont(font);
    ui->pushButton_disconnect->setText("DISCONNECTED");

    // textEdit_log
    ui->textEdit_log->append("> 输入端口号, 点击start以开启服务器...\n");

    // 创建服务器对象
    chat_server = new Server(this);

    //
    connect(chat_server, &Server::newMessage, this, &MainWindow::displayMessage);
    connect(chat_server, &Server::newClient, this, &MainWindow::upgradeClient);
    connect(this, &MainWindow::close_client,chat_server, &Server::close_client);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::displayMessage(const QString &message)
{
    // 展示连接信息
    ui->textEdit_log->append(message);
}

// 开启服务器关闭服务器按键
void MainWindow::on_pushButton_open_service_clicked()
{
    if (!chat_server->isOpen)
    {
        // 开启监听
        if (!chat_server->listen(QHostAddress::Any, ui->lineEdit_port->text().toUShort())) {
            ui->textEdit_log->append("Server failed to start!\n");
            return;
        }

        chat_server->isOpen = 1;

        ui->pushButton_open_service->setStyleSheet("background-color: #FFB6C1;");
        ui->pushButton_open_service->setText("CLOSE");

        ui->textEdit_log->append(QString("Server started on port:%1!\n").arg(ui->lineEdit_port->text().toUShort()));
    } else {
        // 关闭监听
        chat_server->close();

        //关闭所有的连接
        for (int i = 0; i < ui->listWidget_client->count(); ++i) {
                    QListWidgetItem *item = ui->listWidget_client->item(i);
                    QRegularExpression regex("(\\d+)\\s*->"); // 匹配数字后跟着一个箭头
                    QRegularExpressionMatch match = regex.match(item->text());

                    if (match.hasMatch()) {
                        QString numberString = match.captured(1);
                        int number = numberString.toInt();
                        emit close_client(number);
                        //qDebug() << "Extracted number:" << number;
                    } else {
                        qDebug() << "No match found for:" << item->text();
                    }
                }
        ui->listWidget_client->clear();

        // 设置标志位 更新
        chat_server->isOpen = 0;
        chat_server->ClinetNums = 1;
        ui->pushButton_open_service->setText("OPEN");
        ui->pushButton_open_service->setStyleSheet("background-color: white;");
    }
}


void MainWindow::upgradeClient(const QString &clientInfo)
{
    chat_server->ClinetNums += 1;

    // 更新连接框的信息
    ui->listWidget_client->addItem(clientInfo);

}

// 点击断开按钮
void MainWindow::on_pushButton_disconnect_clicked()
{
    QList<QListWidgetItem*> selectedItems = ui->listWidget_client->selectedItems();
    if (!selectedItems.isEmpty())
    {
        for (QListWidgetItem *item : selectedItems)
        {
            QRegularExpression regex("(\\d+)\\s*->"); // 匹配数字后跟着一个箭头
            QRegularExpressionMatch match = regex.match(item->text());

            if (match.hasMatch()) {
                QString numberString = match.captured(1);
                int number = numberString.toInt();
                emit close_client(number);
                //qDebug() << "Extracted number:" << number;
            } else {
                qDebug() << "No match found for:" << item->text();
            }

            ui->listWidget_client->takeItem(ui->listWidget_client->row(ui->listWidget_client->currentItem()));
        }
    }
}
