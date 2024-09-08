#pragma once
#include <QWidget>
#include <QPainter>
#include <QPoint>

// Абстрактный базовый класс для всех геометрических объектов
class Shape : public QWidget {

    Q_OBJECT

public:
    virtual ~Shape() = default;

    // Чисто виртуальные методы, которые должны быть реализованы в наследниках
    virtual void draw(QPainter& painter) const = 0;
    virtual void move(const QPoint& delta) = 0;
    virtual bool contains(const QPoint& point) const = 0;
};

// Класс отрезка, наследник Shape
class Line : public Shape {

    Q_OBJECT

public:
    Line(const QPoint& startPoint, const QPoint& endPoint);

    void draw(QPainter& painter) const override;
    void move(const QPoint& delta) override;
    bool contains(const QPoint& point) const override;

private:
    QPoint startPoint;
    QPoint endPoint;
};

// В дальнейшем можно добавлять другие классы (Circle, Rectangle и т.д.)
