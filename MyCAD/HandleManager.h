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

class HandleManager {

public:
    HandleManager(int handleSize);

    QRect getHandle(HandleType type, const ShapePoints& points) const;

    bool isHandleSelected(const HandleType& handle, const QPoint& point, const ShapePoints& points) const;

    void captureCursorforHandle(QTabWidget* tabWidget, const HandleType& handle, const QPoint& point, const ShapePoints& points);

    //void captureCursorForAllHandles(QTabWidget* tabWidget, const QPoint& point, const QPoint& startPoint, const QPoint& endPoint, int radius);
    void captureCursorForAllHandles(QTabWidget* tabWidget, const QPoint& point, const ShapePoints& points);
private:

    int handleSize;

};