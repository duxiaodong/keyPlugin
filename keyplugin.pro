
TEMPLATE = lib
CONFIG +=  plugin +=release
#编程成动态库
PLUGIN_FOR = qt
PLUGIN_TYPE = kbddrivers

TARGET = optkeypad

HEADERS = \
    MyKeyHandler.h \
    keypadDriverPlugin.h

SOURCES = \
    MyKeyHandler.cpp \
    keypadDriverPlugin.cpp

