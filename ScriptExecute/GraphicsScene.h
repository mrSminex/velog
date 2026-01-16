#pragma execution_character_set("utf-8")
#pragma once
#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H
//--------------------------------------------------------------
#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QPainter>
#include <QList>
//--------------------------------------------------------------
// класс для отрисовки фигур из скрипта
class GraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit    GraphicsScene(QObject* parent = nullptr);

public slots:
    void        drawCircle(qreal x, qreal y, qreal radius, const QString& colorName, bool filled);
    void        drawRectangle(qreal x, qreal y, qreal width, qreal height, const QString& colorName);
    void        drawTriangle(qreal x1, qreal y1, qreal x2, qreal y2, qreal x3, qreal y3, const QString& colorName);
    void        drawLine(qreal x1, qreal y1, qreal x2, qreal y2, const QString& colorName);

    void        clearCanvas();

    QColor      parseColor(const QString& colorName);

private:
    QList<QGraphicsItem*> items;  // храним созданные элементы для очистки
};
//--------------------------------------------------------------
#endif // GRAPHICSSCENE_H
