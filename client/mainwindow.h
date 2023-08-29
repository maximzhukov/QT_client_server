#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QStandardItemModel>
#include <QApplication>
#include <QModelIndex>
#include <QString>
#include <QTreeView>
#include "treeitem.h"
#include "BlockModel.h"
#include "model.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void send_to_server(QString str);
    void handleStateChange(QAbstractSocket::SocketState socketState);
    void on_pushButton_clicked();

private:
    void set_log(QString str);
    std::shared_ptr<TreeModel> model;
    void slotReadyRead();
    Ui::MainWindow *ui;
    std::shared_ptr<QTcpSocket> socket;
    QByteArray data;
    void display_data(QString data);
    TreeItem *rootItem; //ссылка на корневой узел
    quint16 next_block_size;
};
#endif // MAINWINDOW_H
