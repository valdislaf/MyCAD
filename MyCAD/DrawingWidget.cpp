#include "DrawingWidget.h"
#include <QDebug>
#include <QMouseEvent>

DrawingWidget::DrawingWidget(MyCAD* parent)
    : QWidget(parent), myCad(parent)
{
    myCad->setCursor(myCad->createCustomCrossCursor());
    qDebug() << "DrawingWidget created:" << this;
    qDebug() << "DrawingWidget size:" << size();
}

void DrawingWidget::MyMethod() {
    qDebug() << "DrawingWidget Mymethod";
}
void DrawingWidget::leaveEvent(QEvent* event)
{
    myCad->setCursor(Qt::ArrowCursor);
    QWidget::leaveEvent(event);  // Вызов базового метода
}

void DrawingWidget::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::MiddleButton) // Проверяем, что нажата средняя кнопка мыши
    {
        myCad->setCursor(QCursor(Qt::PointingHandCursor));
    }
    QWidget::mousePressEvent(event);  // Вызов базового метода
}

void DrawingWidget::enterEvent(QEnterEvent* event)
{
    myCad->setCursor(myCad->createCustomCrossCursor());
    QWidget::enterEvent(event);  // Вызов базового метода
}

void DrawingWidget::mouseReleaseEvent(QMouseEvent* event)
{
    myCad->setCursor(myCad->createCustomCrossCursor());
    QWidget::mouseReleaseEvent(event);  // Вызов базового метода
}


void DrawingWidget::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    if (!painter.isActive()) {
        qDebug() << "DrawingWidget::paintEvent QPainter is not active!";
        return;
    }
    qDebug() << "DrawingWidget::paintEvent called for:" << this;

    // Ваш код отрисовки
    if (myCad) {
        myCad->drawGrid(painter);
        myCad->drawShapes(painter);
    }
}

bool DrawingWidget::event(QEvent* e) {
    qDebug() << "DrawingWidget Event type:" << e->type();
    return QWidget::event(e);  // Не забывайте передавать событие дальше
}