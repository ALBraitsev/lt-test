#include "myserver.h"

#include <QAbstractSocket>
#include <QDebug>
#include <QHostAddress>
#include <QNetworkInterface>
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

    const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
    for (const QHostAddress &address: QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != localhost)
            qDebug() << "TCP-server running " << address.toString();
    }
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
                // отправка отчёта о работе сервера
                QSqlQuery query = _database.exec("select * from statistics;");
                socket->write(QByteArray("\ntime, address, bytes\n"));
                size_t rc = 0;
                if (query.exec())
                {
                    while (query.next()) 
                    {
                        QString time = query.value(0).toString();
                        QString address = query.value(1).toString();
                        int bytes = query.value(2).toInt();

                        QString record = QString("%1, %2, %3\n").arg(time).arg(address).arg(bytes);

                        qDebug() << record;
                        socket->write(QByteArray(record.toStdString().c_str()));

                        rc++;
                    }
                }
                socket->write(QByteArray(QString("Total record(s) %1\n").arg(rc).toStdString().c_str()));
            }
            else
            {
                // обработка данных и отправка результатов
                socket->write("\n");
                socket->write(_dataHanler.getReport(datas));
                
                // регистрация работы в базе sqlite
                if (_database.isOpen())
                {
                    QString queryString = QString("insert into statistics values (datetime('now', 'localtime'), '%1', %2);").arg(sender->peerAddress().toString()).arg(datas.size());
                    qDebug() << queryString;

                    _database.exec(queryString);
                }
            }
        }
    }
}
