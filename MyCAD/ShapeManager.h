#pragma once

#include <vector>
#include <memory>
#include "Shape.h"

struct TabData {
    int delataX = 0;
    int delataY = 0;
    std::vector<std::shared_ptr<Shape>> shapes;  // Список фигур для этой вкладки
};

class ShapeManager {
public:
    void addTab(TabData tab);
    void addShape(std::shared_ptr<Shape> shape, int idx);
    const std::vector<std::shared_ptr<Shape>>& getShapes(int idx) ;
    void clearShapes(int idx);

    int setDelataX(int idx, int dx);

    int setDelataY(int idx, int dy);

    int getDelataX(int idx)const;

    int getDelataY(int idx)const;

private:    
    QVector<TabData> tabDataList; // Список данных для каждой вкладки
};