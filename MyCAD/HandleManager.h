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

    bool isHandleSelected(const QRect& handleRect, const QPoint& point) const;

private:
    int handleSize;
};