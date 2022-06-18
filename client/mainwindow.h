#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QTcpSocket>

#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void onReadyRead();
    void onButtonClicked();

private:
    QLineEdit* _lineEdit;
    QPushButton* _button;
    QTextEdit* _textEdit;

    QTcpSocket  _socket;
};

#endif // MAINWINDOW_H
