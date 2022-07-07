QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    wplot/axis.cpp \
    wplot/cursors.cpp \
    wplot/plotter.cpp \
    wplot/sdata.cpp \
    wplot/state/hzoomstate.cpp \
    wplot/state/normalstate.cpp \
    wplot/state/state.cpp \
    wplot/state/vzoomstate.cpp \
    wplot/state/zoomstate.cpp \
    wplot/wplot.cpp

HEADERS += \
    mainwindow.h \
    wplot/axis.h \
    wplot/cursors.h \
    wplot/plotter.h \
    wplot/sdata.h \
    wplot/state/hzoomstate.h \
    wplot/state/normalstate.h \
    wplot/state/state.h \
    wplot/state/vzoomstate.h \
    wplot/state/zoomstate.h \
    wplot/wplot.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    wplot/resource.qrc

DISTFILES += \
    wplot/png/icons8-zoom-in-24.png \
    wplot/png/icons8-zoom-in-30.png \
    wplot/png/icons8-zoom-out-24.png
