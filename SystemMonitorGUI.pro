QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Common/common.cpp \
    Common/tdbloger.cpp \
    Common/thttpquery.cpp \
    about.cpp \
    coupons.cpp \
    levelgauge.cpp \
    logs.cpp \
    main.cpp \
    mainwindow.cpp \
    subwindow.cpp \
    tconfig.cpp \
    tdbquery.cpp

HEADERS += \
    Common/common.h \
    Common/tdbloger.h \
    Common/thttpquery.h \
    about.h \
    coupons.h \
    levelgauge.h \
    logs.h \
    mainwindow.h \
    subwindow.h \
    tconfig.h \
    tdbquery.h

FORMS += \
    about.ui \
    coupons.ui \
    levelgauge.ui \
    logs.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resurses.qrc

RC_ICONS += res/SystemMonitorGUI64.ico

DISTFILES += \
    ToDo
