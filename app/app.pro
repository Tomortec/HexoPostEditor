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
    Dialogs/hpeaboutdialog.cpp \
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
    ThirdParty/Terminal/qterminalprocess.cpp \
    ThirdParty/Terminal/qterminalwidget.cpp \
    Editor/hpeconvertedmarkdownpreview.cpp \
    Controller/hpedocument.cpp \
    Controller/hpehexocontroller.cpp \
    Editor/hpelinenumberarea.cpp \
    Controller/hpelocalresources.cpp \
    Editor/hpemarkdowneditor.cpp \
    Frame/hpeprettyframe.cpp \
    Controller/hpepreviewpage.cpp \
    Controller/hpesettings.cpp \
    Frame/hpesplitter.cpp \
    Editor/hpesyntaxhighlighter.cpp \
    main.cpp \
    hpemainwindow.cpp

HEADERS += \
    Dialogs/hpeaboutdialog.h \
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
    ThirdParty/Terminal/qterminalprocess.h \
    ThirdParty/Terminal/qterminalwidget.h \
    Editor/hpeconvertedmarkdownpreview.h \
    Controller/hpedocument.h \
    Controller/hpehexocontroller.h \
    Editor/hpelinenumberarea.h \
    Controller/hpelocalresources.h \
    hpemainwindow.h \
    Editor/hpemarkdowneditor.h \
    Frame/hpeprettyframe.h \
    Controller/hpepreviewpage.h \
    Controller/hpesettings.h \
    Frame/hpesplitter.h \
    Editor/hpesyntaxhighlighter.h

FORMS += \
    Dialogs/hpeaboutdialog.ui \
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
