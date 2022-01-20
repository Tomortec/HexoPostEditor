/**
 * @file hpesettings.cpp
 * @brief This file is part of HPEController
 * @version 1.0.0
 * @date 2022-01-20
 * 
 * @author Tomortec (everything@tomortec.com)
 * @copyright Copyright © 2021 - 2022 Tomortec.
 * @website https://tomortec.com
 * @license GPL v3 (https://www.gnu.org/licenses/gpl-3.0.html)
*/

#include "hpesettings.h"

//Initializtion of Singleton
HPESettings* HPESettings::m_instance = new HPESettings();


HPESettings::HPESettings()
{
    HPE_DEFAULT_SETTINGS[QString("markdownField/font")] = QString("Monaco,18,-1,5,400,0,0,0,0,0,0,0,0,0,0,1,Regular");
    HPE_DEFAULT_SETTINGS[QString("window/sizeState")] = QString("MAXIMIZED");
    HPE_DEFAULT_SETTINGS[QString("window/previewWidth")] = 500;
    HPE_DEFAULT_SETTINGS[QString("basic/presetDir")] = QDir::homePath();
}

HPESettings* HPESettings::config()
{
    return m_instance;
}

QVariant HPESettings::value(const QString &key, const QVariant &defaultValue) const
{
    return QSettings::value(key,
            HPE_DEFAULT_SETTINGS.contains(key) ?
                                defaultValue : HPE_DEFAULT_SETTINGS[key]);
}

bool HPESettings::isDefaultValue(const QString &key) const
{
    return QSettings::value(key) == HPE_DEFAULT_SETTINGS[key];
}
