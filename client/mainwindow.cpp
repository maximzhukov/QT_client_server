#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "parser.h"
#include "model.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),  model(new TreeModel()), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    socket = std::make_shared<QTcpSocket>( new QTcpSocket(this));
    connect(socket.get(), &QTcpSocket::readyRead, this, &MainWindow::slotReadyRead);
    connect(socket.get(), SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            this, SLOT(handleStateChange(QAbstractSocket::SocketState)));
    ui->treeView->setModel(model.get());
    socket->connectToHost("127.0.0.1", 2332);
    next_block_size = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    if(socket->state() == QAbstractSocket::SocketState::ConnectedState || socket->state() == QAbstractSocket::SocketState::ListeningState) {
        send_to_server("GET");
    } else {
        set_log("reconnection...");
        socket->connectToHost("127.0.0.1", 2332);
    }
}

void MainWindow::handleStateChange(QAbstractSocket::SocketState socketState) {
    switch (socketState) {
    case QAbstractSocket::UnconnectedState:
        set_log("QAbstractSocket::UnconnectedState");
        break;
    case QAbstractSocket::HostLookupState:
        set_log("QAbstractSocket::HostLookupState");
        break;
    case QAbstractSocket::ConnectingState:
        set_log("QAbstractSocket::ConnectingState");
        break;
    case QAbstractSocket::ConnectedState:
        set_log("QAbstractSocket::ConnectedState");
        break;
    case QAbstractSocket::BoundState:
        set_log("QAbstractSocket::BoundState");
        break;
    default:
         break;
    }
}
void MainWindow::set_log(QString str) {
    QDateTime date = QDateTime::currentDateTime();
    QString formattedTime = date.toString("dd.MM.yyyy hh:mm:ss");
    QByteArray formattedTimeMsg = formattedTime.toLocal8Bit();
    QString result = ui->textBrowser->toHtml() + "<br>" + "<b>" + QString(formattedTimeMsg) +  "</b>" + " > " + str;
    ui->textBrowser->clear();
    ui->textBrowser->insertHtml(result);
}

void MainWindow::send_to_server(QString str) {
    data.clear();
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_15) ;
    out << str;
    socket->write(data);
    set_log("socket sended message: " + str);
}

void MainWindow::slotReadyRead() {
    QDataStream in(socket.get());
    in.setVersion(QDataStream::Qt_5_15);
    QString result = "";
    if(in.status() == QDataStream::Ok) {
        for(;;) {
            if(next_block_size == 0) {
                if(socket->bytesAvailable() < 2) {
                    break;
                }
                in >> next_block_size;

                if(socket->bytesAvailable() < next_block_size) {
                    break;
                }
                QString str;
                in >> str;
                next_block_size = 0;
                result += str;
            }
        }
        set_log("data received: " + result);
        display_data(result);
    } else {
        set_log("error of data receiving");
    }
}

void MainWindow::display_data(QString data) {
    QVector<Block> result = Parser().parse_json(data);
    model->update(result);
    ui->treeView->reset();
}


