TEMPLATE = app
QT += core gui widgets network

HEADERS += \
    include/MainWindow.h \
    include/ui_MainWindow.h

FORMS += \
    include/MainWindow.ui

SOURCES += \
    src/Client.cpp \
    src/MainWindow.cpp

INCLUDEPATH += include
