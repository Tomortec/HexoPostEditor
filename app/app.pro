QT       += core gui
QT       += core5compat
QT       += webenginewidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

include(ThirdParty/QsLog/QsLog.pri)

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Dialogs/hpecodesdialogform.cpp \
    Dialogs/hpedialog.cpp \
    Dialogs/hpefilecreatorform.cpp \
    Dialogs/hpefileselectorform.cpp \
    Dialogs/hpeheadingdialogform.cpp \
    Dialogs/hpeimagedialogform.cpp \
    Dialogs/hpelinkdialogform.cpp \
    Dialogs/hpeprocessingdialog.cpp \
    Dialogs/hpestartupdialog.cpp \
    Dialogs/hpetabledialogform.cpp \
    Terminal/qterminalprocess.cpp \
    Terminal/qterminalwidget.cpp \
    hpeconvertedmarkdownpreview.cpp \
    hpedocument.cpp \
    hpehexocontroller.cpp \
    hpelinenumberarea.cpp \
    hpelocalresources.cpp \
    hpemarkdowneditor.cpp \
    hpeprettyframe.cpp \
    hpepreviewpage.cpp \
    hpesettings.cpp \
    hpesplitter.cpp \
    hpesyntaxhighlighter.cpp \
    main.cpp \
    hpemainwindow.cpp

HEADERS += \
    Dialogs/hpecodesdialogform.h \
    Dialogs/hpedialog.h \
    Dialogs/hpefilecreatorform.h \
    Dialogs/hpefileselectorform.h \
    Dialogs/hpeheadingdialogform.h \
    Dialogs/hpeimagedialogform.h \
    Dialogs/hpelinkdialogform.h \
    Dialogs/hpeprocessingdialog.h \
    Dialogs/hpestartupdialog.h \
    Dialogs/hpetabledialogform.h \
    Terminal/qterminalprocess.h \
    Terminal/qterminalwidget.h \
    hpeconvertedmarkdownpreview.h \
    hpedocument.h \
    hpehexocontroller.h \
    hpelinenumberarea.h \
    hpelocalresources.h \
    hpemainwindow.h \
    hpemarkdowneditor.h \
    hpeprettyframe.h \
    hpepreviewpage.h \
    hpesettings.h \
    hpesplitter.h \
    hpesyntaxhighlighter.h

FORMS += \
    Dialogs/hpecodesdialogform.ui \
    Dialogs/hpedialog.ui \
    Dialogs/hpefilecreatorform.ui \
    Dialogs/hpefileselectorform.ui \
    Dialogs/hpeheadingdialogform.ui \
    Dialogs/hpeimagedialogform.ui \
    Dialogs/hpelinkdialogform.ui \
    Dialogs/hpeprocessingdialog.ui \
    Dialogs/hpestartupdialog.ui \
    Dialogs/hpetabledialogform.ui \
    hpemainwindow.ui

TRANSLATIONS += \
    HexoPostEditor_zh_CN.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    HPEResources.qrc

#DISTFILES += \
#    resources/3rdParty/markdown.css \
#    resources/3rdParty/marked.min.js \
#    resources/index.html
#    resources/style.qss

DISTFILES += \
    resources/index.html

# copy local resource files
copydata.commands = $(COPY_DIR) $$PWD/resources $$OUT_PWD
first.depends = $(first) copydata
export(first.depends)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += first copydata
