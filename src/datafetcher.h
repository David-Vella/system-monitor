#ifndef DATAFTCHER_H
#define DATAFETCHER_H

#include <vector>
#include <array>
#include <memory>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

#include <QVector>
#include <QString>

class DataFetcher {
public:
    DataFetcher();

    void getUsageAvailable(QVector<QString> &names);
    void getTempAvailable(QVector<QString> &names);
    void getFanAvailable(QVector<QString> &names);

    void getUsageData(QVector<int> &data);
    void getTempData(QVector<int> &data);
    void getFanData(QVector<int> &data);

    int getUsageNum();
    int getTempNum();
    int getFanNum();

    static constexpr int BUFFER_SIZE = 2048;
    static constexpr const char *SENSORS_COMMAND= "sensors -A";
    static constexpr const char *USAGE_DATA_FILE = "/proc/stat";

private:
    std::string getSensorsOutput();

    QVector<int> usagePrev;

    QVector<QString> usageNames;
    QVector<QString> tempNames;
    QVector<QString> fanNames;

    std::vector<std::string> usage_path;
    std::vector<std::string> usage_name;
    std::vector<int> usage_prev;

    std::vector<std::string> temp_path;
    std::vector<std::string> temp_name;

    std::vector<std::string> fan_path;
    std::vector<std::string> fan_name;
};

#endif // DATAFETCHER_H
