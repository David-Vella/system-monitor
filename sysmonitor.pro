QT += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

SOURCES += \
    src/datafetcher.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    lib/qcustomplot.cpp

HEADERS += \
    src/datafetcher.h \
    src/mainwindow.h \
    lib/qcustomplot.h

FORMS += \
    src/mainwindow.xml
