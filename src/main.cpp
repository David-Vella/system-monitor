#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
//    DataFetcher info;
//    QVector<int> data;
//    info.getTempData(data);
//    for (int val : data)
//        std::cout << val << std::endl;
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("CPU Monitor");
    w.show();
    return a.exec();
}
