#include "calcprogressdialog.h"
#include "calcworker.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>
#include <QThread>
#include <QTimer>
#include <QApplication>

CalcProgressDialog::CalcProgressDialog(const QString& labelText, int maxValue, QWidget* parent)
    : QDialog(parent),
    m_label(new QLabel(labelText, this)),
    m_progressBar(new QProgressBar(this)),
    m_cancelButton(new QPushButton("中止", this)),
    m_worker(nullptr),
    m_thread(nullptr),
    m_max(maxValue)
{
    setWindowTitle("計算処理中...");

    // ★setModal(true)を削除 → 非モーダル！

    m_progressBar->setRange(0, maxValue);
    m_progressBar->setValue(0);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(m_label);
    layout->addWidget(m_progressBar);
    layout->addWidget(m_cancelButton);
    layout->setContentsMargins(20, 20, 20, 20);

    setFixedSize(380, 140);

    connect(m_cancelButton, &QPushButton::clicked,
            this, &CalcProgressDialog::onCancelClicked);
}

CalcProgressDialog::~CalcProgressDialog()
{
    if (m_thread && m_thread->isRunning()) {
        m_thread->quit();
        m_thread->wait(2000);
    }
}

void CalcProgressDialog::startWorker(CalcWorker* worker)
{
    m_worker = worker;
    m_thread = new QThread(this);
    m_worker->moveToThread(m_thread);

    // スレッド開始
    connect(m_thread, &QThread::started, m_worker, &CalcWorker::start);

    // ★修正：connect文を1行ずつ正しく記述
    connect(m_worker, &CalcWorker::progress, this, &CalcProgressDialog::onProgress);
    connect(m_worker, &CalcWorker::finished, this, &CalcProgressDialog::onFinished);
    connect(m_worker, &CalcWorker::canceled, this, &CalcProgressDialog::onCanceled);

    // スレッド自動終了
    connect(m_worker, &CalcWorker::finished, m_thread, &QThread::quit);
    connect(m_worker, &CalcWorker::canceled, m_thread, &QThread::quit);
    connect(m_thread, &QThread::finished, m_worker, &QObject::deleteLater);
    connect(m_thread, &QThread::finished, [this]() { this->close(); });

    m_thread->start();
    show();  // 非モーダル表示
}

void CalcProgressDialog::onProgress(int value)
{
    m_progressBar->setValue(value);
    m_label->setText(QString("%1 (%2/%3) %4%")
                         .arg(m_label->text().split("...").first())
                         .arg(value)
                         .arg(m_max)
                         .arg((value * 100) / m_max));
}

void CalcProgressDialog::onFinished()
{
    m_label->setText("✅ 計算完了！");
    m_cancelButton->setEnabled(false);
    m_progressBar->setValue(m_max);
    QTimer::singleShot(1500, this, &QDialog::close);
}

void CalcProgressDialog::onCanceled()
{
    m_label->setText("⏹️ キャンセル完了");
    m_cancelButton->setEnabled(false);
    QTimer::singleShot(800, this, &QDialog::close);
}

void CalcProgressDialog::onCancelClicked()
{
    if (m_worker) {
        m_cancelButton->setText("停止中...");
        m_cancelButton->setEnabled(false);
        m_worker->requestCancel();
    }
}
