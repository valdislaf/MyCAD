#pragma once
#include <QRect>
#include <QPoint>

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

class HandleManager {

public:
    HandleManager(int handleSize);

    QRect getHandle(HandleType type, const QPoint& startPoint, const QPoint& endPoint = QPoint(), int radius = 0) const;

    bool isHandleSelected(const HandleType& handle, const QPoint& point, const QPoint& startPoint, const QPoint& endPoint, int radius) const;


private:
    int handleSize;

};