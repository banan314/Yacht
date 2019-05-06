include(Yacht.pri)

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += core gui


SOURCES += \
    main.cpp

target.path = $$PWD/openglwindow
INSTALLS += target

INCLUDEPATH += /media/Windows/ext4/Qt/5.12.2/Src/qtbase/include
