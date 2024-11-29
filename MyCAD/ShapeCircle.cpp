#include "ShapeCircle.h"


Circle::Circle(const QPoint& startPoint, const int radius)
{
    setHandleSize(10);
    ShapePoints points;
    points.startPoint = startPoint;
    points.radius = radius;
    setPoints(points);
}

Circle::Circle(const Circle& other)
{
    setHandleSize(10);
    const  ShapePoints& otherpoints = other.getPoints();
    ShapePoints points;
    points.startPoint = otherpoints.startPoint;
    points.radius = otherpoints.radius;
    setPoints(points);
}

Circle::~Circle()
{
}

void Circle::draw(QPainter& painter) const
{
    const auto& points = getPoints();
    QPen pen = painter.pen();
    // Если линия выделена, изменяем цвет
    if (getisSelected() && !getisMoveSelected()) {
        QColor highlightedColor(90, 150, 255, 169);
        QPen highlightedPen(highlightedColor, 4, Qt::SolidLine);
        painter.setPen(highlightedPen);
        painter.drawEllipse(points.startPoint, points.radius, points.radius);
        // возвращаем Pen
        painter.setPen(pen);
        // Рисуем квадраты на сверху снизу спарва и слева и по центру, если  выделено
        painter.setBrush(getColors().ColorStartPoint);
        painter.drawRect(getStartHandle());

        painter.setBrush(getColors().ColorLeftPoint);
        painter.drawRect(getLeftHandle());

        painter.setBrush(getColors().ColorTopPoint);
        painter.drawRect(getToptHandle());

        painter.setBrush(getColors().ColorRightPoint);
        painter.drawRect(getRighttHandle());

        painter.setBrush(getColors().ColorBottomPoint);
        painter.drawRect(getBottomtHandle());

        // Отключаем заливку
        painter.setBrush(Qt::NoBrush);
    }

    if (getisover()) {
        QColor highlightedColor(250, 250, 250, 120);
        QPen highlightedPen(highlightedColor, 4, Qt::SolidLine);
        painter.setPen(highlightedPen);
        painter.drawEllipse(points.startPoint, points.radius, points.radius);

        // возвращаем Pen
        painter.setPen(pen);
    }
    painter.setPen(pen);
    painter.drawEllipse(points.startPoint, points.radius, points.radius);


}

void Circle::move(const QPoint& delta)
{
    auto points = getPoints();
    points.startPoint += delta;
    setPoints(points);
}

void Circle::moveStart(const QPoint& delta)
{
    auto points = getPoints();
    points.startPoint += delta;
    setPoints(points);
}

void Circle::moveRadius(const int radius_)
{
    auto points = getPoints();
    points.radius = radius_;
    setPoints(points);
}

bool Circle::contains(const QPoint& point)
{
    const auto& points = getPoints();
    // Вычисляем расстояние от точки до центра
    double distanceSquared = pow(point.x() - points.startPoint.x(), 2) + pow(point.y() - points.startPoint.y(), 2);
    double radiusSquared = points.radius * points.radius;
    // Проверяем условие на нахождение точки на окружности с учетом погрешности
    return fabs(distanceSquared - radiusSquared) < 1000;
}

HandleType Circle::getHandleAt(const QPoint& point)
{
    const auto& points = getPoints();
    // Сбрасываем цвета всех хендлов
    resetColor();

    // Проверка каждого хендла через HandleManager
    if (getHandle()->isHandleSelected(HandleType::StartHandle, point, points)) {
        if (getisSelected()) {
            setColorsStart(QColor(165, 0, 0));
        }
        setHandleState(HandleType::StartHandle);
        return HandleType::StartHandle;
    }

    if (getHandle()->isHandleSelected(HandleType::LeftHandle, point, points)) {
        if (getisSelected()) {
            setColorsLeft(QColor(165, 0, 0));
        }
        setHandleState(HandleType::LeftHandle);
        return HandleType::LeftHandle;
    }

    if (getHandle()->isHandleSelected(HandleType::TopHandle, point, points)) {
        if (getisSelected()) {
            setColorsTop(QColor(165, 0, 0));
        }
        setHandleState(HandleType::TopHandle);
        return HandleType::TopHandle;
    }

    if (getHandle()->isHandleSelected(HandleType::RightHandle, point, points)) {
        if (getisSelected()) {
            setColorsRight(QColor(165, 0, 0));
        }
        setHandleState(HandleType::RightHandle);
        return HandleType::RightHandle;
    }

    if (getHandle()->isHandleSelected(HandleType::BottomHandle, point, points)) {
        if (getisSelected()) {
            setColorsBottom(QColor(165, 0, 0));
        }
        setHandleState(HandleType::BottomHandle);
        return HandleType::BottomHandle;
    }

    // Если ни один хендл не выбран
    setHandleState(HandleType::None);
    return HandleType::None;
}



//
std::shared_ptr<Shape> Circle::clone() const
{
    return std::make_shared<Circle>(*this);
}

void Circle::setCentre(const QPoint& startPoint_, const int radius_, bool isSelected_)
{
    setSelected(isSelected_);
    ShapePoints points;
    points.startPoint = startPoint_;
    points.radius = radius_;
    setPoints(points);
}

QPoint Circle::getstartPoint() const
{
    return getPoints().startPoint;
}

int Circle::getradius() const
{
    return getPoints().radius;
}


bool Circle::getisStart()const
{
    return getSelected().isStart;
}

bool Circle::getisLeft()const
{
    return getSelected().isLeft;
}

bool Circle::getisRight()const
{
    return getSelected().isRight;
}

bool Circle::getisTop() const
{
    return getSelected().isTop;
}

bool Circle::getisBottom() const
{
    return getSelected().isBottom;
}

Type Circle::name() const
{
    return Type::circle;
}

QRect Circle::getStartHandle() const {
    const auto& points = getPoints();
    return getHandle()->getHandle(HandleType::StartHandle, points);
}


QRect Circle::getLeftHandle() const {
    const auto& points = getPoints();
    return getHandle()->getHandle(HandleType::LeftHandle, points);
}

QRect Circle::getRighttHandle() const {
    const auto& points = getPoints();
    return getHandle()->getHandle(HandleType::RightHandle, points);
}

QRect Circle::getToptHandle() const {
    const auto& points = getPoints();
    return getHandle()->getHandle(HandleType::TopHandle, points);
}

QRect Circle::getBottomtHandle() const {
    const auto& points = getPoints();
    return getHandle()->getHandle(HandleType::BottomHandle, points);
}

