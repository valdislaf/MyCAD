#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MyCAD.h"

struct TabData {
    int delataX = 0;
    int delataY = 0;
};

class MyCAD : public QMainWindow
{
    Q_OBJECT

public:
    MyCAD(QWidget* parent = nullptr);
    ~MyCAD();

protected:
    void mousePressEvent(QMouseEvent* event) override;
    bool eventFilter(QObject* obj, QEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    QVector<TabData> tabDataList; // Список данных для каждой вкладки

private slots:  // Методы, связанные с сигналами
    void onExitThis();
    void onCloseThisTab();
    void onDrawLine();
    void onTabChanged(int index);

private:  // Обычные методы
    void createNewWindow();
    void updateMenusBasedOnTabWidgetVisibility();
    void initialTabWidget();
    QCursor createCustomCrossCursor();
    void drawGrid();
    void setupTabWidgetStyle();
    void CoordinateAxes(QPainter& painter, QWidget* currentTab);

private:
    Ui::MyCADClass ui;

private:
    bool isDragging = false;  // Флаг для отслеживания состояния перетаскивания
    QPoint lastMousePosition; // Последняя позиция мыши
    QPoint offset;            // Смещение от начальной позиции

    void updateGridPosition(const QPoint& delta); // Метод для обновления позиции сетки

};
