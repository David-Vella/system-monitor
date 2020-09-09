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

    static constexpr const int VECTOR_MAX_LENGTH = 3600;
    static constexpr const int SELECT_ICON_SIZE = 12;

private slots:
    void plot();

private:
    class ColorGenerator {
    public:
        ColorGenerator();
        QColor operator()();
        void reset();

    private:
        QVector<QColor> colors;
    };

    void setupPlots();
    void setupSelect();
    void addLegend(QCustomPlot *plot);
    int getTime();

    void clearVector(QVector<QListWidgetItem*> vec);

    Ui::MainWindow *ui;
    QTimer *timer;
    DataFetcher *data;

    QVector<QListWidgetItem*> usageItems;
    QVector<QListWidgetItem*> tempItems;
    QVector<QListWidgetItem*> fanItems;

    QVector<double> x;

    QList<QVector<double>> yUsage;
    QList<QVector<double>> yTemp;
    QList<QVector<double>> yFan;
};

#endif // MAINWINDOW_H
