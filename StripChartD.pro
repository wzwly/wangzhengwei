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
    label/dlg.cpp \
    label/softkey.cpp \
    label/configset.cpp \
    ui/mainframe.cpp \
    ui/basepage.cpp \
    ui/autopage.cpp \
    ui/filepage.cpp \
    ui/drillparam.cpp \
    ui/sysparam.cpp \
    ui/syspage.cpp \
    core/qsysdata.cpp \
    core/dxfreader.cpp \
    core/drawthread.cpp \
    core/cmddef.cpp \
    core/parseconfig.cpp \
    core/serial.cpp \
    label/listbox.cpp \ # core/xtfreader.cpp \
# ui/editview.cpp \
    label/codeedit.cpp \
    ui/editview.cpp
HEADERS += ghead.h \
    label/item.h \
    label/button.h \
    label/dlg.h \
    label/softkey.h \
    label/configset.h \
    ui/mainframe.h \
    ui/basepage.h \
    ui/autopage.h \
    ui/filepage.h \
    ui/drillparam.h \
    ui/sysparam.h \
    ui/syspage.h \
    ui/addrdef.h \
    core/qsysdata.h \
    core/typedef.h \
    core/dxfreader.h \
    core/cmddef.h \
    core/drawthread.h \
    core/parseconfig.h \
    core/serial.h \
    core/typedef.h \
    label/listbox.h \
    label/label.h \ # core/xtfreader.h \
# ui/editview.h \
    label/codeedit.h \
    ui/editview.h
DEFINES += 
OTHER_FILES += 备注.txt \
    PR.info
