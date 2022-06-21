#ifndef LT_MODEL_H
#define LT_MODEL_H

#include <QObject>

#include <QAbstractSocket>
QT_FORWARD_DECLARE_CLASS(QTcpSocket)

class Model : public QObject
{
    Q_OBJECT
    
public:
    explicit Model(QObject *parent = 0);
    ~Model();

public slots:
    void onReadyRead();
    void onSocketStateChanged(QAbstractSocket::SocketState socketState);
    void connectTo(QString ip);
    void handleFile(QString fileName);
    void getStatistics();

private:
    QTcpSocket*  _socket;

};

#endif // LT_MODEL_H