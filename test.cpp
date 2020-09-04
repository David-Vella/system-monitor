#include <iostream>
#include <chrono>
#include <unistd.h>

#include "datafetcher.h"

int main() {
    DataFetcher cputemp("/sys/class/hwmon/hwmon6/temp1_input");
    while(true) {
        time_t time = std::time(NULL);
        std::cout << time << ' ' << cputemp.fetch() << std::endl;
        sleep(1);
    }
    return 0;
}
