#ifndef CALCWORKER_H
#define CALCWORKER_H

#include <QObject>

class CalcWorker : public QObject
{
    Q_OBJECT
public:
    enum Operation {
        Add,
        Sub,
        Mul,
        Div
    };

    explicit CalcWorker(Operation op, int times, QObject* parent = nullptr);

signals:
    void progress(int value);     // 進捗（0〜times）
    void finished();              // 正常完了
    void canceled();              // キャンセル完了

public slots:
    void start();                 // スレッド側で実行
    void requestCancel();         // キャンセル要求

private:
    Operation m_op;
    int m_times;
    bool m_cancelRequested;
};

#endif // CALCWORKER_H
