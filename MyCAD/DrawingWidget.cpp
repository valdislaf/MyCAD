#include "DrawingWidget.h"
#include <QDebug>
#include <QMouseEvent>

DrawingWidget::DrawingWidget(MyCAD* parent)
    : QWidget(parent), myCad(parent)
{
    this->setCursor(Qt::BlankCursor);
    // myCad->setCursor(myCad->createCustomCrossCursor());
    //qDebug() << "DrawingWidget created:" << this;
    //qDebug() << "DrawingWidget size:" << size();
}

void DrawingWidget::MyMethod() {
    //qDebug() << "DrawingWidget Mymethod";
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

    //if (!ondrawline && !ondrawcircle) {
    //    this->unsetCursor();
    //    myCad->setCursor(myCad->createCustomCrossCursor());
    //}

    /*if (!ondrawcircle) {
        this->unsetCursor();
        myCad->setCursor(myCad->createCustomCrossCursor());
    }*/
   /* if (!ondrawline || !ondrawcircle) {
        this->unsetCursor();
        myCad->setCursor(myCad->createCustomCrossCursor());
    }*/
    //if (ondrawline) {
    //    this->setCursor(Qt::BlankCursor);
    //   // myCad->setCursor(myCad->createCustomCrossCursorIn()); 
    //}
    //else if (ondrawcircle) {
    //    this->setCursor(Qt::BlankCursor);
    //    //myCad->setCursor(myCad->createCustomCrossCursorIn());
    //    }
    //else { 
    //   // this->unsetCursor();

    //   // myCad->setCursor(myCad->createCustomCrossCursor());
    //}
    QWidget::enterEvent(event);  // Вызов базового метода
}

void DrawingWidget::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::MiddleButton) // Проверяем, что нажата средняя кнопка мыши
    {
        this->setCursor(Qt::BlankCursor);
        isMiddlON = false;
      /*  if (!ondrawline && !ondrawcircle) {
            this->unsetCursor();
            myCad->setCursor(myCad->createCustomCrossCursor());
        }*/
      /*  if (!ondrawcircle && ondrawline) {
            this->unsetCursor();
            myCad->setCursor(myCad->createCustomCrossCursor());
        }*/
       /* if ((!ondrawline || !ondrawcircle) && !isdraw) {
            this->unsetCursor();
            myCad->setCursor(myCad->createCustomCrossCursor());
        }*/
    }
    //this->setCursor(Qt::BlankCursor);
    //if (ondrawline) {
    //    this->setCursor(Qt::BlankCursor);
    //    // myCad->setCursor(myCad->createCustomCrossCursorIn()); 
    //}
    //else if (ondrawcircle) {
    //    this->setCursor(Qt::BlankCursor);
    //    //myCad->setCursor(myCad->createCustomCrossCursorIn());
    //}
    //else {
    // //   this->unsetCursor();

    //    // myCad->setCursor(myCad->createCustomCrossCursor());
    //}
    QWidget::mouseReleaseEvent(event);  // Вызов базового метода
}

void DrawingWidget::mouseMoveEvent(QMouseEvent* event)
{
    update();
   /* QPainter painter(this);
    if (myCad) {
        myCad->DrawLine(painter);       
    }*/
    QWidget::mouseMoveEvent(event);  // Вызов базового метода
}



void DrawingWidget::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    if (!painter.isActive()) {
       // qDebug() << "DrawingWidget::paintEvent QPainter is not active!";
        return;
    }
    //qDebug() << "DrawingWidget::paintEvent called for:" << this;

    // Ваш код отрисовки
    if (myCad) {
        myCad->drawGrid(painter);
        myCad->drawShapes(painter);
        if (!isMiddlON) {
        if (ondrawline) { myCad->CrossCursorIn(painter); }
        else if (ondrawcircle) { myCad->CrossCursorIn(painter); }
        else if(selShapes.empty() ){ myCad->CrossCursorOut(painter); }
        else { myCad->CrossCursorIn(painter); }
        }
        
        if (isdraw) {
            if (ondrawline) { myCad->DrawLine(painter, clickpoint); }
            else  if (ondrawcircle) { myCad->DrawCircle(painter, clickpoint); }
        }
    }


}

bool DrawingWidget::event(QEvent* e) {
    
    //qDebug() << "DrawingWidget Event type:" << e->type();
    return QWidget::event(e);  // Не забывайте передавать событие дальше
}