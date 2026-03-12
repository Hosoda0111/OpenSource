#include "mainwindow.h"
#include "taskmanager.h"
#include "calcworker.h"
#include "calcprogressdialog.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QProgressDialog>
#include <QLineEdit>
#include <QMessageBox>
#include <QFileDialog>
#include <QDir>
#include <QFileInfo>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    m_taskManager(new TaskManager(this)),
    m_progressDialog(new QProgressDialog(this)),
    m_statusLabel(nullptr),
    m_wallpaperButton(nullptr),
    m_timesEdit(nullptr),
    m_addButton(nullptr),
    m_subButton(nullptr),
    m_mulButton(nullptr),
    m_divButton(nullptr)
{
    setupUi();

    // ★修正：壁紙ダイアログ即表示防止
    m_progressDialog->setWindowTitle("壁紙変更中...");
    m_progressDialog->setLabelText("画像処理中...");
    m_progressDialog->setCancelButton(nullptr);
    m_progressDialog->setMinimumDuration(0);  // 即表示防止
    m_progressDialog->setRange(0, 0);         // 不確定プログレス
    m_progressDialog->setModal(true);
    m_progressDialog->reset();                // 初期化必須
    m_progressDialog->hide();                 // 明示的非表示

    // 完全非同期シグナル接続
    connect(m_wallpaperButton, &QPushButton::clicked,
            this, &MainWindow::onSelectWallpaper);
    connect(m_taskManager, &TaskManager::taskFinished,
            this, &MainWindow::onTaskFinished);

    connect(m_addButton, &QPushButton::clicked, this, &MainWindow::onAddClicked);
    connect(m_subButton, &QPushButton::clicked, this, &MainWindow::onSubClicked);
    connect(m_mulButton, &QPushButton::clicked, this, &MainWindow::onMulClicked);
    connect(m_divButton, &QPushButton::clicked, this, &MainWindow::onDivClicked);
}

MainWindow::~MainWindow() {}

void MainWindow::setupUi()
{
    resize(450, 280);
    setWindowTitle("🖼️ Wallpaper + 🔢 四則演算（完全非同期）");

    QWidget* central = new QWidget(this);
    setCentralWidget(central);
    QVBoxLayout* mainLayout = new QVBoxLayout(central);

    // ステータスと壁紙変更ボタン
    m_statusLabel = new QLabel("初期画面: 壁紙を変更します", this);
    m_statusLabel->setAlignment(Qt::AlignCenter);
    m_statusLabel->setStyleSheet("font-size: 14px; padding: 10px;");

    m_wallpaperButton = new QPushButton("📁 壁紙画像を選択", this);
    m_wallpaperButton->setStyleSheet("QPushButton { padding: 12px; font-size: 16px; }");

    mainLayout->addWidget(m_statusLabel);
    mainLayout->addWidget(m_wallpaperButton);
    mainLayout->addSpacing(25);

    // 四則演算UI
    QHBoxLayout* timesLayout = new QHBoxLayout();
    QLabel* timesLabel = new QLabel("回数:", this);
    m_timesEdit = new QLineEdit(this);
    m_timesEdit->setPlaceholderText("回数を入力");
    m_timesEdit->setFixedWidth(100);
    m_timesEdit->setText("0");  // ★初期値0に修正

    timesLayout->addWidget(timesLabel);
    timesLayout->addWidget(m_timesEdit);
    timesLayout->addStretch();

    mainLayout->addLayout(timesLayout);
    mainLayout->addSpacing(15);

    // 演算ボタン
    QHBoxLayout* buttonsLayout = new QHBoxLayout();
    m_addButton = new QPushButton("+", this);
    m_subButton = new QPushButton("-", this);
    m_mulButton = new QPushButton("×", this);
    m_divButton = new QPushButton("÷", this);

    QList<QPushButton*> calcButtons = {m_addButton, m_subButton, m_mulButton, m_divButton};
    for (QPushButton* btn : calcButtons) {
        btn->setFixedSize(70, 50);
        btn->setStyleSheet("QPushButton { font-size: 24px; font-weight: bold; border-radius: 10px; }");
    }

    buttonsLayout->addStretch();
    buttonsLayout->addWidget(m_addButton);
    buttonsLayout->addWidget(m_subButton);
    buttonsLayout->addWidget(m_mulButton);
    buttonsLayout->addWidget(m_divButton);
    buttonsLayout->addStretch();

    mainLayout->addLayout(buttonsLayout);
    mainLayout->addStretch();
}

void MainWindow::onSelectWallpaper()
{
    QString filter = "画像ファイル (*.jpg *.jpeg *.png *.bmp *.gif)";
    QString filePath = QFileDialog::getOpenFileName(
        this,
        "壁紙画像を選択",
        QDir::homePath(),
        filter
        );

    if (filePath.isEmpty()) return;

    m_progressDialog->show();
    m_statusLabel->setText(QString("「%1」で壁紙変更中...").arg(QFileInfo(filePath).fileName()));
    m_taskManager->startWallpaperChange(filePath);
}

void MainWindow::onTaskFinished(bool success)
{
    m_progressDialog->hide();
    m_statusLabel->setText(success ? "✅ 壁紙変更完了！" : "❌ 壁紙変更失敗");
}

// 完全非同期演算ボタン
void MainWindow::onAddClicked() { startCalculation(0); }
void MainWindow::onSubClicked() { startCalculation(1); }
void MainWindow::onMulClicked() { startCalculation(2); }
void MainWindow::onDivClicked() { startCalculation(3); }

void MainWindow::startCalculation(int opType)
{
    bool ok;
    int times = m_timesEdit->text().toInt(&ok);
    if (!ok || times <= 0) {
        QMessageBox::warning(this, "エラー", "正しい回数を入力してください");
        return;
    }

    QString opStr;
    CalcWorker::Operation op = CalcWorker::Add;
    switch (opType) {
    case 0: op = CalcWorker::Add; opStr = "+"; break;
    case 1: op = CalcWorker::Sub; opStr = "-"; break;
    case 2: op = CalcWorker::Mul; opStr = "×"; break;
    case 3: op = CalcWorker::Div; opStr = "÷"; break;
    }

    QString label = QString("1 %1 1 を %2回計算中...").arg(opStr).arg(times);

    // 完全非同期：即座にダイアログ表示＋スレッド開始
    CalcWorker* worker = new CalcWorker(op, times);
    CalcProgressDialog* dlg = new CalcProgressDialog(label, times, this);
    dlg->startWorker(worker);
    dlg->show();  // 非ブロッキング！即座に制御返却
}
