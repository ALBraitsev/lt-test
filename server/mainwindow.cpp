#include "mainwindow.h"

///////////////////////////////////////////////////////////////////////////
#include <sstream>
std::ostream& operator<< (std::ostream& os, const Algorithm::CalculateResult& v) {
    std::map<size_t, size_t> ordered(v.begin(), v.end());
    for (auto& p : ordered) {
        os << p.first << "=" << p.second << std::endl;
    }
    return os;
}

QByteArray DataHandler::getReport(const QByteArray& ba) const {
    std::stringstream report;
    for (const auto& ap : m_algorithmManager.getAlgoritms()) {
        report << ap.first << std::endl;
        report << (*ap.second)(ba.data()) << std::endl;
    }
    return QByteArray::fromStdString(report.str());
}

///////////////////////////////////////////////////////////////////////////

#include <QDebug>
#include <QHostAddress>
#include <QAbstractSocket>

MainWindow::MainWindow(QObject *parent) :
    QObject(parent),
    _server(this),
    _dataHanler({AlgorithmPtr(new NumberCharacters()), AlgorithmPtr(new WordLengths())})
{
    _server.listen(QHostAddress::Any, 4242);
    connect(&_server, SIGNAL(newConnection()), this, SLOT(onNewConnection()));

    qDebug() << "TCP-server running";
}

MainWindow::~MainWindow()
{
    qDebug() << "TCP-server closed";
}

void MainWindow::onNewConnection()
{
   QTcpSocket *clientSocket = _server.nextPendingConnection();
   connect(clientSocket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
   connect(clientSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(onSocketStateChanged(QAbstractSocket::SocketState)));

    _sockets.push_back(clientSocket);
    // for (QTcpSocket* socket : _sockets) {
    //     socket->write(QByteArray::fromStdString(clientSocket->peerAddress().toString().toStdString() + " connected to server !\n"));
    // }

    clientSocket->write(QByteArray::fromStdString(clientSocket->peerAddress().toString().toStdString() + " connected to server !\n"));
}

void MainWindow::onSocketStateChanged(QAbstractSocket::SocketState socketState)
{
    if (socketState == QAbstractSocket::UnconnectedState)
    {
        QTcpSocket* sender = static_cast<QTcpSocket*>(QObject::sender());
        _sockets.removeOne(sender);
    }
}

void MainWindow::onReadyRead()
{
    QTcpSocket* sender = static_cast<QTcpSocket*>(QObject::sender());
    QByteArray datas = sender->readAll();
    for (QTcpSocket* socket : _sockets) {
        if (socket == sender) {
            // socket->write(QByteArray::fromStdString(sender->peerAddress().toString().toStdString() + ": " + datas.toStdString()));
            socket->write(_dataHanler.getReport(datas));
        }
    }
}
