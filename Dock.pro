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


SOURCES += main.cpp\
        mainwindow.cpp \
    vocabmatch.cpp \
    imageworker.cpp

HEADERS  += mainwindow.h \
    imageworker.h \
    match.h

FORMS    += mainwindow.ui

RESOURCES += \
    icon.qrc




win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../example/lib/release/ -lANN_char
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../example/lib/debug/ -lANN_char
else:unix: LIBS += -L$$PWD/../example/lib/ -lANN_char

INCLUDEPATH += $$PWD/../example/lib/ann_1.1_char/include/
DEPENDPATH += $$PWD/../example/lib/ann_1.1_char/include/

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../example/lib/release/libANN_char.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../example/lib/debug/libANN_char.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../example/lib/release/ANN_char.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../example/lib/debug/ANN_char.lib
else:unix: PRE_TARGETDEPS += $$PWD/../example/lib/libANN_char.a



win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../example/lib/release/ -lANN
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../example/lib/debug/ -lANN
else:unix: LIBS += -L$$PWD/../example/lib/ -lANN

INCLUDEPATH += $$PWD/../example/lib/ann_1.1/include/ANN
DEPENDPATH += $$PWD/../example/lib/ann_1.1/include/ANN

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../example/lib/release/libANN.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../example/lib/debug/libANN.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../example/lib/release/ANN.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../example/lib/debug/ANN.lib
else:unix: PRE_TARGETDEPS += $$PWD/../example/lib/libANN.a

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../example/lib/release/ -limage
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../example/lib/debug/ -limage
else:unix: LIBS += -L$$PWD/../example/lib/ -limage

INCLUDEPATH += $$PWD/../example/lib/imagelib
DEPENDPATH += $$PWD/../example/lib/imagelib

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../example/lib/release/libimage.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../example/lib/debug/libimage.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../example/lib/release/image.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../example/lib/debug/image.lib
else:unix: PRE_TARGETDEPS += $$PWD/../example/lib/libimage.a

INCLUDEPATH += $$PWD/../example/lib/zlib/include
DEPENDPATH += $$PWD/../example/lib/zlib/include



win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../example/release/ -lvocab
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../example/debug/ -lvocab
else:unix: LIBS += -L$$PWD/../example/ -lvocab -lANN_char -lANN -limage -lz -O3 -fopenmp

INCLUDEPATH += $$PWD/../example/VocabLib
DEPENDPATH += $$PWD/../example/VocabLib

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../example/release/libvocab.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../example/debug/libvocab.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../example/release/vocab.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../example/debug/vocab.lib
else:unix: PRE_TARGETDEPS += $$PWD/../example/VocabLib/libvocab.a
