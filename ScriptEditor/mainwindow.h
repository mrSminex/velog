#pragma execution_character_set("utf-8")
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
//--------------------------------------------------------------------
#include <QMainWindow>
#include <QFile>
#include <QTextStream>
#include "UDPSender.h"
//--------------------------------------------------------------------
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
//--------------------------------------------------------------------
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void            on_savePBN_clicked();
    void            on_loadPBN_clicked();
    void            on_sendPBN_clicked();


    // cлоты для сигналов от UdpSender
    void            onScriptSent();
    void            onErrorOccurred(const QString& error);
    void            onLogMessage(const QString& message);
private:
    Ui::MainWindow* ui;
    UDPSender* udpSender;                          // отправитель
    QString         currentFilePath;                     // текущий открытый файл

private:
    void            loadFile(const QString& filePath);
    void            saveFile(const QString& filePath);
    void            setupInitialScript();                // заполняем редактор примером скрипта
    void            appendToLog(const QString& message); // добавление сообщения в лог
};
#endif // MAINWINDOW_H
//--------------------------------------------------------------------
