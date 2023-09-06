QT += gui

TEMPLATE = lib
DEFINES += QT_LIBRARY_LIBRARY

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    qt_library.cpp

HEADERS += \
    qt_library_global.h \
    qt_library.h

TRANSLATIONS += \
    qt_library_fr_FR.ts

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
