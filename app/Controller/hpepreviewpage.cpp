/**
 * @file hpepreviewpage.cpp
 * @brief This file is part of HPEController
 * @version 1.0.0
 * @date 2022-01-20
 * 
 * @author Tomortec (everything@tomortec.com)
 * @copyright Copyright © 2021 - 2022 Tomortec.
 * @website https://tomortec.com
 * @license GPL v3 (https://www.gnu.org/licenses/gpl-3.0.html)
*/

#include "hpepreviewpage.h"

#include <QDesktopServices>

HPEPreviewPage::HPEPreviewPage(QObject *parent)
    : QWebEnginePage{parent} { }

bool HPEPreviewPage::acceptNavigationRequest(const QUrl &url,
                                          QWebEnginePage::NavigationType /*type*/,
                                          bool /*isMainFrame*/)
{
    // Only allow qrc:/index.html and localhost
    if (url.scheme() == QString("qrc") || url.scheme() == QString("file") || url.host() == "localhost")
        return true;
    QDesktopServices::openUrl(url);
    return false;
}
