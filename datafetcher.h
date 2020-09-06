#ifndef DATAFTCHER_H
#define DATAFETCHER_H

#include <vector>
#include <iostream>
#include <fstream>

#include <QVector>
#include <QString>

class DataFetcher {
public:
    DataFetcher(const char *path);

    bool checkConfig();

    void fetchUsage(QVector<int> &data);
    void fetchTemp(QVector<int> &data);
    void fetchFan(QVector<int> &data);

    void getUsageNames(QVector<QString> &names);
    void getTempNames(QVector<QString> &names);
    void getFanNames(QVector<QString> &names);

    int getUsageNum();
    int getTempNum();
    int getFanNum();

    static constexpr const char *DEFAULT_CONFIG_FILE = "/home/david/projects/hwmon/hwmon.conf";
    static constexpr const char *USAGE_DATA_FILE = "/proc/stat";

private:
    enum Section { USAGE, TEMP, FAN };

    int parse_line(std::string line, Section state);
    int parse_config();

    std::string config_file;

    std::vector<std::string> usage_path;
    std::vector<std::string> usage_name;
    std::vector<int> usage_prev;

    std::vector<std::string> temp_path;
    std::vector<std::string> temp_name;

    std::vector<std::string> fan_path;
    std::vector<std::string> fan_name;
};

#endif // DATAFETCHER_H
