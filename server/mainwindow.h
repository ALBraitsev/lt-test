#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

class MainWindow : public QObject
{
    Q_OBJECT

public:
    explicit MainWindow(QObject *parent = 0);
    ~MainWindow();

public slots:
    void onNewConnection();
    void onSocketStateChanged(QAbstractSocket::SocketState socketState);
    void onReadyRead();

private:
    QTcpServer  _server;
    QList<QTcpSocket*>  _sockets;
};

#endif // MAINWINDOW_H
