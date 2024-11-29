#include "ShapeManager.h"

void ShapeManager::addTab(TabData tab) {
    tabDataList.push_back(tab);
}

void ShapeManager::addShape(std::shared_ptr<Shape> shape, int idx) {
    if (idx >= tabDataList.size()) {
        tabDataList.resize(idx + 1);  // �������� ������ �� ������� �������
    }
    tabDataList[idx].shapes.push_back(std::move(shape));
}

const std::vector<std::shared_ptr<Shape>>& ShapeManager::getShapes(int idx) {
    static const std::vector<std::shared_ptr<Shape>> emptyVector;
    if (!tabDataList.isEmpty()) {
        if (idx >= tabDataList.size()) {
            tabDataList.resize(idx + 1);  // �������� ������ �� ������� �������
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

// shapeManager->getShapes(idx())   shapeManager->setDelataX(idx(), int dx )