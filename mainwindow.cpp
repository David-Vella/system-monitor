#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), timer(new QTimer(this)),
      cputemp(new DataFetcher("/sys/class/hwmon/hwmon6/temp1_input"))
{
    ui->setupUi(this);
    ui->plot->addGraph();
    ui->plot->xAxis->setTickLabels(false);
    ui->plot->xAxis->setRange(0, 30);

    ui->plot->yAxis->setLabel(QString("Temperature  (deg C)"));
    ui->plot->yAxis->setRange(0, 100);

    connect(timer, SIGNAL(timeout()), this, SLOT(addPoint()));
    timer->start(1000);
}

MainWindow::~MainWindow()
{
    delete cputemp;
    delete timer;
    delete ui;
}

void MainWindow::addPoint() {
    int time, temp;
    cputemp->fetch(time, temp);

    if (x.size() == 0)
        x.push_back(0);
    else if (x.size() < 3600)
        x.push_back(x.back() + 1);

    y.push_front(temp);

    if (y.size() > 3600)
        y.pop_back();

    ui->plot->graph(0)->setData(x, y);
    ui->plot->replot();
    ui->plot->update();
}
