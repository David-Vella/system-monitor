#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      timer(new QTimer(this)),
      data(new DataFetcher(DataFetcher::DEFAULT_CONFIG_FILE)),
      layouts(new QCPLayoutGrid[3]) {
    ui->setupUi(this);
    setupPlots();
}

MainWindow::~MainWindow() {
    delete data;
    delete timer;
    delete ui;
    delete[] layouts;
}

void MainWindow::plot() {
    QVector<int> usage, temp, fan;

    data->fetchUsage(usage);
    data->fetchTemp(temp);
    data->fetchFan(fan);

    if (x.size() == 0)
        x.push_back(0);
    else if (x.size() < 3600)
        x.push_back(x.back() + 1);

    for (int i = 0; i < data->getUsageNum(); ++i) {
        y_usage[i].push_front(usage[i]);
        if (y_usage[i].size() > 3600)
            y_usage[i].pop_back();
        ui->usagePlot->graph(i)->setData(x, y_usage[i]);
    }
    for (int i = 0; i < data->getTempNum(); ++i) {
        y_temp[i].push_front(temp[i]);
        if (y_temp[i].size() > 3600)
            y_temp[i].pop_back();
        ui->tempPlot->graph(i)->setData(x, y_temp[i]);
    }
    for (int i = 0; i < data->getFanNum(); ++i) {
        y_fan[i].push_front(fan[i]);
        if (y_fan[i].size() > 3600)
            y_fan[i].pop_back();
        ui->fanPlot->graph(i)->setData(x, y_fan[i]);
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
    colorGenerator getColor;

    QVector<QString> names;

    data->getUsageNames(names);
    for (int i = 0; i < data->getUsageNum(); ++i) {
        ui->usagePlot->addGraph()->setName(names[i]);
        ui->usagePlot->graph(i)->setPen(getColor());
        y_usage.push_back(QVector<double>());
    }

    getColor.reset();
    data->getTempNames(names);
    for (int i = 0; i < data->getTempNum(); ++i) {
        ui->tempPlot->addGraph()->setName(names[i]);
        ui->tempPlot->graph(i)->setPen(getColor());
        y_temp.push_back(QVector<double>());
    }

    getColor.reset();
    data->getFanNames(names);
    for (int i = 0; i < data->getFanNum(); ++i) {
        ui->fanPlot->addGraph()->setName(names[i]);
        ui->fanPlot->graph(i)->setPen(getColor());
        y_fan.push_back(QVector<double>());
    }

    ui->usagePlot->xAxis->setRange(0, getTime());
    ui->usagePlot->yAxis->setLabel("Usage  (%)");
    ui->usagePlot->yAxis->setRange(0, 100);

    ui->tempPlot->xAxis->setRange(0, getTime());
    ui->tempPlot->yAxis->setLabel("Temperature  (deg C)");
    ui->tempPlot->yAxis->setRange(0, 100);

    ui->fanPlot->xAxis->setRange(0, getTime());
    ui->fanPlot->yAxis->setLabel("Fan Speed  (RPM)");
    ui->fanPlot->yAxis->setRange(0, 5000);

    addLegend(ui->usagePlot);
    addLegend(ui->tempPlot);
    addLegend(ui->fanPlot);

    connect(timer, SIGNAL(timeout()), this, SLOT(plot()));
    timer->start(1000);
}

void MainWindow::addLegend(QCustomPlot *plot) {
    static QCPLayoutGrid *layout = layouts;

    plot->xAxis->setTickLabels(false);
    plot->legend->setVisible(true);
    plot->plotLayout()->addElement(1, 0, layout);
    layout->setMargins(QMargins(5, 0, 5, 0));
    layout->addElement(0, 0, plot->legend);
    plot->legend->setIconTextPadding(0);
    plot->legend->setIconSize(QSize(20,20));
    plot->legend->setBorderPen(QPen(Qt::NoPen));
    plot->legend->setFillOrder(QCPLegend::foColumnsFirst);
    plot->plotLayout()->setRowStretchFactor(1, 0.001);

    if (layout - layouts >= 3)
        layout = layouts;
    else
        ++layout;
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

MainWindow::colorGenerator::colorGenerator() {
    reset();
}

QPen MainWindow::colorGenerator::operator()() {
    if (colors.size() == 0)
        reset();

    QPen color = colors.back();
    colors.pop_back();
    return color;
}

void MainWindow::colorGenerator::reset() {
    colors = {
        QPen(Qt::lightGray),
        QPen(Qt::darkCyan),
        QPen(Qt::darkMagenta),
        QPen(Qt::darkGray),
        QPen(Qt::darkYellow),
        QPen(Qt::darkGreen),
        QPen(Qt::darkBlue),
        QPen(Qt::darkRed),
        QPen(Qt::cyan),
        QPen(Qt::magenta),
        QPen(Qt::green),
        QPen(Qt::yellow),
        QPen(Qt::gray),
        QPen(Qt::black),
        QPen(Qt::red),
        QPen(Qt::blue)
    };
}
