#include "reporttools.h"
#include <sstream>


Algorithm::Algorithm() : m_info() {

}

Algorithm::~Algorithm() {

}

const std::string &Algorithm::getInfo() const {
    return m_info;
}

NumberCharacters::NumberCharacters() {
    m_info = "количество повторений символа";
}

Algorithm::CalculateResult NumberCharacters::operator()(const char *text) const {
    CalculateResult result;
    int c;
    while((c = (unsigned char)(*text++))) ++(result[c]);
    return result;
}

WordLengths::WordLengths() {
    m_info = "распределение слов по длинам";
}

Algorithm::CalculateResult WordLengths::operator()(const char *text) const {
    CalculateResult result;
    std::istringstream iss(text);
    std::string word;
    while(iss >> word) ++(result[word.size()]);
    return result;
}

void AlgorithmManager::add(const AlgorithmPtr &ap) {
    m_algoritms.emplace(ap->getInfo(), ap);
}

const AlgorithmManager::AlgorithmContainer &AlgorithmManager::getAlgoritms() const {
    return m_algoritms;
}

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

DataHandler::DataHandler(std::initializer_list<AlgorithmPtr> algoritms)
{
    for (const auto& algoritm : algoritms) m_algorithmManager.add(algoritm);
}
