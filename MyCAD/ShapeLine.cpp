#include "ShapeLine.h"




// Реализация конструктора для класса Line
Line::Line(const QPoint& startPoint, const QPoint& endPoint)
{
    setHandleSize(10);
    ShapePoints points;
    points.startPoint = startPoint;
    points.endPoint = endPoint;
    setPoints(points);
}

Line::Line(const Line& other)
{
    setHandleSize(10);
    const  ShapePoints& otherpoints = other.getPoints();
    ShapePoints points;
    points.startPoint = otherpoints.startPoint;
    points.endPoint = otherpoints.endPoint;
    setPoints(points);
}

// Реализация метода  клонирования
std::shared_ptr<Shape> Line::clone() const {
    return std::make_shared<Line>(*this);
}

void Line::setCoords(const QPoint& startPoint_, const QPoint& endPoint_, bool isSelected_)
{
    setSelected(isSelected_);
    ShapePoints points;
    points.startPoint = startPoint_;
    points.endPoint = endPoint_;
    setPoints(points);
}

QPoint Line::getstartPoint() const
{
    return getPoints().startPoint;
}

QPoint Line::getendPoint() const
{
    return getPoints().endPoint;
}

QPoint Line::getmiddlePoint()const
{
    const auto& points = getPoints();
    return (points.startPoint + points.endPoint) / 2;
}


bool Line::getisStart() const
{
    return getSelected().isStart;
}

bool Line::getisEnd() const
{
    return getSelected().isEnd;
}

bool Line::getisMiddle() const
{
    return getSelected().isMiddle;
}

Type Line::name() const
{
    return Type::line;
}

// Реализация метода рисования отрезка
void Line::draw(QPainter& painter) const {
    QPen pen = painter.pen();
    const auto& points = getPoints();
    // Если линия выделена, изменяем цвет
    if (getisSelected() && !getisMoveSelected()) {
        QColor highlightedColor(90, 150, 255, 169);
        QPen highlightedPen(highlightedColor, 4, Qt::SolidLine);
        painter.setPen(highlightedPen);
        painter.drawLine(points.startPoint, points.endPoint);
        // возвращаем Pen
        painter.setPen(pen);
        // Рисуем квадраты на концах и в середине, если линия выделена
        painter.setBrush(getColors().ColorStartPoint);
        painter.drawRect(getStartHandle());
        painter.setBrush(getColors().ColorEndPoint);
        painter.drawRect(getEndHandle());
        painter.setBrush(getColors().ColorMiddlePoint);
        painter.drawRect(getMiddleHandle());
        // Отключаем заливку
        painter.setBrush(Qt::NoBrush);
    }

    if (getisover()) {
        QColor highlightedColor(250, 250, 250, 120);
        QPen highlightedPen(highlightedColor, 4, Qt::SolidLine);
        painter.setPen(highlightedPen);
        painter.drawLine(points.startPoint, points.endPoint);

        // возвращаем Pen
        painter.setPen(pen);
    }

    painter.setPen(pen);
    painter.drawLine(points.startPoint, points.endPoint);


}

// Реализация метода перемещения отрезка
void Line::move(const QPoint& delta) {
    auto points = getPoints();
    points.startPoint += delta;
    points.endPoint += delta;
    setPoints(points);

}

void Line::moveStart(const QPoint& delta)
{
    auto points = getPoints();
    points.startPoint += delta;
    setPoints(points);
}

void Line::moveEnd(const QPoint& delta)
{
    auto points = getPoints();
    points.endPoint += delta;
    setPoints(points);
}

// Реализация метода проверки попадания точки в отрезок
bool Line::contains(const QPoint& point) {
    const auto& points = getPoints();
    // Основная логика проверки принадлежности точки линии
    QPoint v1 = point - points.startPoint;
    QPoint v2 = points.endPoint - points.startPoint;

    double lengthSquared = v2.x() * v2.x() + v2.y() * v2.y();
    double dotProduct = v1.x() * v2.x() + v1.y() * v2.y();

    double projection = dotProduct / lengthSquared;

    if (projection < 0 || projection > 1) {
        return false;
    }

    QPoint closestPoint = points.startPoint + projection * v2;

    return (point - closestPoint).manhattanLength() < 5; // Допускаем небольшое расстояние для попадания
}

// Возвращает область квадрата в начале линии
QRect Line::getStartHandle() const {
    const auto& points = getPoints();
    return getHandle()->getHandle(HandleType::StartHandle, points);
}

QRect Line::getEndHandle() const {
    const auto& points = getPoints();
    return getHandle()->getHandle(HandleType::EndHandle, points);
}

QRect Line::getMiddleHandle() const {
    const auto& points = getPoints();
    return getHandle()->getHandle(HandleType::MiddleHandle, points);
}

bool Line::isHandleSelected(const HandleType& handle, const QPoint& point) const {
    const auto& points = getPoints();
    return getHandle()->isHandleSelected(handle, point, points);
}

void Line::captureCursorForAllHandles(QTabWidget* tabWidget, const QPoint& point, const ShapePoints& points)
{
    getHandle()->captureCursorForAllHandles(tabWidget, point, points);
}


HandleType Line::getHandleAt(const QPoint& point) {
    // Сбрасываем цвета хендлов
    resetColor();

    const auto& points = getPoints();
    // Проверка каждого хендла через HandleManager
    if (getHandle()->isHandleSelected(HandleType::StartHandle, point, points)) {
        if (getisSelected()) {
            setColorsStart(QColor(165, 0, 0));
        }
        setHandleState(HandleType::StartHandle);
        return HandleType::StartHandle;
    }

    if (getHandle()->isHandleSelected(HandleType::EndHandle, point, points)) {
        if (getisSelected()) {
            setColorEnd(QColor(165, 0, 0));
        }
        setHandleState(HandleType::EndHandle);
        return HandleType::EndHandle;
    }

    if (getHandle()->isHandleSelected(HandleType::MiddleHandle, point, points)) {
        if (getisSelected()) {
            setColorMiddle(QColor(165, 0, 0));
        }
        setHandleState(HandleType::MiddleHandle);
        return HandleType::MiddleHandle;
    }

    // Если ни один хендл не выбран
    setHandleState(HandleType::None);
    return HandleType::None;
}


// Функция для установки состояния хендлов
void Line::setHandleState(HandleType handleType) {
    setisStart(handleType == HandleType::StartHandle);
    setisEnd(handleType == HandleType::EndHandle);
    setisMiddle(handleType == HandleType::MiddleHandle);
}

