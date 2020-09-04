#include "datafetcher.h"

DataFetcher::DataFetcher(std::string path_in) : path(path_in) { }

int DataFetcher::fetch() {
    std::ifstream fin(path);
    if (!fin.is_open())
        return 0;

    int temp;
    fin >> temp;
    temp /= 1000;

    fin.close();

    return temp;
}
