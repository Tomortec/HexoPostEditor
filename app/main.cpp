#include "hpemainwindow.h"

#include <QCoreApplication>
#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include "Dialogs/hpestartupdialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //Core application settings
    QCoreApplication::setOrganizationName("Tomortec");
    QCoreApplication::setOrganizationDomain("tomortec.com");
    QCoreApplication::setApplicationName("Hexo Posts Editor");
    QCoreApplication::setApplicationVersion("1.0.0");

    //initialize file logger
    QsLogging::Logger::instance().addDestination(
        QsLogging::DestinationFactory::MakeFileDestination(
        QDir().absoluteFilePath("log"),         //CWD
        QsLogging::EnableLogRotation,   //split a large file into several ones
        QsLogging::MaxSizeBytes(512*1024),
        QsLogging::MaxOldLogCount(5)    //how many log files can stay
        )
    );

    //Mounting Translator
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "HexoPostEditor_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    HPEStartupDialog startupDialog;
    if(!startupDialog.exec())
        return 1;

    //Launch Window
    HPEMainWindow w;
    w.show();
    w.activateWindow();
    w.openFile(startupDialog.getTargetFile());

    return a.exec();
}
