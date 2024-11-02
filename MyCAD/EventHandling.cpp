#include "EventHandling.h"
#include "MyCAD.h"

DrawMode currentDrawMode = DrawMode::None;
bool isdraw = false;
QPoint clickpoint = QPoint(INT_MIN, INT_MIN);
std::vector<bool> movingMiddles;
std::vector<bool> movingEnds;
std::vector<bool> movingStarts;
std::vector<bool> movingLefts;
std::vector<bool> movingTops;
std::vector<bool> movingRights;
std::vector<bool> movingBottoms;
std::vector<std::shared_ptr<Shape>>selShapes;
std::vector<std::shared_ptr<Shape>>tmpShapes;

EventHandling::EventHandling(QWidget* parent)
    : QMainWindow(parent), shapeManager(new ShapeManager()) {
    // Конструктор
}

EventHandling::~EventHandling() {
    // Деструктор
    delete shapeManager;
}

void EventHandling::mousePressEvent(QMouseEvent* event) {
    bool circleFlag = false; // Флаг для отслеживания рисования круга
    if (currentDrawMode != DrawMode::None) {
        isdraw = true; // Устанавливаем флаг рисования
    }

    if (isdraw && event->button() == Qt::LeftButton) {
        handleDrawing(event, circleFlag); // Обработка рисования фигуры
    }
    else if (event->button() == Qt::LeftButton) {
        handleSelection(event, circleFlag); // Обработка выделения фигуры
    }

    if (event->button() == Qt::MiddleButton) {
        isDragging = true; // Устанавливаем флаг перетаскивания
        lastMousePosition = event->pos(); // Сохраняем последнюю позицию мыши
    }
    QMainWindow::mousePressEvent(event); // Вызов базового метода
}

void EventHandling::mouseMoveEvent(QMouseEvent* event) {
    update();

    if (isDragging) { // Если мышь перетаскивается
        QPoint delta = event->pos() - lastMousePosition; // Рассчитываем смещение
        updateGridPosition(delta); // Обновляем позицию сетки
        lastMousePosition = event->pos(); // Обновляем последнюю позицию мыши        
    }

    QMainWindow::mouseMoveEvent(event); // Вызов базового метода
}

void EventHandling::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::MiddleButton) { // Проверяем, что отпущена средняя кнопка мыши
        if (chekTab()) {
            lastMousePosition = tabWidget->currentWidget()->mapFromGlobal(QCursor::pos());           
        }
        isDragging = false; // Устанавливаем флаг перетаскивания в false
    }

    if (chekTab()) {
        QWidget* currentTab = tabWidget->currentWidget();
        if (currentTab) {
            currentTab->update();  // Вызов перерисовки активного виджета
        }
    }
    QMainWindow::mouseReleaseEvent(event); // Вызов базового метода
}

void EventHandling::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Escape) {
        clearSelection();
    }
    QMainWindow::keyPressEvent(event);
}

bool EventHandling::event(QEvent* e) {
    if (e->type() == QEvent::HoverMove) {
        handleHoverMoveEvent(); // Обработка события перемещения мыши
    }
    return QWidget::event(e); // Вызов базового метода
}

void EventHandling::handleHoverMoveEvent() {
    if (chekTab()) {
        int currentIndex = tabWidget->currentIndex();
        if (shapesNoEmpt()) {
            processShapeSelection(currentIndex); // Обработка выделения фигур        
        }
        if (!isdraw) {
            highlightShapesUnderCursor(currentIndex); // Подсветка фигур под курсором
        }
        update(); // Обновление виджета
        if (currentDrawMode == DrawMode::None && !isDragging) {
            updateShapePositions(); // Обновление позиций фигур
        }
    }
}

void EventHandling::addTab(TabData tab) {
    shapeManager->addTab(tab);
}

const int EventHandling::idx() const {
  return  tabWidget->currentIndex();
}

const std::vector<std::shared_ptr<Shape>>& EventHandling::getShapes() const
{
    return shapeManager->getShapes(idx());
}

const int EventHandling::setDelataX(int dx) const {
    return  shapeManager->setDelataX(idx(), dx);
    }

const int EventHandling::setDelataY(int dy) const {
    return  shapeManager->setDelataX(idx(), dy);
}

const int EventHandling::getDelataX() const {
    return  shapeManager->getDelataX(idx());
}

const int EventHandling::getDelataY() const {
    return  shapeManager->getDelataY(idx());
}

