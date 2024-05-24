QT -= gui
QT += websockets

TEMPLATE = lib
DEFINES += LISTCOMPARISON_LIBRARY

CONFIG += c++17

SOURCES += \
	listcomparison.cpp \
	network/rest.cpp

HEADERS += \
	../../include/HEADERS_QT.h \
	ListComparison_global.h \
	listcomparison.h \
	network/rest.h

INCLUDEPATH += \
	$$PWD/../../include

# Default rules for deployment.
unix: target.path = /usr/lib
!isEmpty(target.path): INSTALLS += target


