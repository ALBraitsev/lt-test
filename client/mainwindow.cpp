#include "mainwindow.h"

#include <QDebug>
#include <QHostAddress>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QFile>
#include <QNetworkAddressEntry>

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    _socket(this)
{
    _lineEdit = new QLineEdit;
    _lineEdit->setReadOnly(true);
    _button = new QPushButton("Select file");

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(_lineEdit);
    layout->addWidget(_button);

    _textEdit = new QTextEdit;
    _textEdit->setReadOnly(true);

    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->addLayout(layout);
    vlayout->addWidget(_textEdit);

    setLayout(vlayout);

    connect(_button, SIGNAL(clicked()), this, SLOT(onButtonClicked()));


    const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
    for (const QHostAddress &address: QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != localhost)
             qDebug() << address.toString();
    }

    _socket.connectToHost(QHostAddress("192.168.0.101"), 4242);
    //_socket.connectToHost(QHostAddress::Broadcast, 4242);

    connect(&_socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));

    setWindowTitle("TCP-client");
}

MainWindow::~MainWindow()
{
    _socket.disconnectFromHost();
}

void MainWindow::onReadyRead()
{
    QByteArray datas = _socket.readAll();
    _textEdit->append(datas);
}

void MainWindow::onButtonClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open A File");
    _lineEdit->setText(fileName);

    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QByteArray bytes = file.readAll();
        if (_socket.state() == QAbstractSocket::ConnectedState)
            _socket.write(bytes);
        else
            // _socket.connectToHost(QHostAddress("127.0.0.1"), 4242);
            _socket.connectToHost(QHostAddress::Broadcast, 4242);

    }
}
