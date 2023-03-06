#-------------------------------------------------
#
# Project created by QtCreator 2023-02-05T15:04:23
#
#-------------------------------------------------

QT       += core gui
QT       += widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OS
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
    memory/memory.cpp \
    process/pcb.cpp \
    process/processdialog.cpp \
    process/processtab.cpp \
    main.cpp \
    mainwindow.cpp \
    file/file.cpp \
    file/filecontext.cpp \
    file/filecreate.cpp \
    file/fileproperty.cpp \
    file/foldercreate.cpp \
    file/myfilepro.cpp \
    file/spaceexband.cpp \
    disk/confilespacedetail.cpp \
    disk/disk.cpp \
    disk/fatfilespacedetail.cpp \
    disk/freeblocktable.cpp \
    disk/indexfilespacedetail.cpp \
    disk/showfileseekoder.cpp \
    equipment/dequip.cpp \
    equipment/myprocess.cpp \
    equipment/request.cpp \
    equipment/widget.cpp


HEADERS += \
    disk/disk.h \
    file/file.h \
    memory/memory.h \
    process/pcb.h \
    process/processdialog.h \
    process/processtab.h \
    mainwindow.h \
    file/file.h \
    file/filecontext.h \
    file/filecreate.h \
    file/fileproperty.h \
    file/foldercreate.h \
    file/myfilepro.h \
    file/spaceexband.h \
    disk/confilespacedetail.h \
    disk/disk.h \
    disk/fatfilespacedetail.h \
    disk/freeblocktable.h \
    disk/indexfilespacedetail.h \
    disk/showfileseekoder.h \
    equipment/dequip.h \
    equipment/myprocess.h \
    equipment/request.h \
    equipment/widget.h



FORMS += \
    disk/disk.ui \
    file/file.ui \
    memory/memory.ui \
    process/processdialog.ui \
    process/processtab.ui \
    mainwindow.ui \
    file/file.ui \
    file/filecontext.ui \
    file/filecreate.ui \
    file/fileproperty.ui \
    file/foldercreate.ui \
    file/spaceexband.ui \
    disk/confilespacedetail.ui \
    disk/disk.ui \
    disk/fatfilespacedetail.ui \
    disk/freeblocktable.ui \
    disk/indexfilespacedetail.ui \
    disk/showfileseekoder.ui \
    equipment/dequip.ui \
    equipment/myprocess.ui \
    equipment/request.ui \
    equipment/widget.ui



# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icon.qrc \
    pics.qrc \
    pics.qrc

SUBDIRS += \
    OS.pro

DISTFILES += \
    pics/tubiao.jpg \
    README.md \
    LICENSE
