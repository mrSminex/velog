#pragma execution_character_set("utf-8")
#pragma once
#ifndef SCRIPTENGINE_H
#define SCRIPTENGINE_H
//--------------------------------------------------------------------------
#include <QObject>
#include <QScriptEngine>
#include <QString>
//--------------------------------------------------------------------------
class GraphicsScene; // чтобы не включать заголовочный файл - уменьшает врем€ компил€ции
//--------------------------------------------------------------------------
//  ласс движка выполнени€ скриптов QTScript
class ScriptEngine : public QObject
{
    Q_OBJECT
public:
    explicit        ScriptEngine(QObject* parent = nullptr);

public:
    bool            executeScript(const QString& script, GraphicsScene* graphicsScene);

    QString         getLastError() const { return lastError; }
    bool            hasError() const { return !lastError.isEmpty(); }
    void            clearError() { lastError.clear(); }

signals:
    void             logMessage(const QString& message);
    // сигнал о результате выполнени€ скрипта
    void             scriptExecuted(bool success, const QString& message);

private:
    // –егистраци€ API рисовани€ в движке скриптов
    // делает функции GraphicsScene доступными из скрипта
    void            registerGraphicsAPI(GraphicsScene* graphicsScene);

    QScriptEngine   engine;
    QString         lastError;
};
//--------------------------------------------------------------------------
#endif // SCRIPTENGINE_H
