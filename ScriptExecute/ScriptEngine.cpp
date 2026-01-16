#pragma execution_character_set("utf-8")
#include "ScriptEngine.h"
#include "GraphicsScene.h"
#include <QDebug>
//--------------------------------------------------------------------------
ScriptEngine::ScriptEngine(QObject* parent) : QObject(parent), lastError("")
{

}
//--------------------------------------------------------------------------
bool ScriptEngine::executeScript(const QString& script, GraphicsScene* graphicsScene)
{
    emit logMessage("Начало выполнение скрипта...");

    //перед выполнением регаем API
    registerGraphicsAPI(graphicsScene);

    //очистка последней ошибки
    clearError();

    // Выполняем скрипт
    QScriptValue result = engine.evaluate(script);

    // Проверяем на ошибки
    if (engine.hasUncaughtException()) {
        lastError = QString("Строка %1: %2")
            .arg(engine.uncaughtExceptionLineNumber())
            .arg(result.toString());

        emit logMessage("ERROR script: " + lastError);
        emit scriptExecuted(false, lastError);
        return false;
    }

    // Проверяем результат
    if (result.isError()) {
        lastError = result.toString();
        emit logMessage("ERROR execute: " + lastError);
        emit scriptExecuted(false, lastError);
        return false;
    }

    emit scriptExecuted(true, "Скрипт успешно выполнен!");
    return true;

}
//--------------------------------------------------------------------------
void ScriptEngine::registerGraphicsAPI(GraphicsScene* graphicsScene)
{
    if (!graphicsScene) {
        emit logMessage("ОШИБКА: GraphicsScene не инициализирован!");
        return;
    }
    //  объект-обёртку для GraphicsScene
    QScriptValue graphicsObject = engine.newQObject(graphicsScene);
    // Делаем функции доступными в глобальной области видимости скрипта
    // иначе бы пришлось писать в скрипте graphicsObject.drawCircle(100, 100, 50);
    engine.globalObject().setProperty("drawCircle", graphicsObject.property("drawCircle"));
    engine.globalObject().setProperty("drawRectangle", graphicsObject.property("drawRectangle"));
    engine.globalObject().setProperty("drawTriangle", graphicsObject.property("drawTriangle"));
    engine.globalObject().setProperty("drawLine", graphicsObject.property("drawLine"));
    engine.globalObject().setProperty("clearCanvas", graphicsObject.property("clearCanvas"));

    //...
    // print("x =", 10, "y =", 20);     -->  [Script Debug]: x = 10 y = 20
//    engine.globalObject().setProperty("print",
//        engine.newFunction([](QScriptContext *context, QScriptEngine *engine) {
//           QString result;
//           for (int i = 0; i < context->argumentCount(); ++i) {
//               if (i > 0) result += " ";
//               result += context->argument(i).toString();
//           }
//           qDebug() << "[Script Debug]:" << result;
//           return engine->undefinedValue();
//       }));

    emit logMessage("API для рисования зарегистрировано в движке скриптов");
}
//--------------------------------------------------------------------------
