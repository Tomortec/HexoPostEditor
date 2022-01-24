/**
 * @file hpelocalresources.cpp
 * @brief This file is part of HPEController
 * @version 1.0.0
 * @date 2022-01-20
 * 
 * @author Tomortec (everything@tomortec.com)
 * @copyright Copyright © 2021 - 2022 Tomortec.
 * @website https://tomortec.com
 * @license GPL v3 (https://www.gnu.org/licenses/gpl-3.0.html)
*/

#include "hpelocalresources.h"

#include <QDir>
#include <QUrl>

QString HPELocalResources::getLocalPathWithName(const QString &name)
{
    QDir localResourcesDir = QDir(QCoreApplication::applicationDirPath());
#ifdef Q_OS_MACOS
    //for macOS, the executable locates in the app package
    while(localResourcesDir.dirName() != QString("%1.app").arg(HPE_EXECUTABLE_NAME)
          && localResourcesDir.cdUp());
    localResourcesDir.cdUp();
#endif
    localResourcesDir.cd(HPE_LOCAL_RESOURCE_DIR_NAME);

    return localResourcesDir.absoluteFilePath(name);
}

QUrl HPELocalResources::getLocalURLWithName(const QString &name)
{
    return QUrl::fromLocalFile(HPELocalResources::getLocalPathWithName(name));
}
