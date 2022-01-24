/**
 * @file main.cpp
 * @brief Entry of HPE
 * @version 1.0.0
 * @date 2022-01-22
 * 
 * @author Tomortec (everything@tomortec.com)
 * @copyright Copyright © 2021 - 2022 Tomortec.
 * @website https://tomortec.com
 * @license GPL v3 (https://www.gnu.org/licenses/gpl-3.0.html)
*/

/**
 * @details 
 * Hexo Post Editor is a markdown editor for writing Hexo posts more conveniently.
 * 
 * @par Features
 * 
 * -: HexoPostEditor can render Markdown files in real time, even if it uses Hexo's special syntax, such as Assets Folder、Front-matter, etc.
 * -: The rendered Markdown page supports codes highlighting and Latex
 * -: Supports Markdown syntax highlighting
 * -: Markdown syntax can be added through the GUI
 * -: Generate, Clean, Server, Depoly Hexo blog posts by one click
 * -: There is a built-in terminal through which Hexo commands can be executed
 * -: Multiple languages
*/

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
