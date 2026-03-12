#include "calcworker.h"
#include <QThread>

CalcWorker::CalcWorker(Operation op, int times, QObject* parent)
    : QObject(parent),
    m_op(op),
    m_times(times),
    m_cancelRequested(false)
{
}

void CalcWorker::start()
{
    for (int i = 0; i < m_times; ++i) {
        if (m_cancelRequested) {
            emit canceled();
            return;
        }

        // 疑似計算: 1 op 1
        volatile double result = 0.0;
        switch (m_op) {
        case Add:
            result = 1.0 + 1.0;
            break;
        case Sub:
            result = 1.0 - 1.0;
            break;
        case Mul:
            result = 1.0 * 1.0;
            break;
        case Div:
            result = 1.0 / 1.0;
            break;
        }

        // ちょっとだけ待つと進捗が分かりやすい（重すぎれば調整）
        QThread::msleep(10);

        emit progress(i + 1);
    }

    emit finished();
}

void CalcWorker::requestCancel()
{
    m_cancelRequested = true;
}
