QT += widgets
CONFIG += c++17
TARGET = FanManager
TEMPLATE = app
SOURCES += main.cpp \
           fanatdatabase.cpp \
           fanatsworldcup.cpp \
           mainviolators.cpp \
           mainwindow.cpp
HEADERS += absfanat.h \
           fanatdatabase.h \
           fanatsworldcup.h \
           mainviolators.h \
           mainwindow.h
FORMS += mainwindow.ui
DISTFILES += \
    in1 \
    in2
