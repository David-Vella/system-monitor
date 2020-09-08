#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow),
      timer(new QTimer(this)), data(new DataFetcher) {
    ui->setupUi(this);
    setupPlots();
    setupSelect();
}

MainWindow::~MainWindow() {
    delete data;
    delete timer;
    delete ui;

    clearVector(usageItems);
    clearVector(tempItems);
    clearVector(fanItems);
}

void MainWindow::clearVector(QVector<QListWidgetItem*> vec) {
    for (QListWidgetItem *item : vec)
        delete item;
}

void MainWindow::plot() {
    QVector<int> usage, temp, fan;

    data->getUsageData(usage);
    data->getTempData(temp);
    data->getFanData(fan);

    if (x.size() == 0)
        x.push_back(0);
    else if (x.size() < VECTOR_MAX_LENGTH)
        x.push_back(x.back() + 1);

    for (int i = 0; i < data->getUsageNum(); ++i) {
        yUsage[i].push_front(usage[i]);

        if (yUsage[i].size() > VECTOR_MAX_LENGTH)
            yUsage[i].pop_back();

        if (usageItems[i]->checkState()) {
            ui->usagePlot->graph(i)->setData(x, yUsage[i]);
            ui->usagePlot->graph(i)->setVisible(true);
        } else {
            ui->usagePlot->graph(i)->setVisible(false);
        }
    }
    for (int i = 0; i < data->getTempNum(); ++i) {
        yTemp[i].push_front(temp[i]);

        if (yTemp[i].size() > VECTOR_MAX_LENGTH)
            yTemp[i].pop_back();

        if (tempItems[i]->checkState()) {
            ui->tempPlot->graph(i)->setData(x, yTemp[i]);
            ui->tempPlot->graph(i)->setVisible(true);
        } else {
            ui->tempPlot->graph(i)->setVisible(false);
        }
    }
    for (int i = 0; i < data->getFanNum(); ++i) {
        yFan[i].push_front(fan[i]);

        if (yFan[i].size() > VECTOR_MAX_LENGTH)
            yFan[i].pop_back();

        if (fanItems[i]->checkState()) {
            ui->fanPlot->graph(i)->setData(x, yFan[i]);
            ui->fanPlot->graph(i)->setVisible(true);
        } else {
            ui->fanPlot->graph(i)->setVisible(false);
        }
    }

    ui->usagePlot->xAxis->setRange(0, getTime());
    ui->usagePlot->replot();
    ui->usagePlot->update();

    ui->tempPlot->xAxis->setRange(0, getTime());
    ui->tempPlot->replot();
    ui->tempPlot->update();

    ui->fanPlot->xAxis->setRange(0, getTime());
    ui->fanPlot->replot();
    ui->fanPlot->update();
}

void MainWindow::setupPlots() {
    ColorGenerator getColor;

    QVector<QString> names;

    data->getUsageAvailable(names);
    for (int i = 0; i < data->getUsageNum(); ++i) {
        ui->usagePlot->addGraph()->setName(names[i]);
        ui->usagePlot->graph(i)->setPen(QPen(getColor()));
        yUsage.push_back(QVector<double>());
    }

    getColor.reset();
    data->getTempAvailable(names);
    for (int i = 0; i < data->getTempNum(); ++i) {
        ui->tempPlot->addGraph()->setName(names[i]);
        ui->tempPlot->graph(i)->setPen(QPen(getColor()));
        yTemp.push_back(QVector<double>());
    }

    getColor.reset();
    data->getFanAvailable(names);
    for (int i = 0; i < data->getFanNum(); ++i) {
        ui->fanPlot->addGraph()->setName(names[i]);
        ui->fanPlot->graph(i)->setPen(QPen(getColor()));
        yFan.push_back(QVector<double>());
    }

    ui->usagePlot->xAxis->setTickLabels(false);
    ui->usagePlot->xAxis->setRange(0, getTime());
    ui->usagePlot->yAxis->setLabel("Usage  (%)");
    ui->usagePlot->yAxis->setRange(0, 100);

    ui->tempPlot->xAxis->setTickLabels(false);
    ui->tempPlot->xAxis->setRange(0, getTime());
    ui->tempPlot->yAxis->setLabel("Temperature  (deg C)");
    ui->tempPlot->yAxis->setRange(0, 100);

    ui->fanPlot->xAxis->setTickLabels(false);
    ui->fanPlot->xAxis->setRange(0, getTime());
    ui->fanPlot->yAxis->setLabel("Fan Speed  (RPM)");
    ui->fanPlot->yAxis->setRange(0, 5000);

    connect(timer, SIGNAL(timeout()), this, SLOT(plot()));
    timer->start(1000);
}

void MainWindow::setupSelect() {
    QVector<QString> names;
    ColorGenerator getColor;

    data->getUsageAvailable(names);
    for (QString name : names) {
        usageItems.push_back(new QListWidgetItem(name, ui->usageSelect));
        usageItems.back()->setBackground(QBrush(getColor()));
        usageItems.back()->setCheckState(Qt::Unchecked);
        ui->usageSelect->addItem(usageItems.back());
    }
    getColor.reset();

    data->getTempAvailable(names);
    for (QString name : names) {
        tempItems.push_back(new QListWidgetItem(name, ui->tempSelect));
        tempItems.back()->setBackground(QBrush(getColor()));
        tempItems.back()->setCheckState(Qt::Unchecked);
        ui->tempSelect->addItem(tempItems.back());
    }
    getColor.reset();

    data->getFanAvailable(names);
    for (QString name : names) {
        fanItems.push_back(new QListWidgetItem(name, ui->fanSelect));
        fanItems.back()->setBackground(QBrush(getColor()));
        fanItems.back()->setCheckState(Qt::Unchecked);
        ui->fanSelect->addItem(fanItems.back());
    }
}

int MainWindow::getTime() {
    QString string = ui->TimeDropDown->currentText();

    if (string == "30sec")
        return 30;
    else if (string == "1min")
        return 60;
    else if (string == "5min")
        return 300;
    else if (string == "15min")
        return 900;
    else if (string == "30min")
        return 1800;
    else
        return 3600;
}

MainWindow::ColorGenerator::ColorGenerator() {
    reset();
}

QColor MainWindow::ColorGenerator::operator()() {
    if (colors.size() == 0)
        reset();

    QColor color = colors.back();
    colors.pop_back();
    return color;
}

void MainWindow::ColorGenerator::reset() {
    colors = {
        Qt::lightGray,
        Qt::darkCyan,
        Qt::darkMagenta,
        Qt::darkGray,
        Qt::darkYellow,
        Qt::darkGreen,
        Qt::darkBlue,
        Qt::darkRed,
        Qt::cyan,
        Qt::magenta,
        Qt::green,
        Qt::yellow,
        Qt::gray,
        Qt::black,
        Qt::red,
        Qt::blue
    };
}
