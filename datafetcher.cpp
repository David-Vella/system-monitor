#include "datafetcher.h"

DataFetcher::DataFetcher(const char *path) : config_file(path) {
    parse_config();

    for (auto path : usage_path)
        usage_prev.push_back(0);
}

int DataFetcher::parse_line(std::string line, Section state) {
    size_t pos = line.find('=');
    if (pos == std::string::npos) {
        std::cout << "Failed to parse line: " << line << std::endl;
        return -1;
    }

    std::string name = line.substr(0, pos);
    std::string path = line.substr(pos + 1);

    if (state == USAGE) {
        usage_name.push_back(name);
        usage_path.push_back(path);
    } else if (state == TEMP) {
        temp_name.push_back(name);
        temp_path.push_back(path);
    } else if (state == FAN) {
        fan_name.push_back(name);
        fan_path.push_back(path);
    }

    return 0;
}

int DataFetcher::parse_config() {
    std::ifstream fin(config_file, std::ios::in);

    if (!fin.is_open()) {
        std::cout << "Failed to open config file: " << config_file << std::endl;
        return -1;
    }

    std::string line;
    Section state;

    while (std::getline(fin, line)) {
        if (line.empty() || line.front() == '#')
            continue;
        else if (line == "[usage]") {
            state = USAGE;
            continue;
        } else if (line == "[temp]") {
            state = TEMP;
            continue;
        } else if (line == "[fan]") {
            state = FAN;
            continue;
        }

        if (parse_line(line, state))
            return -1;
    }
    fin.close();

    return 0;
}

bool DataFetcher::checkConfig() {
    if (parse_config())
        return false;
    else
        return true;
}

void DataFetcher::fetchUsage(QVector<int> &data) {
    static bool first_call = true;

    data.clear();

    auto path_iter = usage_path.begin();
    auto prev_iter = usage_prev.begin();

    std::string line;

    std::ifstream fin(USAGE_DATA_FILE, std::ios::in);
    if (!fin.is_open()) {
        std::cout << "Failed to open file: " << USAGE_DATA_FILE << std::endl;
        return;
    }

    while (std::getline(fin, line)) {
        if (path_iter == usage_path.end())
            break;
        else if (line.find(*path_iter) == std::string::npos)
            continue;

        int beg = line.find(' ');
        int end = line.find(' ', beg + 1);

        std::string str_usage = line.substr(beg + 1, end - beg - 1);
        int cumulative = std::stoi(str_usage);
        int usage = cumulative - *prev_iter;
        *prev_iter = cumulative;

        if (first_call)
            data.push_back(0);
        else
            data.push_back(usage);

        ++path_iter;
        ++prev_iter;
    }
    fin.close();

    if (first_call)
        first_call = false;
}

void DataFetcher::fetchTemp(QVector<int> &data) {
    data.clear();

    for (auto path : temp_path) {
        std::ifstream fin(path, std::ios::in);

        if (!fin.is_open()) {
            std::cout << "Failed to open file: " << path << std::endl;
            return;
        }

        int temp;
        fin >> temp;
        temp /= 1000;

        data.push_back(temp);

        fin.close();
    }
}

void DataFetcher::fetchFan(QVector<int> &data) {
    data.clear();

    for (auto path : fan_path) {
        std::ifstream fin(path, std::ios::in);

        if (!fin.is_open()) {
            std::cout << "Failed to open file: " << path << std::endl;
            return;
        }

        int speed;
        fin >> speed;

        data.push_back(speed);

        fin.close();
    }
}

void DataFetcher::getUsageNames(QVector<QString> &names) {
    names.clear();
    for (auto name : usage_name)
        names.push_back(name.c_str());
}

void DataFetcher::getTempNames(QVector<QString> &names) {
    names.clear();
    for (auto name : temp_name)
        names.push_back(name.c_str());
}

void DataFetcher::getFanNames(QVector<QString> &names) {
    names.clear();
    for (auto name : fan_name)
        names.push_back(name.c_str());
}

int DataFetcher::getUsageNum() {
    return usage_name.size();
}

int DataFetcher::getTempNum() {
    return temp_name.size();
}

int DataFetcher::getFanNum() {
    return fan_name.size();
}
