#include "datafetcher.h"

DataFetcher::DataFetcher(std::string path_in)
    : path(path_in), start(std::time(NULL)) { }

void DataFetcher::fetch(int &time, int &temp) {
    time = 0;
    temp = 0;

    std::ifstream fin(path);
    if (!fin.is_open())
        return;

    fin >> temp;
    temp /= 1000;

    fin.close();

    time = (int)(std::time(NULL) - start);
}
