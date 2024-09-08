#pragma once
#include <QWidget>
#include <QPainter>
#include "MyCAD.h"

class MyCAD;  // Объявление вперед для использования указателя

class DrawingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DrawingWidget(MyCAD* parent = nullptr);
    void MyMethod();

protected:
    void paintEvent(QPaintEvent* event) override;
    bool event(QEvent* e)override;
    void leaveEvent(QEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void enterEvent(QEnterEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    
private:
    MyCAD* myCad;  // Указатель на родительский объект MyCAD
};
