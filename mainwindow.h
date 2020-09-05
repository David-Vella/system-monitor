#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "datafetcher.h"

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
    Ui::MainWindow *ui;
    QTimer *timer;
    DataFetcher *cputemp;

    QVector<double> x;
    QVector<double> y;
};

#endif // MAINWINDOW_H
