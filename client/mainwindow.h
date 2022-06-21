#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QAbstractSocket>

QT_FORWARD_DECLARE_CLASS(QLineEdit)
QT_FORWARD_DECLARE_CLASS(QPushButton)
QT_FORWARD_DECLARE_CLASS(QTcpSocket)
QT_FORWARD_DECLARE_CLASS(QTextEdit)

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void onReadyRead();
    void onConnectButtonClicked();
    void onHandleButtonClicked();
    void onReportButtonClicked();
    void onSocketStateChanged(QAbstractSocket::SocketState socketState);

private:
    QLineEdit* _lineEdit;
    QPushButton* _connectButton;
    QPushButton* _handleButton;
    QPushButton* _reportButton;
    QTextEdit* _textEdit;
    QTcpSocket*  _socket;
};

#endif // MAINWINDOW_H
