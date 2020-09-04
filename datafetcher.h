#ifndef DATAFTCHER_H
#define DATAFETCHER_H

#include <string>
#include <iostream>
#include <fstream>

class DataFetcher {
public:
    DataFetcher(std::string path);
    int fetch();

private:
    std::string path;
};

#endif // DATAFETCHER_H
