#ifndef MYSERVER_H
#define MYSERVER_H

#include <QObject>
#include <QSqlDatabase>
#include <QTcpServer>
#include <QTcpSocket>

#include "reporttools.h"

class MyServer : public QObject
{
    Q_OBJECT

public:
    explicit MyServer(QObject *parent = 0);
    ~MyServer();

public slots:
    void onNewConnection();
    void onSocketStateChanged(QAbstractSocket::SocketState socketState);
    void onReadyRead();

private:
    QTcpServer _server;
    QList<QTcpSocket*> _sockets;
    QSqlDatabase _database;
    DataHandler _dataHanler;
};

#endif // MYSERVER_H
