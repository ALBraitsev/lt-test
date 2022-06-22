#include "mainwindow.h"

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

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
    _socket = new QTcpSocket(this);

    _lineEdit = new QLineEdit;

    _connectButton = new QPushButton("Connect to...");
    _handleButton = new QPushButton("Handle file...");
    _reportButton = new QPushButton("Get Report");

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(_lineEdit);
    layout->addWidget(_connectButton);
    layout->addWidget(_handleButton);
    layout->addWidget(_reportButton);

    _textEdit = new QTextEdit;

    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->addLayout(layout);
    vlayout->addWidget(_textEdit);

    setLayout(vlayout);

    connect(_connectButton, SIGNAL(clicked()), this, SLOT(onConnectButtonClicked()));
    connect(_handleButton, SIGNAL(clicked()), this, SLOT(onHandleButtonClicked()));
    connect(_reportButton, SIGNAL(clicked()), this, SLOT(onReportButtonClicked()));

    const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
    for (const QHostAddress &address: QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != localhost)
             qDebug() << address.toString();
    }
    _socket->connectToHost(QHostAddress("127.0.0.1"), 4242);

    connect(_socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(_socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(onSocketStateChanged(QAbstractSocket::SocketState)));
 
    setWindowTitle("lt-client");
}

MainWindow::~MainWindow()
{
    _socket->disconnectFromHost();
}

void MainWindow::onSocketStateChanged(QAbstractSocket::SocketState socketState)
{
    switch(socketState)
    {
    case QAbstractSocket::UnconnectedState:
        setWindowTitle("lt-client (Unconnected)");
        _handleButton->setDisabled(true);
        _reportButton->setDisabled(true);
        break;
    case QAbstractSocket::ConnectingState:
        setWindowTitle("lt-client (Connecting...)");
        _handleButton->setDisabled(true);
        _reportButton->setDisabled(true);
        break;
    case QAbstractSocket::ConnectedState:
        setWindowTitle(QString("lt-client (Connected to %1)").arg(_socket->peerAddress().toString()));
        _handleButton->setDisabled(false);
        _reportButton->setDisabled(false);
        break;
    default:
        break;
    }
}

void MainWindow::onReadyRead()
{
    QByteArray datas = _socket->readAll();
    _textEdit->append(datas);
}

void MainWindow::onConnectButtonClicked()
{
    _socket->disconnectFromHost();
    _socket->connectToHost(QHostAddress(_lineEdit->text()), 4242);
}

void MainWindow::onHandleButtonClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open A File");
    _lineEdit->setText(fileName);

    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QByteArray bytes = file.readAll();
        _socket->write(bytes);
    }
}

void MainWindow::onReportButtonClicked()
{
    QByteArray bytes("Get Statistics");
    _socket->write(bytes);
}
