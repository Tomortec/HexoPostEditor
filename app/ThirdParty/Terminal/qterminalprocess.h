#ifndef QTERMINALPROCESS_H
#define QTERMINALPROCESS_H

#include <QProcess>
#include <QMap>

class QProcessPathFinder;

class QTerminalProcess : public QProcess
{
    Q_OBJECT
public:
    QTerminalProcess(QObject* parent=nullptr);

private:
    QMap<QString, QString> m_executablePaths;

public:
    QMap<QString, QString> getExecutablePaths() const;

private:
    void searchExecutablePath();
    QStringList commandsFilter(const QString&);

public slots:
    void onCommandEnter(const QString&);

signals:
    void commandNotFound(const QString&);
    void filteredCommandCompleted();
    void workingDirChanged(const QString&);
    void executablePathsChanged();
};

#endif // QTERMINALPROCESS_H
