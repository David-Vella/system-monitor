#ifndef DATAFTCHER_H
#define DATAFETCHER_H

#include <string>
#include <iostream>
#include <fstream>
#include <chrono>

class DataFetcher {
public:
    DataFetcher(std::string path);
    void fetch(int &time, int &temp);

private:
    std::string path;
    time_t start;
};

#endif // DATAFETCHER_H
