QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
greaterThan(QT_MAJOR_VERSION, 5): QT += statemachine

CONFIG += c++14

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    rangesetter.cpp \
    sortwidget.cpp \
    sortwidgeteventhandler.cpp \
    swapmanager.cpp

HEADERS += \
    graphicsview.h \
    mainwidget.h \
    qgraphicsrectwidget.h \
    rangesetter.h \
    sortwidget.h \
    sortwidgeteventhandler.h \
    swapanimation.h \
    swapmanager.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
