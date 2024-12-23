#include "ShapeManager.h"

void ShapeManager::addTab(TabData tab) {
    tabDataList.push_back(tab);
}

void ShapeManager::addShape(std::shared_ptr<Shape> shape, int idx) {
    if (idx < tabDataList.size()) {
        tabDataList[idx].shapes.push_back(std::move(shape));
    }
}

const std::vector<std::shared_ptr<Shape>>& ShapeManager::getShapes(int idx) {
    static const std::vector<std::shared_ptr<Shape>> emptyVector;
    if (!tabDataList.isEmpty()) {
        if (idx >= tabDataList.size()) {
            return emptyVector;
           // tabDataList.resize(idx + 1);  
        }
        return tabDataList.at(idx).shapes;
    }
    else {
        return emptyVector;  // возвращаем ссылку на статический пустой вектор
    }
}

void ShapeManager::clearShapes(int idx) {
    tabDataList[idx].shapes.clear();
}

int ShapeManager::setDelataX(int idx, int dx) {
    if (!tabDataList.isEmpty()) {
        return  tabDataList[idx].delataX += dx;
    }
    else {
        return INT_MIN;
    }
}

int ShapeManager::setDelataY(int idx, int dy) {
    if (!tabDataList.isEmpty()) {
        return  tabDataList[idx].delataY += dy;
    }
    else {
        return INT_MIN;
    }
}

int ShapeManager::setgridSize(int idx, int size)
{
    if (!tabDataList.isEmpty()) {
        return  tabDataList[idx].gridSize = size;
    }
    else {
        return 37;
    }
}

int ShapeManager::getDelataX(int idx) const {
    if (!tabDataList.isEmpty()) {
        return  tabDataList[idx].delataX;
    }
    else {
        return INT_MIN;
    }
}

int ShapeManager::getDelataY(int idx) const {
    if (!tabDataList.isEmpty()) {
        return  tabDataList[idx].delataY;
    }
    else {
        return INT_MIN;
    }
}

int ShapeManager::getgridSize(int idx) const
{
    if (!tabDataList.isEmpty()) {
        return  tabDataList[idx].gridSize;
    }
    else {
        return 37;
    }
}

double ShapeManager::getScale(int idx) const
{
    if (!tabDataList.isEmpty()) {
        return  tabDataList[idx].scale;
    }
    else {
        return 1.0;
    }
}

void ShapeManager::scaleUp(int idx)
{
    if (!tabDataList.isEmpty()) {
          tabDataList[idx].scale /= 41.0 / 37.0;
    }
   
}

void ShapeManager::scaleDown(int idx)
{
    if (!tabDataList.isEmpty()) {
        tabDataList[idx].scale *= 41.0 / 37.0;
    }
}

// shapeManager->getShapes(idx())   shapeManager->setDelataX(idx(), int dx )