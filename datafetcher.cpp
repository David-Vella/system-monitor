#include "datafetcher.h"

DataFetcher::DataFetcher() {
    getUsageAvailable(usageNames);
    getTempAvailable(tempNames);
    getFanAvailable(fanNames);

    for (int i = 0; i < getUsageNum(); ++i)
        usagePrev.push_back(0);
}

void DataFetcher::getUsageAvailable(QVector<QString> &names) {
    names.clear();

    std::ifstream fin(USAGE_DATA_FILE, std::ios::in);
    if (!fin.is_open())
        throw std::runtime_error("Failed to open file: " + std::string(USAGE_DATA_FILE));

    std::string line;

    while (std::getline(fin, line)) {
        if (line.find("cpu") != std::string::npos) {
            std::string name = line.substr(0, line.find(' '));
            names.push_back(QString(name.c_str()));
        }
    }
    fin.close();
}

void DataFetcher::getTempAvailable(QVector<QString> &names) {
    names.clear();

    std::istringstream strin(getSensorsOutput());

    bool first = true;
    std::string line, device;
    while (std::getline(strin, line)) {
        if (line.empty() || first) {
            std::getline(strin, line);
            device = line.substr(0, line.find('-'));
            first = false;
        }
        else if (line.find(" C") != std::string::npos
                 && line.find(':') != std::string::npos) {
            std::string name = line.substr(0, line.find(':'));
            name = device + ':' + name;
            names.push_back(QString(name.c_str()));
        }
    }
}

void DataFetcher::getFanAvailable(QVector<QString> &names) {
    names.clear();

    std::istringstream strin(getSensorsOutput());

    bool first = true;
    std::string line, device;
    while (std::getline(strin, line)) {
        if (line.empty() || first) {
            std::getline(strin, line);
            device = line.substr(0, line.find('-'));
            first = false;
        }
        else if (line.find(" RPM") != std::string::npos
                 && line.find(':') != std::string::npos) {
            std::string name = line.substr(0, line.find(':'));
            name = device + ':' + name;
            names.push_back(QString(name.c_str()));
        }
    }
}

std::string DataFetcher::getSensorsOutput() {
    std::array<char, BUFFER_SIZE> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(SENSORS_COMMAND, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("Failed to open pipe for sensor input");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

void DataFetcher::getUsageData(QVector<int> &data) {
    static bool first_call = true;

    data.clear();

    QVector<QString>::iterator namesIter = usageNames.begin();
    QVector<int>::iterator prevIter = usagePrev.begin();

    std::ifstream fin(USAGE_DATA_FILE, std::ios::in);
    if (!fin.is_open())
        throw std::runtime_error("Failed to open file: " + std::string(USAGE_DATA_FILE));

    std::string line;

    while (std::getline(fin, line)) {
        if (namesIter == usageNames.end())
            break;
        else if (line.find(namesIter->toStdString()) == std::string::npos)
            continue;

        int beg = line.find(' ');
            while (line[beg] == ' ')
                ++beg;
        int end = line.find(' ', beg + 1);

        std::string str = line.substr(beg + 1, end - beg - 1);
        int cumulative = std::stoi(str);
        int usage = cumulative - *prevIter;
        *prevIter = cumulative;

        if (first_call)
            data.push_back(0);
        else
            data.push_back(usage);

        ++namesIter;
        ++prevIter;
    }
    fin.close();

    if (first_call)
        first_call = false;
}

void DataFetcher::getTempData(QVector<int> &data) {
    data.clear();

    std::istringstream strin(getSensorsOutput());
    std::string line;

    QVector<QString>::iterator iter = tempNames.begin();

    while (std::getline(strin, line)) {
        if (iter == tempNames.end())
            break;

        QString needle = iter->right(iter->length() - iter->indexOf(':') - 1);

        if (line.find(needle.toStdString()) != std::string::npos) {
            int beg = line.find(": ");
            int end = line.find(" C ");
            std::string stringData = line.substr(beg + 1, end - 1);

            int i = 0;
            while (stringData[i] == ' ')
                ++i;
            stringData = stringData.substr(i);

            data.push_back(std::stoi(stringData));

            ++iter;
        }
    }
}

void DataFetcher::getFanData(QVector<int> &data) {
    data.clear();

    std::istringstream strin(getSensorsOutput());
    std::string line;

    QVector<QString>::iterator iter = fanNames.begin();

    while (std::getline(strin, line)) {
        if (iter == fanNames.end())
            break;

        QString needle = iter->right(iter->length() - iter->indexOf(':') - 1);

        if (line.find(needle.toStdString()) != std::string::npos) {
            int beg = line.find(": ");
            int end = line.find(" C ");
            std::string stringData = line.substr(beg + 1, end - 1);

            int i = 0;
            while (stringData[i] == ' ')
                ++i;
            stringData = stringData.substr(i);

            data.push_back(std::stoi(stringData));

            ++iter;
        }
    }
}

int DataFetcher::getUsageNum() {
    return usageNames.size();
}

int DataFetcher::getTempNum() {
    return tempNames.size();
}

int DataFetcher::getFanNum() {
    return fanNames.size();
}
