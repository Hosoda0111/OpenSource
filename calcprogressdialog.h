#ifndef CALCPROGRESSDIALOG_H
#define CALCPROGRESSDIALOG_H

#include <QDialog>

class QProgressBar;
class QPushButton;
class QLabel;
class CalcWorker;
class QThread;

class CalcProgressDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CalcProgressDialog(const QString& labelText, int maxValue, QWidget* parent = nullptr);
    ~CalcProgressDialog();

    void startWorker(CalcWorker* worker);   // ワーカーを受け取り起動

private slots:
    void onProgress(int value);
    void onFinished();
    void onCanceled();
    void onCancelClicked();

private:
    QLabel*      m_label;
    QProgressBar* m_progressBar;
    QPushButton*  m_cancelButton;

    CalcWorker*  m_worker;
    QThread*     m_thread;
    int          m_max;
};

#endif // CALCPROGRESSDIALOG_H
