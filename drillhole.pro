# -------------------------------------------------
# Project created by QtCreator 2012-06-30T20:16:46
# -------------------------------------------------
QT += core \
    gui
TARGET = drillhole
TEMPLATE = app
SOURCES += main.cpp \
    label/item.cpp \
    label/button.cpp \
    ui/mainframe.cpp \
    ui/basepage.cpp \
    ui/autopage.cpp \
    ui/filepage.cpp \
    ui/drillparam.cpp \
    ui/sysparam.cpp \
    ui/syspage.cpp \
    label/dlg.cpp \
    label/softkey.cpp \
    label/configset.cpp \
    core/qsysdata.cpp \
    core/dxfreader.cpp \
    core/serial.cpp \
    core/drawthread.cpp \
    core/cmddef.cpp \
    core/parseconfig.cpp
HEADERS += ghead.h \
    label/item.h \
    label/button.h \
    ui/mainframe.h \
    ui/basepage.h \
    ui/autopage.h \
    ui/filepage.h \
    ui/drillparam.h \
    ui/sysparam.h \
    ui/syspage.h \
    label/dlg.h \
    label/softkey.h \
    label/configset.h \
    core/qsysdata.h \
    core/typedef.h \
    core/dxfreader.h \
    core/serial.h \
    core/cmddef.h \
    core/drawthread.h \
    ui/addrdef.h \
    core/parseconfig.h
DEFINES += PC_BEBUG
