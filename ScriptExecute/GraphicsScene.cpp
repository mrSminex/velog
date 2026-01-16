#pragma execution_character_set("utf-8")
#include "GraphicsScene.h"
#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>
#include <QGraphicsPolygonItem>
#include <QGraphicsLineItem>
#include <QPolygonF>
//--------------------------------------------------------------
GraphicsScene::GraphicsScene(QObject* parent) : QGraphicsScene(parent)
{
    setBackgroundBrush(Qt::white);
}
//--------------------------------------------------------------
void GraphicsScene::drawCircle(qreal x, qreal y, qreal radius, const QString& colorName, bool filled)
{
    QColor color = parseColor(colorName);

    // создаём эллипс = круг
    QGraphicsEllipseItem* circle = new QGraphicsEllipseItem(
        x - radius, y - radius, radius * 2, radius * 2
    );

    // Настраиваем стиль
    QPen pen(color);
    pen.setWidth(2);
    circle->setPen(pen);

    if (filled) {
        circle->setBrush(QBrush(color));
    }
    else {
        circle->setBrush(Qt::NoBrush);
    }

    // Добавляем на сцену
    addItem(circle);
    items.append(circle);
}
//--------------------------------------------------------------
void GraphicsScene::drawRectangle(qreal x, qreal y, qreal width, qreal height, const QString& colorName)
{
    QColor color = parseColor(colorName);

    QGraphicsRectItem* rect = new QGraphicsRectItem(x, y, width, height);

    QPen pen(color);
    pen.setWidth(2);
    rect->setPen(pen);
    rect->setBrush(Qt::NoBrush);  // незалитый прямоугольник по ТЗ

    addItem(rect);
    items.append(rect);
}
//--------------------------------------------------------------
void GraphicsScene::drawTriangle(qreal x1, qreal y1, qreal x2, qreal y2, qreal x3, qreal y3, const QString& colorName)
{
    QColor color = parseColor(colorName);

    // Создаём полигон (треугольник)
    QPolygonF polygon;
    polygon << QPointF(x1, y1) << QPointF(x2, y2) << QPointF(x3, y3);

    QGraphicsPolygonItem* triangle = new QGraphicsPolygonItem(polygon);

    QPen pen(color);
    pen.setWidth(2);
    triangle->setPen(pen);
    triangle->setBrush(Qt::NoBrush);  // незалитый треугольник

    addItem(triangle);
    items.append(triangle);
}
//--------------------------------------------------------------
void GraphicsScene::drawLine(qreal x1, qreal y1, qreal x2, qreal y2, const QString& colorName)
{
    QColor color = parseColor(colorName);

    QGraphicsLineItem* line = new QGraphicsLineItem(x1, y1, x2, y2);

    QPen pen(color);
    pen.setWidth(2);
    line->setPen(pen);

    addItem(line);
    items.append(line);
}
//--------------------------------------------------------------
void GraphicsScene::clearCanvas()
{
    // удаляем все элементы
    for (QGraphicsItem* item : items) {
        removeItem(item);
        delete item;
    }
    items.clear();
}
//--------------------------------------------------------------
QColor GraphicsScene::parseColor(const QString& colorName)
{
    // сопоставление имён цветов с QColor
    if (colorName.compare("black", Qt::CaseInsensitive) == 0) return Qt::black;  //CaseInsensitive - без учета регистра
    if (colorName.compare("white", Qt::CaseInsensitive) == 0) return Qt::white;
    if (colorName.compare("red", Qt::CaseInsensitive) == 0) return Qt::red;
    if (colorName.compare("green", Qt::CaseInsensitive) == 0) return Qt::green;
    if (colorName.compare("blue", Qt::CaseInsensitive) == 0) return Qt::blue;
    if (colorName.compare("cyan", Qt::CaseInsensitive) == 0) return Qt::cyan;
    if (colorName.compare("magenta", Qt::CaseInsensitive) == 0) return Qt::magenta;
    if (colorName.compare("yellow", Qt::CaseInsensitive) == 0) return Qt::yellow;
    if (colorName.compare("gray", Qt::CaseInsensitive) == 0) return Qt::gray;
    if (colorName.compare("darkRed", Qt::CaseInsensitive) == 0) return Qt::darkRed;
    if (colorName.compare("darkGreen", Qt::CaseInsensitive) == 0) return Qt::darkGreen;
    if (colorName.compare("darkBlue", Qt::CaseInsensitive) == 0) return Qt::darkBlue;
    if (colorName.compare("darkCyan", Qt::CaseInsensitive) == 0) return Qt::darkCyan;
    if (colorName.compare("darkMagenta", Qt::CaseInsensitive) == 0) return Qt::darkMagenta;
    if (colorName.compare("darkYellow", Qt::CaseInsensitive) == 0) return QColor(128, 128, 0);
    if (colorName.compare("darkGray", Qt::CaseInsensitive) == 0) return Qt::darkGray;
    if (colorName.compare("lightGray", Qt::CaseInsensitive) == 0) return Qt::lightGray;

    // если имя не распознано, пробуем создать цвет из строки
    QColor color(colorName);
    if (color.isValid()) {
        return color;
    }
    // если ошибка, то будет черный
    return Qt::black;
}
//----------------------------------------------------------
