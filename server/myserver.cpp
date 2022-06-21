#include "myserver.h"

#include <QDebug>
#include <QHostAddress>
#include <QAbstractSocket>
#include <QSqlQuery>

MyServer::MyServer(QObject *parent) : QObject(parent),
                                      _server(this),
                                      _dataHanler({AlgorithmPtr(new NumberCharacters()), AlgorithmPtr(new WordLengths())})
{
    _server.listen(QHostAddress::Any, 4242);
    connect(&_server, SIGNAL(newConnection()), this, SLOT(onNewConnection()));

    _database = QSqlDatabase::addDatabase("QSQLITE");
    _database.setDatabaseName("l-telecom.db");
    _database.open();
    _database.exec("create table statistics(time text, address text, bytes integer);");

    qDebug() << "TCP-server running";
}

MyServer::~MyServer()
{
    qDebug() << "TCP-server closed";
    _database.close();
    _server.close();
}

void MyServer::onNewConnection()
{
    QTcpSocket *clientSocket = _server.nextPendingConnection();
    connect(clientSocket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(clientSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(onSocketStateChanged(QAbstractSocket::SocketState)));

    _sockets.push_back(clientSocket);
    clientSocket->write(QByteArray::fromStdString(clientSocket->peerAddress().toString().toStdString() + " connected to server !\n"));
}

void MyServer::onSocketStateChanged(QAbstractSocket::SocketState socketState)
{
    if (socketState == QAbstractSocket::UnconnectedState)
    {
        QTcpSocket *sender = static_cast<QTcpSocket *>(QObject::sender());
        _sockets.removeOne(sender);
    }
}

void MyServer::onReadyRead()
{
    QTcpSocket *sender = static_cast<QTcpSocket *>(QObject::sender());
    QByteArray datas = sender->readAll();

    for (QTcpSocket *socket : _sockets)
    {
        if (socket == sender)
        {
            if (datas == "Get Statistics")
            {
                QSqlQuery query = _database.exec("select * from statistics;");
                while (query.next()) {
                    QString time = query.value(0).toString();
                    QString address = query.value(1).toString();
                    int bytes = query.value(2).toInt();

                    QString record = QString("%1, %2, %3\n").arg(time).arg(address).arg(bytes);

                    qDebug() << record;
                    socket->write(QByteArray(record.toStdString().c_str()));
                }
            }
            else
            {
                socket->write(_dataHanler.getReport(datas));
            }
        }
    }

    if (_database.isOpen())
    {
        QString queryString = QString("insert into statistics values (datetime('now', 'localtime'), '%1', %2);").arg(sender->peerAddress().toString()).arg(datas.size());
        qDebug() << queryString;

        _database.exec(queryString);
    }
}
