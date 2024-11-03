#pragma once
#include "HandleManager.h"
#include <QPoint>
#include <QTabWidget>
#include <QWidget>

HandleManager::HandleManager(int handleSize) : handleSize(handleSize) {}

QRect HandleManager::getHandle(HandleType type, const ShapePoints& points) const {
    QPoint middlePoint; // Инициализация переменной
    switch (type) {
    case HandleType::StartHandle:
        return QRect(points.startPoint.x() - handleSize / 2, points.startPoint.y() - handleSize / 2, handleSize, handleSize);
    case HandleType::EndHandle:
        return QRect(points.endPoint.x() - handleSize / 2, points.endPoint.y() - handleSize / 2, handleSize, handleSize);
    case HandleType::MiddleHandle:
        middlePoint = (points.startPoint + points.endPoint) / 2;
        return QRect(middlePoint.x() - handleSize / 2, middlePoint.y() - handleSize / 2, handleSize, handleSize);
    case HandleType::LeftHandle:
        return QRect(points.startPoint.x() - points.radius - handleSize / 2, points.startPoint.y() - handleSize / 2, handleSize, handleSize);
    case HandleType::RightHandle:
        return QRect(points.startPoint.x() + points.radius - handleSize / 2, points.startPoint.y() - handleSize / 2, handleSize, handleSize);
    case HandleType::TopHandle:
        return QRect(points.startPoint.x() - handleSize / 2, points.startPoint.y() - points.radius - handleSize / 2, handleSize, handleSize);
    case HandleType::BottomHandle:
        return QRect(points.startPoint.x() - handleSize / 2, points.startPoint.y() + points.radius - handleSize / 2, handleSize, handleSize);
    default:
        return QRect();
    }
}

bool HandleManager::isHandleSelected(const HandleType& handle, const QPoint& point, const ShapePoints& points) const {
    QRect handleRect = getHandle(handle, points);
    return handleRect.contains(point);
}

void HandleManager::captureCursorforHandle(QTabWidget* tabWidget, const HandleType& handle, const QPoint& point, const ShapePoints& points) {
    QRect handleRect = getHandle(handle, points);
    QWidget* currentTab = tabWidget->currentWidget();
   
    // Проверяем, находится ли курсор в области handleRect
    if (handleRect.contains(point)) {
        inHandle = true;       
        handlepoint = QPoint(handleRect.center().x() + 1, handleRect.center().y() + 1);
        // Получаем центр квадрата Handle
        QPoint centerHandle = handleRect.center();
        // Преобразуем локальные координаты в глобальные
        QPoint globalPos = currentTab->mapToGlobal(centerHandle);
        QCursor::setPos(globalPos);       
    }
}

void HandleManager::captureCursorForAllHandles(QTabWidget* tabWidget, const QPoint& point, const ShapePoints& points) {
    // Проходимся по всем типам HandleType
    for (HandleType handle : {HandleType::StartHandle, HandleType::EndHandle, HandleType::MiddleHandle,
        HandleType::LeftHandle, HandleType::RightHandle, HandleType::TopHandle, HandleType::BottomHandle}) {
        captureCursorforHandle(tabWidget, handle, point, points);
    }
}

const bool HandleManager::getInHandle() const {
    return inHandle;
}

void HandleManager::setInHandle(bool inHandle_)
{
	inHandle = inHandle_;
}

const QPoint HandleManager::getHandlepoint() const {
	return handlepoint;
}