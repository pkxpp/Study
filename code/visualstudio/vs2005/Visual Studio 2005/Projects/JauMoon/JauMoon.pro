#-------------------------------------------------
#
# Project created by QtCreator 2012-10-29T15:42:55
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = JauMoon
TEMPLATE = app


SOURCES += main.cpp\
        jaumoon.cpp \
    str_operate.cpp \
    db_operate.cpp \
    interface.cpp

HEADERS  += jaumoon.h \
    str_operate.h \
    db_operate.h \
    interface.h

FORMS    += jaumoon.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/build_windows/lib/mysql/release/ -llibmysqlD
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/build_windows/lib/mysql/debug/ -llibmysqlD
else:symbian: LIBS += -llibmysqlD
else:unix: LIBS += -L$$PWD/build_windows/lib/mysql/ -llibmysqlD

INCLUDEPATH += $$PWD/build_windows/lib/mysql/debug
DEPENDPATH += $$PWD/build_windows/lib/mysql/debug

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/build_windows/lib/bdb/release/ -llibdb45d
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/build_windows/lib/bdb/debug/ -llibdb45d
else:symbian: LIBS += -llibdb45d
else:unix: LIBS += -L$$PWD/build_windows/lib/bdb/ -llibdb45d

INCLUDEPATH += $$PWD/build_windows/lib/bdb/debug
DEPENDPATH += $$PWD/build_windows/lib/bdb/debug
