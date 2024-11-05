#pragma once
#include "Shape.h"


class Circle : public Shape {

    Q_OBJECT

public:
    Circle(const QPoint& startPoint, const int radius);
    Circle(const Circle& other);
    void draw(QPainter& painter) const override;
    void move(const QPoint& delta) override;
    void moveStart(const QPoint& delta) override;
    void moveRadius(const int radius)override;
    bool contains(const QPoint& point)  override;
    HandleType getHandleAt(const QPoint& point)  override;
    
    // Метод клонирования
    std::shared_ptr<Shape> clone() const override;
    void setCentre(const QPoint& startPoint, const int radius, bool isSelected) override;
    QPoint getstartPoint()const override;
    int getradius()const override;
    bool getisStart()const override;
    bool getisLeft()const override;
    bool getisRight()const override;
    bool getisTop()const override;
    bool getisBottom()const override;
    Type name() const override;
    QRect getStartHandle() const override;  // Возвращает область квадрата в начале линии    
    QRect getLeftHandle() const override;
    QRect getToptHandle() const override;
    QRect getRighttHandle() const override;
    QRect getBottomtHandle() const override;
    
private:

};