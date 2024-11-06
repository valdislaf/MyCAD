#include "DrawingWidget.h"
#include <QDebug>
#include <QMouseEvent>

DrawingWidget::DrawingWidget(MyCAD* parent)
    : QWidget(parent), myCad(parent)
{
    this->setCursor(Qt::BlankCursor);
}

void DrawingWidget::MyMethod() {
}

void DrawingWidget::leaveEvent(QEvent* event)
{
    this->unsetCursor();
    myCad->setCursor(Qt::ArrowCursor);
    QWidget::leaveEvent(event);  // Вызов базового метода
}

void DrawingWidget::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::MiddleButton) // Проверяем, что нажата средняя кнопка мыши
    {
        this->unsetCursor();
        myCad->setCursor(QCursor(Qt::PointingHandCursor));
        isMiddlON = true;
        update();
    }
    QWidget::mousePressEvent(event);  // Вызов базового метода
}

void DrawingWidget::enterEvent(QEnterEvent* event)
{
    this->setCursor(Qt::BlankCursor);
    QWidget::enterEvent(event);  // Вызов базового метода
}

void DrawingWidget::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::MiddleButton) // Проверяем, что нажата средняя кнопка мыши
    {
        this->setCursor(Qt::BlankCursor);
        isMiddlON = false;

    }

    QWidget::mouseReleaseEvent(event);  // Вызов базового метода
}

void DrawingWidget::mouseMoveEvent(QMouseEvent* event)
{
    update();
    QWidget::mouseMoveEvent(event);  // Вызов базового метода
}



void DrawingWidget::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    if (!painter.isActive()) {
        return;
    }
  
    // Ваш код отрисовки
    if (myCad) {
        myCad->drawGrid(painter);
        myCad->drawShapes(painter);
        if (!isMiddlON ) {
            if (!disableCursor) {
                if (currentDrawMode == DrawMode::Line) { myCad->CrossCursorIn(painter); }
                else if (currentDrawMode == DrawMode::Circle) { myCad->CrossCursorIn(painter); }
                else if (selShapes.empty() && !disableCursor) { myCad->CrossCursorOut(painter); }
                else {
                    myCad->CrossCursorIn(painter);
                }
			}
            else {
                myCad->CrossCursorHandle(painter);
            }
        }
        if (isdraw && clickpoint != QPoint(INT_MIN, INT_MIN)) {
            if (currentDrawMode == DrawMode::Line) { myCad->DrawLine(painter, clickpoint); }
            else  if (currentDrawMode == DrawMode::Circle) { myCad->DrawCircle(painter, clickpoint); }
        }
    }


}

bool DrawingWidget::event(QEvent* e) {
    return QWidget::event(e);  // Не забывайте передавать событие дальше
}