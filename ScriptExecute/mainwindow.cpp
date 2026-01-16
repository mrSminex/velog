#pragma execution_character_set("utf-8")
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSettings>
#include <QDateTime>
//--------------------------------------------------------------
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , udpReceiver(new UDPReceiver(this))
    , scriptEngine(new ScriptEngine(this))
    , graphicsScene(new GraphicsScene(this))
    , receivedScript("")
{
    ui->setupUi(this);

    QSettings s;
    this->restoreGeometry(s.value("geometry").toByteArray());

    ui->ListenPortLE->setText("1111");
    setupGraphicsView();  // сразу настроиваем QGraphicsView
    updateUIState(false); // изначальн не слушаем


    appendToLog("=== ScriptExecutor запущен ===");
    appendToLog("Нажмите Start для начала прослушивания UDP порта");
    // 1.  сигналы от UdpReceiver
    connect(udpReceiver, &UDPReceiver::scriptReceived, this, &MainWindow::onScriptReceived);
    connect(udpReceiver, &UDPReceiver::listeningStarted, this, &MainWindow::onListeningStarted);
    connect(udpReceiver, &UDPReceiver::listeningStopped, this, &MainWindow::onListeningStopped);
    connect(udpReceiver, &UDPReceiver::errorOccurred, this, &MainWindow::onErrorOccurred);
    connect(udpReceiver, &UDPReceiver::logMessage, this, &MainWindow::onLogMessage);
    // 2.  сигналы от ScriptEngine
    connect(scriptEngine, &ScriptEngine::scriptExecuted, this, &MainWindow::onScriptExecuted);
    connect(scriptEngine, &ScriptEngine::logMessage, this, &MainWindow::onLogMessage);
}
//--------------------------------------------------------------
MainWindow::~MainWindow()
{
    QSettings s;
    s.setValue("geometry", this->saveGeometry());
    delete ui;
}
//--------------------------------------------------------------
void MainWindow::on_startPBN_clicked()
{
    QString portText = ui->ListenPortLE->text().trimmed();
    // Валидация порта
    bool ok;
    quint16 port = portText.toUShort(&ok);
    if (!ok || port == 0 || port > 65535) {
        appendToLog("ОШИБКА: Неверный порт ");
        return;
    }
    if (udpReceiver->startListening(port)) {}  // -> onListeningStarted сам вызовется
}
//--------------------------------------------------------------
void MainWindow::on_stopPBN_clicked()
{
    udpReceiver->stopListening();
}
//--------------------------------------------------------------
void MainWindow::on_executeScriptPBN_clicked()
{
    if (receivedScript.isEmpty()) {
        appendToLog("ОШИБКА: Нет скрипта для выполнения");
        return;
    }
    // очистка холста
    graphicsScene->clearCanvas();
    if (scriptEngine->executeScript(receivedScript, graphicsScene)) {}
}
//--------------------------------------------------------------
void MainWindow::onScriptReceived(const QString& script)
{
    receivedScript = script;
    ui->logScript->setPlainText(script);

    // разрешаем выполнятьт
    ui->executeScriptPBN->setEnabled(true);

    appendToLog("✓ Скрипт получен и готов к выполнению");
    appendToLog("Нажмите Execute для выполнения");
}
//--------------------------------------------------------------
void MainWindow::onErrorOccurred(const QString& error)
{
    appendToLog("✗ Ошибка: " + error);
}
//--------------------------------------------------------------
void MainWindow::onListeningStarted(quint16 port)
{
    appendToLog(QString("✓ Начато прослушивание порта %1").arg(port));
    updateUIState(true);
    setWindowTitle(QString("ScriptExecutor - Порт %1").arg(port));
}
//--------------------------------------------------------------
void MainWindow::onListeningStopped()
{
    updateUIState(false);
    setWindowTitle("ScriptExecutor - Неактивно");
}
//--------------------------------------------------------------
void MainWindow::onScriptExecuted(bool success, const QString& message)
{
    if (success) {
        appendToLog("✓ " + message);
    }
    else {
        appendToLog("✗ Ошибка выполнения: " + message);
    }
}
//--------------------------------------------------------------
void MainWindow::onLogMessage(const QString& message)
{
    appendToLog(message);
}
//--------------------------------------------------------------
void MainWindow::setupGraphicsView()
{
    // настройки QGraphicsView для отображения нашей сцены
    ui->scriptGV->setScene(graphicsScene);
    ui->scriptGV->setRenderHint(QPainter::Antialiasing);        // сглаживание
    ui->scriptGV->setBackgroundBrush(Qt::white);                // фон
    ui->scriptGV->setDragMode(QGraphicsView::ScrollHandDrag);   // масштабировать колесиком мыши
    appendToLog("Графический виджет инициализирован");
}
//--------------------------------------------------------------
void MainWindow::appendToLog(const QString& message)
{
    QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
    QString logEntry = QString("[%1] %2").arg(timestamp).arg(message);

    ui->log->append(logEntry);

    QTextCursor cursor = ui->log->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->log->setTextCursor(cursor);
}
//--------------------------------------------------------------
void MainWindow::updateUIState(bool isListening)
{
    // Обновляем состояние кнопок в зависимости от состояния прослушивания
    ui->startPBN->setEnabled(!isListening);
    ui->stopPBN->setEnabled(isListening);
    ui->ListenPortLE->setEnabled(!isListening);

    // активна только если есть скрипт
    ui->executeScriptPBN->setEnabled(!receivedScript.isEmpty());
}
//--------------------------------------------------------------
