#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "datafetcher.h"
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void addPoint();

private:
    class colorGenerator {
    public:
        colorGenerator();
        QPen operator()();
        void reset();

    private:
        QVector<QPen> colors;
    };

    void setupPlots();
    void addLegend(QCustomPlot *plot);

    Ui::MainWindow *ui;
    QTimer *timer;
    DataFetcher *data;

    QCPLayoutGrid *layouts;

    QVector<double> x;
    QVector<QVector<double>> y_usage;
    QVector<QVector<double>> y_temp;
    QVector<QVector<double>> y_fan;
};

#endif // MAINWINDOW_H
