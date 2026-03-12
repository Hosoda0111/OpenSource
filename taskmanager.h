#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include <QObject>
#include <QString>

class TaskManager : public QObject
{
    Q_OBJECT
public:
    explicit TaskManager(QObject* parent = nullptr);

public slots:
    void startWallpaperChange(const QString& imagePath);

signals:
    void taskFinished(bool success);

private:
    void changeWallpaper(const QString& imagePath);
};

#endif // TASKMANAGER_H
