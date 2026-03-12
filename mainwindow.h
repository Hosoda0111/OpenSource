#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class TaskManager;
class QProgressDialog;
class QLabel;
class QPushButton;
class QLineEdit;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onSelectWallpaper();    // 画像選択＋壁紙変更
    void onTaskFinished(bool success);

    void onAddClicked();
    void onSubClicked();
    void onMulClicked();
    void onDivClicked();

private:
    void setupUi();
    void startCalculation(int opType); // 0:+ 1:- 2:* 3:/

private:
    TaskManager*    m_taskManager;
    QProgressDialog* m_progressDialog;

    QLabel*     m_statusLabel;
    QPushButton* m_wallpaperButton;

    QLineEdit*  m_timesEdit;
    QPushButton* m_addButton;
    QPushButton* m_subButton;
    QPushButton* m_mulButton;
    QPushButton* m_divButton;
};

#endif // MAINWINDOW_H
