#include "taskmanager.h"
#include <QTimer>
#include <QDebug>
#include <Windows.h>

TaskManager::TaskManager(QObject* parent) : QObject(parent)
{
}

void TaskManager::startWallpaperChange(const QString& imagePath)
{
    // 3秒遅延してから壁紙変更（非同期の疑似処理）
    QTimer::singleShot(3000, this, [this, imagePath]() {
        changeWallpaper(imagePath);
    });
}

void TaskManager::changeWallpaper(const QString& imagePath)
{
    bool ret = SystemParametersInfoW(
        SPI_SETDESKWALLPAPER,
        0,
        (void*)imagePath.utf16(),
        SPIF_SENDWININICHANGE | SPIF_UPDATEINIFILE
        );

    emit taskFinished(ret);
    qDebug() << "Wallpaper changed:" << ret;
}
