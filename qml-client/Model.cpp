#include "Model.h"

#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QHostAddress>
#include <QLineEdit>
#include <QNetworkAddressEntry>
#include <QPushButton>
#include <QTcpSocket>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QUrl>

QObject *topLevel = nullptr;

Model::Model(QObject *parent) : QObject(parent)
{
    _socket = new QTcpSocket(this);

    _socket->connectToHost(QHostAddress("127.0.0.1"), 4242);
    // _socket->connectToHost(QHostAddress("192.168.0.101"), 4242);

    connect(_socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(_socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(onSocketStateChanged(QAbstractSocket::SocketState)));
}

Model::~Model()
{
    _socket->disconnectFromHost();
}

void Model::onSocketStateChanged(QAbstractSocket::SocketState socketState)
{
    switch(socketState)
    {
    case QAbstractSocket::UnconnectedState:
        topLevel->setProperty("title", "TCP-client (Unconnected)");
        // _handleButton->setDisabled(true);
        // _reportButton->setDisabled(true);
        break;
    case QAbstractSocket::ConnectingState:
        topLevel->setProperty("title", "TCP-client (Connecting...)");
        // _handleButton->setDisabled(false);
        // _reportButton->setDisabled(false);
        break;
    case QAbstractSocket::ConnectedState:
        topLevel->setProperty("title", QString("TCP-client (Connected to %1)").arg(_socket->peerAddress().toString()));
        break;
    default:
        break;
    }
}

void Model::onReadyRead()
{
    QByteArray datas = _socket->readAll();

    QObject *textArea = topLevel->findChild<QObject*>("textArea");
    if (textArea) 
    {
        textArea->setProperty("text", textArea->property("text").toString() + QString(datas.data()));
    }

}

void Model::connectTo(QString ip) 
{
    qDebug() << "Connect to:" << ip;

    _socket->disconnectFromHost();
    _socket->connectToHost(QHostAddress(ip), 4242);
}

void Model::handleFile(QString fileName)
{
    qDebug() << "Handle file:" << fileName;
    
    QFile file(QUrl(fileName).toLocalFile());
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QByteArray bytes = file.readAll();
        _socket->write(bytes);
    }
    else
    {
        qDebug() << "Unable to open:" << fileName;
    }
}

void Model::getStatistics() 
{
    QByteArray bytes("Get Statistics");
    _socket->write(bytes);
}
