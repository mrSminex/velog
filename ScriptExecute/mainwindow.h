#pragma execution_character_set("utf-8")
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
//--------------------------------------------------------------
#include <QMainWindow>
#include "UDPReceiver.h"
#include "ScriptEngine.h"
#include "GraphicsScene.h"
//--------------------------------------------------------------
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
//--------------------------------------------------------------
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:

    void            on_startPBN_clicked();
    void            on_stopPBN_clicked();
    void            on_executeScriptPBN_clicked();

    // слоты для UDPReceiver
    void            onScriptReceived(const QString& script);
    void            onListeningStarted(quint16 port);
    void            onListeningStopped();
    void            onErrorOccurred(const QString& error);
    void            onLogMessage(const QString& message);

    // слот ScriptEngine
    void            onScriptExecuted(bool success, const QString& message);

private:
    Ui::MainWindow* ui;
    UDPReceiver* udpReceiver;
    ScriptEngine* scriptEngine;
    GraphicsScene* graphicsScene;
    QString         receivedScript;    // последний полученный скрипт

    void            setupGraphicsView();
    void            appendToLog(const QString& message);
    void            updateUIState(bool isListening);

};
#endif // MAINWINDOW_H
//--------------------------------------------------------------
