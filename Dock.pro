#-------------------------------------------------
#
# Project created by QtCreator 2017-01-06T20:42:49
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Dock
TEMPLATE = app


DEFINES += QT_DEPRECATED_WARNINGS



INCLUDEPATH += /home/mateusz/example/lib/ann_1.1/include/ANN \
 /home/mateusz/example/lib/ann_1.1_char/include/ANN \
 /home/mateusz/example/lib/zlib/include \
/home/mateusz/example/lib/imagelib

SOURCES += main.cpp\
        mainwindow.cpp \
    vocabmatch.cpp \
    imageworker.cpp

HEADERS  += mainwindow.h \
    imageworker.h

FORMS    += mainwindow.ui

RESOURCES += \
    icon.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../example/VocabLib/release/ -lvocab
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../example/VocabLib/debug/ -lvocab
else:unix: LIBS += -L$$PWD/../example/VocabLib/ -lvocab -lz

INCLUDEPATH += $$PWD/../example/VocabLib
DEPENDPATH += $$PWD/../example/VocabLib

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../example/VocabLib/release/libvocab.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../example/VocabLib/debug/libvocab.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../example/VocabLib/release/vocab.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../example/VocabLib/debug/vocab.lib
else:unix: PRE_TARGETDEPS += $$PWD/../example/VocabLib/libvocab.a
