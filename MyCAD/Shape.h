#pragma once
#include <QWidget>
#include <QPainter>
#include <QPoint>

enum HandleType {
    None,
    StartHandle,
    EndHandle,
    MiddleHandle
};

// Абстрактный базовый класс для всех геометрических объектов
class Shape : public QWidget {

    Q_OBJECT

public:
    virtual ~Shape() = default;

    // Чисто виртуальные методы, которые должны быть реализованы в наследниках
    virtual void draw(QPainter& painter) const = 0;
    virtual void move(const QPoint& delta) = 0;
    virtual bool contains(const QPoint& point) const = 0;
    virtual void setSelected(bool selected) = 0;
    virtual  HandleType getHandleAt(const QPoint& point) const = 0;
};

// Класс отрезка, наследник Shape
class Line : public Shape {

    Q_OBJECT

public:
    Line(const QPoint& startPoint, const QPoint& endPoint);

    void draw(QPainter& painter) const override;
    void move(const QPoint& delta) override;
    bool contains(const QPoint& point) const override;
    void setSelected(bool selected) override;
    HandleType getHandleAt(const QPoint& point) const override;

private:
    QPoint startPoint;
    QPoint endPoint;
    bool isSelected = false;  // Флаг выделения

    // Размеры квадратов на концах и в середине линии
    static constexpr int handleSize = 10;

    QRect getStartHandle() const;  // Возвращает область квадрата в начале линии
    QRect getEndHandle() const;    // Возвращает область квадрата в конце линии
    QRect getMiddleHandle() const; // Возвращает область квадрата в середине линии
   
};

// В дальнейшем можно добавлять другие классы (Circle, Rectangle и т.д.)
