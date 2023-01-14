QT       += core gui
QT       += sql

RC_ICONS += logo.ico

greaterThan(QT_MAJOR_VERSION, 5): QT += core5compat
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    changepassword.cpp \
    main.cpp \
    loginwindow.cpp \
    modifycourse.cpp \
    modifystudent.cpp \
    modifyteacher.cpp \
    reset.cpp \
    statics.cpp \
    studentwindow.cpp \
    teacherwindow.cpp

HEADERS += \
    changepassword.h \
    globel.h \
    loginwindow.h \
    modifycourse.h \
    modifystudent.h \
    modifyteacher.h \
    reset.h \
    statics.h \
    studentwindow.h \
    teacherwindow.h

FORMS += \
    changepassword.ui \
    loginwindow.ui \
    modifycourse.ui \
    modifystudent.ui \
    modifyteacher.ui \
    reset.ui \
    statics.ui \
    studentwindow.ui \
    teacherwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
