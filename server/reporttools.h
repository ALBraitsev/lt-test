#ifndef REPORT_TOOLS_H
#define REPORT_TOOLS_H

#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <unordered_map>

#include <QByteArray>

struct Algorithm {
    typedef std::unordered_map<size_t, size_t> CalculateResult;

    Algorithm();
    virtual ~Algorithm();;
    virtual CalculateResult operator()(const char* text) const = 0;
    const std::string& getInfo() const;
protected:
    std::string m_info;
};

typedef std::shared_ptr<Algorithm> AlgorithmPtr;

struct NumberCharacters : public Algorithm {
    NumberCharacters();
    CalculateResult operator()(const char* text) const override;
};

struct WordLengths : public Algorithm {
    WordLengths();
    CalculateResult operator()(const char* text) const override;
};

struct AlgorithmManager {
    typedef std::unordered_map<std::string, AlgorithmPtr> AlgorithmContainer;

    void add(const AlgorithmPtr& ap);
    const AlgorithmContainer& getAlgoritms() const;
private:
    AlgorithmContainer m_algoritms;
};

struct DataHandler {
    DataHandler(std::initializer_list<AlgorithmPtr> algoritms);
    QByteArray getReport(const QByteArray& ba) const;

private:
    AlgorithmManager m_algorithmManager;
};

#endif // REPORT_TOOLS_H
