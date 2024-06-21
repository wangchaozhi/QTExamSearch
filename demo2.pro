QT       += core gui
QT       += network
QT       += multimedia
QT       += sql



greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    mysqlmanager.cpp \
    newwindow.cpp \
    sqlitemanager.cpp

HEADERS += \
    mainwindow.h \
    mysqlmanager.h \
    newwindow.h \
    sqlitemanager.h

FORMS += \
    mainwindow.ui \
    newwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


INCLUDEPATH += D:/dev/mysql-5.7.36-winx64/include
LIBS += -LD:/dev/mysql-5.7.36-winx64/lib
LIBS += -llibmysql




RESOURCES += \
    resources.qrc
