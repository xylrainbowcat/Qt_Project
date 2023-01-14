QT       += core gui
QT       += widgets
QT       += multimedia

RC_ICONS += logo.ico

greaterThan(QT_MAJOR_VERSION, 5): QT += core5compat
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    gamewindow.cpp \
    generatemaze.cpp \
    heap.cpp \
    homewindow.cpp \
    main.cpp \
    selectwindow.cpp \
    startwindow.cpp

HEADERS += \
    gamewindow.h \
    heap.h \
    homewindow.h \
    selectwindow.h \
    startwindow.h

FORMS += \
    gamewindow.ui \
    homewindow.ui \
    selectwindow.ui \
    startwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
