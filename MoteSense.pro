# -------------------------------------------------
# Project created by QtCreator 2010-04-02T13:58:39
# -------------------------------------------------
QT += network \
    sql
TARGET = MoteSense
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    packethandler.cpp \
    motereading.cpp \
    data_plot.cpp \
    graphwindow.cpp \
    datafilter.cpp \
    filterwindow.cpp \
    filtered_data_plot.cpp
HEADERS += mainwindow.h \
    packethandler.h \
    motereading.h \
    data_plot.h \
    graphwindow.h \
    datafilter.h \
    filterwindow.h \
    filtered_data_plot.h
FORMS += mainwindow.ui
INCLUDEPATH += /usr/local/qwt-5.2.0/include/
LIBS += /usr/local/qwt-5.2.0/lib/libqwt.so.5.2.0
