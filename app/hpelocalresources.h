/**
 * @file hpelocalresources.h
 * @brief This file is part of HPEController
 * @version 1.0.0
 * @date 2022-01-20
 * 
 * @author Tomortec (everything@tomortec.com)
 * @copyright Copyright © 2021 - 2022 Tomortec.
 * @website https://tomortec.com
 * @license GPL v3 (https://www.gnu.org/licenses/gpl-3.0.html)
*/

#ifndef HPELOCALRESOURCES_H
#define HPELOCALRESOURCES_H

#include <QFileInfo>
#include <QCoreApplication>

#define HPE_EXECUTABLE_NAME QFileInfo(QCoreApplication::applicationFilePath()).fileName()
#define HPE_LOCAL_RESOURCE_DIR_NAME "resources"

/**
 * @class HPELocalResources
 * @brief A static class used to get the resouces' paths at runtime instead of qrc.
 * @since 1.0.0
 * 
 * @ingroup controller
 * @ingroup assets
 * 
 * An HPELocalResouces is a static class used to get the resouces' paths
 * at runtime instead of qrc.
 * 
 * For HPE, its package folder might be something as:
 * @code
 *      // Windows
 *      // Hexo Post Editor
 *      // |--> executable
 *      // |--> resources
 * 
 *      // MacOS
 *      // Hexo Post Editor
 *      // |--> resources
 *      // |--> app
 *      // |    --> --> --> executable
 * @endcode
 * 
 * The function of HPELocalResources is to get the absolute path (or URL) of
 * resource by using getLocalPathWithName() or getLocalURLWithName().
*/
class HPELocalResources
{
public:

    /**
     * @brief Returns the local resource's absolute path by passing its name or relative path.
     * Note that this function doesn't ensure the path's correction
     * and the resource's existence!
     * 
     * @attention This function doesn't ensure the path's correction
     * and the resource's existence.
     * 
     * @param name local resource's name or relative path
     * @return local resource's absolute path
    */
    static QString getLocalPathWithName(const QString& name);

    /**
     * @brief Returns the local resource's URL by passing its name or relative path.
     * Note that this function doesn't ensure the URL's correction
     * and the resource's existence!
     * The URL is always starting with 'file://' scheme.
     * 
     * @attention This function doesn't ensure the URL's correction
     * and the resource's existence.
     * 
     * @param name local resource's name or relative path
     * @return local resource's URL(starting with 'file://' scheme)
     * 
     * @see QUrl::fromLocalFile()
    */
    static QUrl    getLocalURLWithName(const QString& name);

private:
    HPELocalResources() = delete;
};

#endif // HPELOCALRESOURCES_H
