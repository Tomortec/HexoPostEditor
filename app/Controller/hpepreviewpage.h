/**
 * @file hpepreviewpage.h
 * @brief This file is part of HPEController
 * @version 1.0.0
 * @date 2022-01-20
 * 
 * @author Tomortec (everything@tomortec.com)
 * @copyright Copyright © 2021 - 2022 Tomortec.
 * @website https://tomortec.com
 * @license GPL v3 (https://www.gnu.org/licenses/gpl-3.0.html)
*/

#ifndef HPEPREVIEWPAGE_H
#define HPEPREVIEWPAGE_H

#include <QWebEnginePage>

/**
 * @class HPEPreviewPage
 * @brief An HPEPreviewPage is a QWebEnginePage used to block invalid url.
 * @since 1.0.0
 * 
 * @ingroup controller
 * 
 * An HPEPreviewPage is a QWebEnginePage used to block invalid url.
 * By overriding the acceptNavigationRequest(), the web page can
 * only open url with 'qrc' or 'file' scheme or localhost for security.
 * For other URLs, the page will ask local browser to open it.
 * 
 * @see QWebEnginePage::acceptNavigationRequest()
*/
class HPEPreviewPage : public QWebEnginePage
{
    Q_OBJECT
public:

    /**
     * @brief Construct an HPEPreviewPage with parent
     * 
     * @param[in] parent 
    */
    explicit HPEPreviewPage(QObject *parent = nullptr);
    using QWebEnginePage::QWebEnginePage;

protected:

    /**
     * @brief Overrides QWebEnginePage::acceptNavigationRequest() to
     * block unsupported domain.
     * 
    */
    bool acceptNavigationRequest(const QUrl &url, NavigationType type, bool isMainFrame) override;
};

#endif // HPEPREVIEWPAGE_H