void EventHandling::handleDrawing(QMouseEvent* event, bool& circleFlag) {
    if (tabWidget->rect().contains(event->pos())) {
        if (chekTab()) {

            QPoint localPos = tabWidget->mapFromGlobal(event->globalPosition().toPoint());
            QRect tabBarRect = tabWidget->geometry();
            if (tabBarRect.contains(localPos)) {
                QPoint newpoint = GetCurrPoint();

                if (clickpoint != QPoint(INT_MIN, INT_MIN)) {
                    if (currentDrawMode == DrawMode::Line) {
                        auto line = std::make_unique<Line>(clickpoint, newpoint);
                        shapeManager->addShape(std::move(line), idx());
                    }
                    else if (currentDrawMode == DrawMode::Circle) {
                        int radius = std::hypot(newpoint.x() - clickpoint.x(), newpoint.y() - clickpoint.y());
                        auto circle = std::make_unique<Circle>(clickpoint, radius);
                        shapeManager->addShape(std::move(circle), idx());
                        currentDrawMode = DrawMode::None;
                        clearSelection();
                        circleFlag = true;
                    }
                }
                clickpoint = newpoint;
            }
        }
    }
}


void EventHandling::handleSelection(QMouseEvent* event, bool circleFlag) {
    if (!selShapes.empty() && !isdraw && !movingStarts.empty()) {
        for (int i = 0; i < selShapes.size(); i++) {
            updateShapeCoordinates(i);
        }
        resetShapeColors();
        clearvectors();
        update();
    }
    else if (tabWidget->rect().contains(event->pos())) {
        selectShapes(event);
    }

    if (chekTab()) {
        lastMousePosition = tabWidget->currentWidget()->mapFromGlobal(QCursor::pos());
    }

    if (!isdraw && !circleFlag) {
        highlightShapes(event);
    }
}

void EventHandling::updateShapeCoordinates(int i) {
    if (movingMiddles[i]) {
        if (tmpShapes[i]->name() == Type::line) {
            tmpShapes[i]->setCoords(selShapes[i]->getstartPoint(), selShapes[i]->getendPoint(), tmpShapes[i]->getisSelected());
        }
    }
    if (movingStarts[i]) {
        if (tmpShapes[i]->name() == Type::line) {
            tmpShapes[i]->setCoords(selShapes[i]->getstartPoint(), tmpShapes[i]->getendPoint(), tmpShapes[i]->getisSelected());
        }
        else if (tmpShapes[i]->name() == Type::circle) {
            tmpShapes[i]->setCentre(selShapes[i]->getstartPoint(), tmpShapes[i]->getradius(), tmpShapes[i]->getisSelected());
        }
    }
    if (movingEnds[i]) {
        if (tmpShapes[i]->name() == Type::line) {
            tmpShapes[i]->setCoords(tmpShapes[i]->getstartPoint(), selShapes[i]->getendPoint(), tmpShapes[i]->getisSelected());
        }
    }
    if (movingLefts[i] || movingTops[i] || movingRights[i] || movingBottoms[i]) {
        if (tmpShapes[i]->name() == Type::circle) {
            tmpShapes[i]->setCentre(tmpShapes[i]->getstartPoint(), selShapes[i]->getradius(), tmpShapes[i]->getisSelected());
        }
    }
}

void EventHandling::resetShapeColors() {
    if (shapesNoEmpt()) {
        int currentIndex = tabWidget->currentIndex();
        for (const auto& shape : getShapes()) {
            shape->resetColor();
        }
    }
}

QPoint EventHandling::GetCurrPoint() {
    QPoint globalPos = QCursor::pos();
    QWidget* currentTab = tabWidget->currentWidget();
    return currentTab->mapFromGlobal(globalPos);
}

void EventHandling::selectShapes(QMouseEvent* event) {
    if (chekTab()) {
        QPoint newpoint = GetCurrPoint();
        for (const auto& shape : shapeManager->getShapes(idx())) {
            HandleType handle = shape->getHandleAt(newpoint);
            if (shape->getisSelected() && shape->isHandleSelected(handle, newpoint)) {
                selShapes.push_back(shape->clone());
                selShapes.back()->setSelected(shape->getisSelected());
                tmpShapes.push_back(shape);
                movingPush(handle, shape->getisSelected());
            }
        }
    }
}

void EventHandling::highlightShapes(QMouseEvent* event) {
    if (chekTab()) {
        QPoint newpoint = GetCurrPoint();
        bool isanyshapeselectedandhandled = false;
        if (!getShapes().empty()) {
            for (const auto& shape : shapeManager->getShapes(idx())) {
                if (shape->getisSelected() && shape->getHandleAt(newpoint) != HandleType::None) {
                    isanyshapeselectedandhandled = true;
                }
            }
           
            for (auto it = getShapes().rbegin(); it != getShapes().rend(); ++it) {
                if ((*it)->contains(newpoint)) {
                    HandleType handle = (*it)->getHandleAt(newpoint);
                    if (handle == HandleType::None || selShapes.size() == 0) {
                        if (!isanyshapeselectedandhandled) {
                            (*it)->setSelected(true);
                        }
                    }
                    break;
                }
            }
        }
    }
}

void EventHandling::processShapeSelection(int currentIndex) {
    for (const auto& shape : getShapes()) {
        if (shape->getisSelected()) {
            QPoint newpoint = GetCurrPoint();
            shape->captureCursorForAllHandles(tabWidget, newpoint, shape->getPoints());
        }
    }
}

