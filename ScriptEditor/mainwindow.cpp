#pragma execution_character_set("utf-8")
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDateTime>
#include <QSettings>
//--------------------------------------------------------------------
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , udpSender(new UDPSender(this))
    , currentFilePath("")
{
    ui->setupUi(this);
    setWindowTitle("ScriptEditor");
    // сохранение настроек размера окна
    QSettings s;
    this->restoreGeometry(s.value("geometry").toByteArray());

    ui->ipLED->setText("127.0.0.1");  // IP по умолчанию
    ui->portLED->setText("1111");     // Порт по умолчанию

    setupInitialScript(); // пример

    // подключаем слоты и сигналы
    connect(udpSender, &UDPSender::scriptSent, this, &MainWindow::onScriptSent);
    connect(udpSender, &UDPSender::errorOccurred, this, &MainWindow::onErrorOccurred);
    connect(udpSender, &UDPSender::logMessage, this, &MainWindow::onLogMessage);


    appendToLog("=== ScriptEditor запущен ===");
    appendToLog("Готов к работе. Введите скрипт и нажмите Send.");
}
//--------------------------------------------------------------------
MainWindow::~MainWindow()
{
    QSettings s;
    s.setValue("geometry", this->saveGeometry());
    delete ui;
}
//--------------------------------------------------------------------
void MainWindow::on_savePBN_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(
        this,
        "Сохранить скрипт",
        "../script.qs",  // Начальный путь
        "Qt Script Files (*.qs);;JavaScript Files (*.js);;All Files (*.*)"
    );

    if (filePath.isEmpty()) {
        appendToLog("Файл не выбран!    ");
        return;
    }
    // чтобы было нужно расширение
    if (!filePath.endsWith(".qs", Qt::CaseInsensitive)) {
        filePath += ".qs";
    }
    currentFilePath = filePath;

    saveFile(currentFilePath);
}
//--------------------------------------------------------------------
void MainWindow::on_loadPBN_clicked()
{
    // выбор файла для загрузки
    QString filePath = QFileDialog::getOpenFileName(
        this,
        "load script",
        "..",
        "Qt Script Files (*.qs *.js);;Text Files (*.txt);;All Files (*.*)"
    );

    if (!filePath.isEmpty()) {
        loadFile(filePath);
        currentFilePath = filePath;
        appendToLog(QString("Загружен файл: %1").arg(filePath));
    }
    else {
        appendToLog("Ошибка загрузки файла");
    }
}
//--------------------------------------------------------------------
void MainWindow::on_sendPBN_clicked()
{
    QString script = ui->PTE->toPlainText();  // получили скрипт с редактора
    if (script.isEmpty()) {
        appendToLog("ERROR: Скрипт пуст!");
        return;
    }
    // получили ip и port
    QString ip = ui->ipLED->text().trimmed();
    QString portText = ui->portLED->text().trimmed();

    if (ip.isEmpty()) {
        appendToLog("Введите IP");
    }
    if (portText.isEmpty()) {
        appendToLog("Введите Port");
    }

    // проверяем валидность ip и port
    bool ok;
    quint16 port = portText.toUShort(&ok);
    if (!ok || port == 0 || port > 65535) {
        appendToLog(QString("ERROR: Неверный порт '%1'").arg(portText));
        return;
    }
    if (ip.isEmpty()) {
        ip = "127.0.0.1";
        ui->ipLED->setText(ip);
        appendToLog("IP не указан, используется 127.0.0.1");
    }

    appendToLog(QString("Отправка скрипта на %1:%2").arg(ip).arg(port));
    // отправка скрипт через UDPSender
    if (!udpSender->sendScript(script, ip, port)) {
        appendToLog("ERROR: Ошибка отправка");
    }
}
//--------------------------------------------------------------------
void MainWindow::onScriptSent()
{
    appendToLog("✓ Скрипт успешно отправлен!");
}
//--------------------------------------------------------------------
void MainWindow::onErrorOccurred(const QString& error)
{
    appendToLog("✗ ERROR: " + error);
}
//--------------------------------------------------------------------
void MainWindow::onLogMessage(const QString& message)
{
    appendToLog(message);
}
//--------------------------------------------------------------------
void MainWindow::loadFile(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString error = QString("Ошибка загрузки файла: %1").arg(file.errorString());
        appendToLog("ERROR:" + error);
        return;
    }

    QTextStream stream(&file);
    stream.setCodec("UTF-8");
    QString script = stream.readAll();
    file.close();

    ui->PTE->setPlainText(script);

    QString fileName = QFileInfo(filePath).fileName();
    setWindowTitle(QString("ScriptEditor - %1").arg(fileName));
}
//--------------------------------------------------------------------
void MainWindow::saveFile(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QString error = QString("Ошибка сохранения файла: %1").arg(file.errorString());
        appendToLog("ERROR: " + error);
        return;
    }

    QTextStream stream(&file);
    stream.setCodec("UTF-8");
    stream << ui->PTE->toPlainText();
    file.close();

    appendToLog(QString("Файл сохранён: %1").arg(filePath));
}
//--------------------------------------------------------------------
void MainWindow::setupInitialScript()
{
    QString exampleScript =
        "function drawAllShapes() {\n"
        "    // 1. Blue filled circle (x, y, radius, color, fill)\n"
        "    drawCircle(100, 100, 50, \"blue\", true);\n"
        "}\n"
        "drawAllShapes();\n"
        "\n";
    ui->PTE->setPlainText(exampleScript);
}
//--------------------------------------------------------------------
void MainWindow::appendToLog(const QString& message)
{
    QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
    QString logEntry = QString("[%1] %2").arg(timestamp).arg(message);

    // добавили в QTextEdit
    ui->logTE->append(logEntry);

    // прокручиваем вниз если много логов
    QTextCursor cursor = ui->logTE->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->logTE->setTextCursor(cursor);
}
//--------------------------------------------------------------------
