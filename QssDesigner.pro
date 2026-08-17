QT += core gui widgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QssDesigner
TEMPLATE = app
CONFIG += c++11
CONFIG -= app_bundle

DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH += src

# MinGW/GCC 不认识 MSVC 的 execution_character_set pragma，屏蔽对应警告
gcc: QMAKE_CXXFLAGS += -Wno-unknown-pragmas

FORMS += \
    forms/mainwindow.ui

SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/widgetpalette.cpp \
    src/designitem.cpp \
    src/designscene.cpp \
    src/designview.cpp \
    src/dialogs.cpp \
    src/widgetdrag.cpp \
    src/propertypanel.cpp \
    src/stylemanager.cpp \
    src/qsseditor.cpp \
    src/previewwidget.cpp \
    src/qsst.cpp \
    src/qsshighlighter.cpp \
    src/uiwriter.cpp \
    src/projectwriter.cpp

HEADERS += \
    src/mainwindow.h \
    src/widgetpalette.h \
    src/designitem.h \
    src/designscene.h \
    src/designview.h \
    src/dialogs.h \
    src/widgetdrag.h \
    src/propertypanel.h \
    src/stylemanager.h \
    src/qsseditor.h \
    src/previewwidget.h \
    src/qsst.h \
    src/qsshighlighter.h \
    src/uiwriter.h \
    src/projectwriter.h

win32 {
    RC_ICONS = res/app.ico
    QMAKE_TARGET_PRODUCT = "QssDesigner"
    QMAKE_TARGET_DESCRIPTION = "Visual QSS designer with drag & drop layout editor"
}

!exists(res/app.ico): RC_ICONS =
