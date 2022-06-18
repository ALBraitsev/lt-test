#ifndef MAINWINDOW_H
#define MAINWINDOW_H

///////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <unordered_map>

#include <QByteArray>

struct Algorithm {
    typedef std::unordered_map<size_t, size_t> CalculateResult;

    Algorithm() : m_info() {};
    virtual ~Algorithm() {};
    virtual CalculateResult operator()(const char* text) const = 0;
    const std::string& getInfo() const {
        return m_info;
    }
protected:
    std::string m_info;
};

typedef std::shared_ptr<Algorithm> AlgorithmPtr;

struct NumberCharacters : public Algorithm {
    NumberCharacters() {
        m_info = "количество повторений символа";
    }
    CalculateResult operator()(const char* text) const override {
        CalculateResult result;
        int c;
        while((c = (unsigned char)(*text++))) ++(result[c]);
        return result;
    }
};

struct WordLengths : public Algorithm {
    WordLengths() {
        m_info = "распределение слов по длинам";
    }
    CalculateResult operator()(const char* text) const override {
        CalculateResult result;
        std::istringstream iss(text);
        std::string word;
        while(iss >> word) ++(result[word.size()]);
        return result;
    }
};

struct AlgorithmManager {
    void add(const AlgorithmPtr& ap) {
        m_algoritms.emplace(ap->getInfo(), ap);
    }
    const std::unordered_map<std::string, AlgorithmPtr>& getAlgoritms() const {
        return m_algoritms;
    }
private:
    std::unordered_map<std::string, AlgorithmPtr> m_algoritms;
};

struct DataHandler {
    DataHandler(std::initializer_list<AlgorithmPtr> algoritms)
    {
        for (const auto& algoritm : algoritms) m_algorithmManager.add(algoritm);
    }
    QByteArray getReport(const QByteArray& ba) const;

private:
    AlgorithmManager m_algorithmManager;
};
///////////////////////////////////////////////////////////////////////////


#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

class MainWindow : public QObject
{
    Q_OBJECT

public:
    explicit MainWindow(QObject *parent = 0);
    ~MainWindow();

public slots:
    void onNewConnection();
    void onSocketStateChanged(QAbstractSocket::SocketState socketState);
    void onReadyRead();

private:
    QTcpServer  _server;
    QList<QTcpSocket*>  _sockets;
    DataHandler _dataHanler;
};

#endif // MAINWINDOW_H
