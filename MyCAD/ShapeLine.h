#pragma once
#include "Shape.h"
// Класс отрезка, наследник Shape
class Line : public Shape {

    Q_OBJECT

public:
    Line(const QPoint& startPoint, const QPoint& endPoint);
    Line(const Line& other);
    void draw(QPainter& painter) const override;
    void move(const QPoint& delta) override;
    void moveStart(const QPoint& delta) override;
    void moveEnd(const QPoint& delta) override;
    bool contains(const QPoint& point)  override;
    HandleType getHandleAt(const QPoint& point)  override;
    void setHandleState(HandleType handleType);
    // Метод клонирования
    std::shared_ptr<Shape> clone() const override;
    void setCoords(const QPoint& startPoint, const QPoint& endPoint, bool isSelected) override;
    QPoint getstartPoint()const override;
    QPoint getendPoint()const override;
    QPoint getmiddlePoint()const override;
    bool getisStart()const override;
    bool getisEnd()const override;
    bool getisMiddle()const override;
    Type name() const override;
    QRect getStartHandle() const override;  // Возвращает область квадрата в начале линии
    QRect getEndHandle() const override;    // Возвращает область квадрата в конце линии
    QRect getMiddleHandle() const override; // Возвращает область квадрата в середине линии
    
private:

};