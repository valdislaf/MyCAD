#pragma once
#include "HandleManager.h"
#include <QPoint>
#include <QTabWidget>
#include <QWidget>

HandleManager::HandleManager(int handleSize) : handleSize(handleSize) {}

QRect HandleManager::getHandle(HandleType type, const QPoint& startPoint, const QPoint& endPoint, int radius) const {
    QPoint middlePoint; // Инициализация переменной
    switch (type) {
    case HandleType::StartHandle:
        return QRect(startPoint.x() - handleSize / 2, startPoint.y() - handleSize / 2, handleSize, handleSize);
    case HandleType::EndHandle:
        return QRect(endPoint.x() - handleSize / 2, endPoint.y() - handleSize / 2, handleSize, handleSize);
    case HandleType::MiddleHandle:
        middlePoint = (startPoint + endPoint) / 2;
        return QRect(middlePoint.x() - handleSize / 2, middlePoint.y() - handleSize / 2, handleSize, handleSize);
    case HandleType::LeftHandle:
        return QRect(startPoint.x() - radius - handleSize / 2, startPoint.y() - handleSize / 2, handleSize, handleSize);
    case HandleType::RightHandle:
        return QRect(startPoint.x() + radius - handleSize / 2, startPoint.y() - handleSize / 2, handleSize, handleSize);
    case HandleType::TopHandle:
        return QRect(startPoint.x() - handleSize / 2, startPoint.y() - radius - handleSize / 2, handleSize, handleSize);
    case HandleType::BottomHandle:
        return QRect(startPoint.x() - handleSize / 2, startPoint.y() + radius - handleSize / 2, handleSize, handleSize);
    default:
        return QRect();
    }
}

bool HandleManager::isHandleSelected(const HandleType& handle, const QPoint& point, const QPoint& startPoint, const QPoint& endPoint, int radius) const {
    QRect handleRect = getHandle(handle, startPoint, endPoint, radius);
    return handleRect.contains(point);
}

void HandleManager::captureCursorforHandle(QTabWidget* tabWidget, const HandleType& handle, const QPoint& point, const QPoint& startPoint, const QPoint& endPoint, int radius) {
    QRect handleRect = getHandle(handle, startPoint, endPoint, radius);
    QWidget* currentTab = tabWidget->currentWidget();
  
    // Проверяем, находится ли курсор в области handleRect
    if (handleRect.contains(point)) {
        // Получаем центр квадрата Handle
        QPoint centerHandle = handleRect.center();

        // Преобразуем локальные координаты в глобальные
        QPoint globalPos = currentTab->mapToGlobal(centerHandle);

        // Если курсор не в центре, возвращаем его обратно
        if (point != centerHandle) {
            QCursor::setPos(globalPos);
        }
    }
}