void EventHandling::highlightShapesUnderCursor(int currentIndex) {
    if (chekTab()) {
        QPoint newpoint = GetCurrPoint();
        bool isanyshapeselectedandhandled = false;
        
        if (!shapeManager->getShapes(idx()).empty()) {
            for (const auto& shape : shapeManager->getShapes(idx())) {
                if (selShapes.size() != 0 && (!movingStarts.empty())) {
                    isanyshapeselectedandhandled = true;
                    break;
                }
                shape->setisover(false);
            }
            for (auto it = shapeManager->getShapes(idx()).rbegin(); it != shapeManager->getShapes(idx()).rend(); ++it) {
                if (!(*it)->contains(newpoint)) {
                    (*it)->setisover(false);
                }
                else {
                    if (!isanyshapeselectedandhandled && !(*it)->getisSelected()) {
                        (*it)->setisover(true);
                    }
                    break;
                }
            }
        }
    }
}

void EventHandling::updateShapePositions() {
    if (!selShapes.empty() && !movingStarts.empty()) {
        QPoint newpoint = GetCurrPoint();
        QPoint delta = newpoint - lastMousePosition;
        for (int i = 0; i < selShapes.size(); i++) {
            bool temp = selShapes[i]->getisSelected();
            if (selShapes[i]->getisSelected()) {
                selShapes[i]->setMoveSelected(true);
                if (movingMiddles[i]) {
                    selShapes[i]->move(delta);
                }
                if (movingStarts[i]) {
                    selShapes[i]->moveStart(delta);
                }
                if (movingEnds[i]) {
                    selShapes[i]->moveEnd(delta);
                }
                if (movingLefts[i] || movingTops[i] || movingRights[i] || movingBottoms[i]) {
                    int radius = std::hypot(newpoint.x() - selShapes[i]->getstartPoint().x(), newpoint.y() - selShapes[i]->getstartPoint().y());
                    selShapes[i]->moveRadius(radius);
                }
            }
        }
        lastMousePosition = newpoint;
    }
}

void EventHandling::updateGridPosition(const QPoint& delta)
{
    // Проверка, что индекс корректный и вкладки существуют
    if (chekTab()) {
        int currentIndex = tabWidget->currentIndex();
        // Обновляем значения смещения сетки на основе переданного delta
        shapeManager->setDelataX(idx(), delta.x());
        shapeManager->setDelataY(idx(), delta.y());
  
        if (isdraw && clickpoint != QPoint(INT_MIN, INT_MIN)) {
            clickpoint = QPoint(clickpoint.x() + delta.x(), clickpoint.y() + delta.y());
        }

        // Перерисовываем текущий активный виджет
        QWidget* currentTab = tabWidget->currentWidget();
        if (currentTab) {
            currentTab->update();  // Вызов перерисовки виджета
        }
        // if (selShape != nullptr) { selShape->move(delta); }
        if (!selShapes.empty()) {
            for (const auto& shape : selShapes)
            {
                shape->move(delta);
            }
        }
        // Рисуем фигуры только для активной вкладки
        for (const auto& shape : getShapes()) {
            shape->move(delta);
        }

    }
}

const bool EventHandling::chekTab() const {
    bool result = false;
    if (tabWidget != nullptr) {
        int currentIndex = tabWidget->currentIndex();
        if (currentIndex >= 0 /*&& currentIndex < tabDataList.size()*/) {
            result = true;
        }
    }
    return result;
}

void EventHandling::movingPush(HandleType handle, bool isselected)
{
    if (isselected) {
        movingEnds.push_back(handle == HandleType::EndHandle);
        movingStarts.push_back(handle == HandleType::StartHandle);
        movingMiddles.push_back(handle == HandleType::MiddleHandle);
        movingLefts.push_back(handle == HandleType::LeftHandle);
        movingTops.push_back(handle == HandleType::TopHandle);
        movingRights.push_back(handle == HandleType::RightHandle);
        movingBottoms.push_back(handle == HandleType::BottomHandle);
    }
}

const bool EventHandling::shapesNoEmpt()const
{
    bool result = false;
    if (chekTab()) {
       
        if (!getShapes().empty())
        {
            result = true;
        }
    }
    return result;
}

void EventHandling::clearvectors()
{
    tmpShapes.clear();
    tmpShapes.resize(0);
    selShapes.clear();
    selShapes.resize(0);
    movingMiddles.clear();
    movingStarts.clear();
    movingEnds.clear();
    movingLefts.clear();
    movingTops.clear();
    movingRights.clear();
    movingBottoms.clear();

}

void EventHandling::clearSelection()
{
    clickpoint = QPoint(INT_MIN, INT_MIN);
    isdraw = false;
    currentDrawMode = DrawMode::None;
    clearvectors();
    update();

    // Проверяем, что currentIndex находится в допустимых пределах
    if (shapesNoEmpt()) {
        int currentIndex = tabWidget->currentIndex();
        // Снимаем выделение со всех фигур
        for (const auto& shape : getShapes()) {
            shape->setSelected(false);
        }
    }
}
