#include "Shape.h"
#include <QDebug>
#include <QTabWidget>
#include <QWidget>

const ShapePoints& Shape::getPoints() const
{
    return points;
}

void Shape::setPoints(const ShapePoints& newPoints)
{
    points = newPoints;
}

void Shape::setHandleSize(int size)
{
    handleManager = new HandleManager(size);
}

HandleManager* Shape::getHandle() const
{
    return handleManager;
}

Shape::~Shape()
{
    delete handleManager;
}

Colors Shape::getColors() const
{
    return colors;
}

void Shape::setColorsStart(QColor color)
{
	colors.ColorStartPoint = color;

}

void Shape::setColorEnd(QColor color)
{
	colors.ColorEndPoint = color;
}

void Shape::setColorMiddle(QColor color)
{
	colors.ColorMiddlePoint = color;
}

void Shape::setColorsLeft(QColor color)
{
	colors.ColorLeftPoint = color;
}

void Shape::setColorsTop(QColor color)
{
	colors.ColorTopPoint = color;
}

void Shape::setColorsRight(QColor color)
{
	colors.ColorRightPoint = color;
}

void Shape::setColorsBottom(QColor color)
{
	colors.ColorBottomPoint = color;
}

Selected Shape::getSelected()  const
{
    return selected;
}

void Shape::setisStart(bool isSelected)
{
    selected.isStart = isSelected;
}

void Shape::setisMiddle(bool isSelected)
{
    selected.isMiddle = isSelected;
}

void Shape::setisEnd(bool isSelected)
{
    selected.isEnd = isSelected;
}

void Shape::setisLeft(bool isSelected)
{
    selected.isLeft = isSelected;
}

void Shape::setisTop(bool isSelected)
{
    selected.isTop = isSelected;
}

void Shape::setisRight(bool isSelected)
{
    selected.isRight= isSelected;
}

void Shape::setisBottom(bool isSelected)
{
    selected.isBottom = isSelected;
}

void Shape::move(const QPoint& delta)
{
}

void Shape::moveStart(const QPoint& delta)
{
}

void Shape::moveEnd(const QPoint& delta)
{
}

void Shape::moveRadius(const int radius)
{
}

void Shape::setSelected(bool selected)
{
    isSelected = selected;
}

bool Shape::getisSelected() const
{
    return isSelected;
}

void Shape::setMoveSelected(bool selected)
{
    moveSelected = selected;
}

bool Shape::getisMoveSelected() const
{
    return moveSelected;
}

void Shape::resetColor()
{
    colors.ColorStartPoint = QColor(0, 127, 255);
    colors.ColorEndPoint = QColor(0, 127, 255);
    colors.ColorMiddlePoint = QColor(0, 127, 255);
    colors.ColorLeftPoint = QColor(0, 127, 255);
    colors.ColorTopPoint = QColor(0, 127, 255);
    colors.ColorRightPoint = QColor(0, 127, 255);
    colors.ColorBottomPoint = QColor(0, 127, 255);

    selected.isStart = false;  // Флаг выделения
    selected.isEnd = false;  // Флаг выделения
    selected.isMiddle = false;  // Флаг выделения 
    selected.isLeft = false;  // Флаг выделения
    selected.isTop = false;  // Флаг выделения
    selected.isRight = false;  // Флаг выделения
    selected.isBottom = false;  // Флаг выделения
}

bool Shape::getisStart() const
{
    return false;
}

bool Shape::getisEnd() const
{
    return false;
}

bool Shape::getisMiddle() const
{
    return false;
}

bool Shape::getisLeft() const
{
    return false;
}

bool Shape::getisRight() const
{
    return false;
}

bool Shape::getisTop() const
{
    return false;
}

bool Shape::getisBottom() const
{
    return false;
}

void Shape::setisover(bool iscursorhovershape_)
{
    iscursorhovershape = iscursorhovershape_;
}

bool Shape::getisover() const
{
    return iscursorhovershape;
}

QRect Shape::getStartHandle() const
{
    return QRect();
}

QRect Shape::getEndHandle() const
{
    return QRect();
}

QRect Shape::getMiddleHandle() const
{
    return QRect();
}

QRect Shape::getLeftHandle() const
{
    return QRect();
}

QRect Shape::getToptHandle() const
{
    return QRect();
}

QRect Shape::getRighttHandle() const
{
    return QRect();
}

QRect Shape::getBottomtHandle() const
{
    return QRect();
}

void Shape::setHandleState(HandleType handleType)
{
    setisStart(handleType == HandleType::StartHandle);
    setisEnd(handleType == HandleType::EndHandle);
    setisMiddle(handleType == HandleType::MiddleHandle);
    setisLeft(handleType == HandleType::LeftHandle);
    setisTop(handleType == HandleType::TopHandle);
    setisRight(handleType == HandleType::RightHandle);
    setisBottom(handleType == HandleType::BottomHandle);
}

void Shape::setCoords(const QPoint& startPoint, const QPoint& endPoint, bool isSelected)
{
}

void Shape::setCentre(const QPoint& startPoint, const int radius, bool isSelected)
{
}

QPoint Shape::getstartPoint() const
{
    return QPoint();
}

QPoint Shape::getendPoint() const
{
    return QPoint();
}

QPoint Shape::getmiddlePoint() const
{
    return QPoint();
}

int Shape::getradius() const
{
    return 0;
}

void Shape::setcolorPoint() const
{
}

QColor Shape::getColorPoint() const
{
    return QColor();
}

void Shape::captureCursorForAllHandles(QTabWidget* tabWidget, const QPoint& point, const ShapePoints& points)
{
    getHandle()->captureCursorForAllHandles(tabWidget, point, points);
}

bool Shape::isHandleSelected(const HandleType& handle, const QPoint& point) const {
    const auto& points = getPoints();
    return getHandle()->isHandleSelected(handle, point, points);
}
