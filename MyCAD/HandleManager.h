#pragma once
#include <QRect>
#include <QPoint>
#include <QCursor>
#include <QTabWidget>
#include <QWidget>

enum class HandleType {
    None,
    StartHandle,
    EndHandle,
    MiddleHandle,
    LeftHandle,
    RightHandle,
    TopHandle,
    BottomHandle
};

enum Type {
    line,
    circle
};

struct ShapePoints {
    QPoint startPoint;
    QPoint endPoint;
    int radius;
};

struct Colors {
    QColor   ColorStartPoint = QColor(0, 127, 255);
    QColor  ColorEndPoint = QColor(0, 127, 255);
    QColor ColorMiddlePoint = QColor(0, 127, 255);
    QColor  ColorLeftPoint = QColor(0, 127, 255);
    QColor  ColorTopPoint = QColor(0, 127, 255);
    QColor  ColorRightPoint = QColor(0, 127, 255);    
    QColor ColorBottomPoint = QColor(0, 127, 255);
};

struct Selected {
    bool  isStart = false;  // Флаг выделения
    bool isEnd = false;  // Флаг выделения
    bool isMiddle = false;  // Флаг выделения 
    bool isLeft = false;  // Флаг выделения
    bool isTop = false;  // Флаг выделения
    bool isRight = false;  // Флаг выделения
    bool isBottom = false;  // Флаг выделения
};

class HandleManager {

public:

    HandleManager(int handleSize);

    QRect getHandle(HandleType type, const ShapePoints& points) const;

    bool isHandleSelected(const HandleType& handle, const QPoint& point, const ShapePoints& points) const;

    void captureCursorforHandle(QTabWidget* tabWidget, const HandleType& handle, const QPoint& point, const ShapePoints& points);

    void captureCursorForAllHandles(QTabWidget* tabWidget, const QPoint& point, const ShapePoints& points);

    const bool getInHandle() const;

    void setInHandle(bool inHandle_);

    const QPoint getHandlepoint() const;

private:

    int handleSize;
	bool inHandle = false;
    QPoint handlepoint;
};