QT -= gui

CONFIG += c++17 console
CONFIG -= app_bundle

VERSION = 1.0.1
DEFINES += VERSION=\\\"$$VERSION\\\"

HEADERS += \
	../include/HEADERS_QT.h \
	core/baselogic.h

SOURCES += \
        core/baselogic.cpp \
        main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

unix:!macx: LIBS += -L$$OUT_PWD/../lib/ListComparison/ -lListComparison

INCLUDEPATH += \
	$$PWD/../lib/ListComparison \
	$$PWD/../include

DEPENDPATH += \
	$$PWD/../lib/ListComparison
