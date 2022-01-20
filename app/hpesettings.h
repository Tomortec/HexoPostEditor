/**
 * @file hpesettings.h
 * @brief This file is part of HPEController
 * @version 1.0.0
 * @date 2022-01-20
 * 
 * @author Tomortec (everything@tomortec.com)
 * @copyright Copyright © 2021 - 2022 Tomortec.
 * @website https://tomortec.com
 * @license GPL v3 (https://www.gnu.org/licenses/gpl-3.0.html)
*/

#ifndef HPESETTINGS_H
#define HPESETTINGS_H

#include <QSettings>
#include <QMap>
#include <QDir>

/**
 * @class HPESettings
 * @brief A singleton class used to store user's settings.
 * @since 1.0.0
 * 
 * @ingroup controller
 * @ingroup assets
 * 
 * HPESetting is a singleton class derives from QSettings.
 * Use config() to get the singleton instance.
 * Deriving from QSettings, this instance also
 * use value() to access stored settings and use setValue() to store settings.
 * 
 * @attention Use slash '/' to show the hierarchy.
 * 
 * For example,
 * @code
 *      // get value
 *      QVariant value = HPESettings::config()->value("foo/bar");
 *      // set value
 *      HPESettings::config()->setValue("foo/bar", value);
 * @endcode
 * 
 * @see QSettings::setValue()
*/
class HPESettings : public QSettings
{
//public:
//    explicit HPESettings(QObject *parent = nullptr);

private:

    /**
     * @brief A PRIVATE constructor used to define default settings
    */
    HPESettings();

    /**
     * @brief A static singleton instance
     * 
    */
    static HPESettings* m_instance;

    /**
     * @brief Holds all runtime defaults.
     * This property should be written only during initialization.
     * 
     * @attention This property should be written only during initialization.
    */
    QMap<QString, QVariant> HPE_DEFAULT_SETTINGS;

public:
    /**
     * @brief Returns the singleton instance of HPESettings
     * 
     * @return Singleton instance
    */
    static HPESettings* config();

public:

    /**
     * @brief Overrides QSettings::value() to make HPE_DEFAULT_SETTINGS has higher priority.    
     * Note that HPESettings::HPE_DEFAULT_SETTINGS has higher priority over defaultValue.
     * By default, the defaultValue is QVariant()
     * 
     * @attention This function's defaultValue is just used for overriding.
     * Actually, \l HPESettings::HPE_DEFAULT_SETTINGS has higher priority over defaultValue.
     * 
     * @param key Should has slash '/' to show hierarchy
     * @param defaultValue 
     * @return Stored setting value as QVariant
     * 
     * @see HPESettings::HPE_DEFAULT_SETTINGS
    */
    QVariant value(const QString &key, const QVariant &defaultValue = QVariant()) const;

    /**
     * @brief Judges if the current stored value is the same as that stored
     * in HPE_DEFAULT_SETTINGS
     * 
     * @param key Should has slash '/' to show hierarchy
     * @return true if the stored setting equals to its default (defined in HPESettings::HPE_DEFAULT_SETTINGS),
     * false otherwise.
    */
    bool isDefaultValue(const QString &key) const;
};

#endif // HPESETTINGS_H
