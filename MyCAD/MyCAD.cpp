#include <QBitmap>
#include <QCursor>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QTabBar>
#include <QTabWidget>
#include <QWidget>
#include <QMenuBar>
#include <QDebug>

#include "MyCAD.h"
#include "MenuInit.h"
#include "MenuMain.h"
#include "Grid.h"
#include "CrossCursor.h"

bool isdraw = false;
DrawMode currentDrawMode = DrawMode::None;
int heightwindow_prev = 0;
QPoint clickpoint = QPoint(INT_MIN, INT_MIN);
std::vector<std::shared_ptr<Shape>>selShapes;
std::vector<std::shared_ptr<Shape>>tmpShapes;
std::vector<bool> movingMiddles;
std::vector<bool> movingEnds;
std::vector<bool> movingStarts;
std::vector<bool> movingLefts;
std::vector<bool> movingTops;
std::vector<bool> movingRights;
std::vector<bool> movingBottoms;

MyCAD::MyCAD(QWidget* parent)
    : QMainWindow(parent) {

    tabWidget = new QTabWidget(this);  // Инициализация tabWidget
    menuBar = new QMenuBar(this);  // Создаем QMenuBar
    setMenuBar(menuBar);
    tabWidget->hide();
    // Настройка стиля для tabWidget
    setupTabWidgetStyle();

    // Проверяем, что menuBar действительно инициализирован и доступен
    if (menuBar) {
        updateMenusBasedOnTabWidgetVisibility();
    }
    // Подписываемся на сигнал смены вкладок
    connect(tabWidget, &QTabWidget::currentChanged, this, &MyCAD::onTabChanged);
    setFocus();
}

MyCAD::~MyCAD()
{
    delete tabWidget;
    delete menuBar;
}

// Обработчик события нажатия кнопки мыши
void MyCAD::mousePressEvent(QMouseEvent* event)
{
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

// Обработчик рисования фигур
void MyCAD::handleDrawing(QMouseEvent* event, bool& circleFlag) {
    if (tabWidget->rect().contains(event->pos())) {

        if (chekTab()) {
            QPoint localPos = tabWidget->mapFromGlobal(event->globalPosition().toPoint());
            QRect tabBarRect = tabWidget->geometry();
            if (tabBarRect.contains(localPos)) {
                QPoint newpoint = GetCurrPoint();

                if (clickpoint != QPoint(INT_MIN, INT_MIN)) {
                    if (currentDrawMode == DrawMode::Line) {
                        auto line = std::make_unique<Line>(clickpoint, newpoint);
                        addShape(std::move(line));
                    }
                    else if (currentDrawMode == DrawMode::Circle) {
                        int radius = std::hypot(newpoint.x() - clickpoint.x(), newpoint.y() - clickpoint.y());
                        auto circle = std::make_unique<Circle>(clickpoint, radius);
                        addShape(std::move(circle));
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

// Обработчик выделения фигур
void MyCAD::handleSelection(QMouseEvent* event, bool circleFlag) {
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

// Обновление координат фигур
void MyCAD::updateShapeCoordinates(int i) {
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

bool MyCAD::shapesNoEmpt()
{
    bool result = false;
    if (chekTab()) {
        int currentIndex = tabWidget->currentIndex();
        if (!tabDataList[currentIndex].shapes.empty()) 
        {
            result = true;
        }
    }
    return result;
}

// Сброс цвета фигур
void MyCAD::resetShapeColors() {
    if (shapesNoEmpt()) {
        int currentIndex = tabWidget->currentIndex();
        for (const auto& shape : tabDataList[currentIndex].shapes) {
            shape->resetColor();
        }
    }
}

QPoint  MyCAD::GetCurrPoint()
{
    QPoint globalPos = QCursor::pos();
    QWidget* currentTab = tabWidget->currentWidget();
    return currentTab->mapFromGlobal(globalPos);
}

// Выбор фигур
void MyCAD::selectShapes(QMouseEvent* event) {

    if (chekTab()) {        
        QPoint newpoint = GetCurrPoint();
        for (const auto& shape : tabDataList[tabWidget->currentIndex()].shapes) {
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

// Подсветка фигур
void MyCAD::highlightShapes(QMouseEvent* event) {

    if (chekTab()) {
        QPoint newpoint = GetCurrPoint();
        bool isanyshapeselectedandhandled = false;

        for (const auto& shape : tabDataList[tabWidget->currentIndex()].shapes) {
            if (shape->getisSelected() && shape->getHandleAt(newpoint) != HandleType::None) {
                isanyshapeselectedandhandled = true;
            }
        }
        int currentIndex = tabWidget->currentIndex();
        for (auto it = tabDataList[currentIndex].shapes.rbegin(); it != tabDataList[currentIndex].shapes.rend(); ++it) {
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

void MyCAD::mouseMoveEvent(QMouseEvent* event)
{

    update();

    if (isDragging) // Если мышь перетаскивается
    {

        QPoint delta = event->pos() - lastMousePosition; // Рассчитываем смещение
        updateGridPosition(delta); // Обновляем позицию сетки
        lastMousePosition = event->pos(); // Обновляем последнюю позицию мыши        
    }

    QMainWindow::mouseMoveEvent(event); // Вызов базового метода
}

void MyCAD::mouseReleaseEvent(QMouseEvent* event)
{

    if (event->button() == Qt::MiddleButton) // Проверяем, что отпущена средняя кнопка мыши
    {

        if (chekTab())
        {
            lastMousePosition = tabWidget->currentWidget()->mapFromGlobal(QCursor::pos());
        }

        isDragging = false; // Устанавливаем флаг перетаскивания в false
    }

    if (chekTab()) {
        // Перерисовываем активную вкладку
        QWidget* currentTab = tabWidget->currentWidget();
        if (currentTab) {
            currentTab->update();  // Вызов перерисовки активного виджета
        }
    }
    QMainWindow::mouseReleaseEvent(event); // Вызов базового метода
}

void MyCAD::keyPressEvent(QKeyEvent* event) {
    // Проверяем, что нажата клавиша ESC  
    if (event->key() == Qt::Key_Escape) {
        clearSelection();
    }

    QMainWindow::keyPressEvent(event);
}

bool MyCAD::event(QEvent* e) {
    if (e->type() == QEvent::HoverMove) {
        handleHoverMoveEvent(); // Обработка события перемещения мыши
    }
    return QWidget::event(e); // Вызов базового метода
}

// Обработка события перемещения мыши
void MyCAD::handleHoverMoveEvent() {
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

// Обработка выделения фигур
// Обновленный метод processShapeSelection
void MyCAD::processShapeSelection(int currentIndex) {
    for (const auto& shape : tabDataList[currentIndex].shapes) {
        if (shape->getisSelected()) {
            QPoint newpoint = GetCurrPoint();
            shape->captureCursorForAllHandles(tabWidget, newpoint, shape->getPoints());
        }
    }
}


// Подсветка фигур под курсором
void MyCAD::highlightShapesUnderCursor(int currentIndex) {
    if (chekTab()) {
        QPoint newpoint = GetCurrPoint();
        bool isanyshapeselectedandhandled = false;
        for (const auto& shape : tabDataList[currentIndex].shapes) {
            if (selShapes.size() != 0 && (!movingStarts.empty())) {
                isanyshapeselectedandhandled = true;
                break;
            }
            shape->setisover(false);
        }
        for (auto it = tabDataList[currentIndex].shapes.rbegin(); it != tabDataList[currentIndex].shapes.rend(); ++it) {
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

// Обновление позиций фигур
void MyCAD::updateShapePositions() {
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

bool MyCAD::chekTab() {
    bool result = false;
    if (tabWidget != nullptr) {
        int currentIndex = tabWidget->currentIndex();
        if (currentIndex >= 0 && currentIndex < tabDataList.size()) {
            result = true;
        }
    }
    return result;
}

void MyCAD::onTabChanged(int index)
{
    clearSelection();

    // Обработка изменения вкладки, например, загрузка настроек сетки для выбранной вкладки
    if (index >= 0) {
        QWidget* currentTab = tabWidget->widget(index);
    }
}

void MyCAD::movingPush(HandleType handle, bool isselected)
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

QCursor MyCAD::createCustomCrossCursor()
{
    // Создаем курсор
    CrossCursor cursor(97, QPen(Qt::white, 1), 3);
    return cursor.createCursor();
}

QCursor MyCAD::createCustomCrossCursorIn()
{
    // Создаем курсор
    CrossCursor cursor(97, QPen(Qt::white, 1), 0);
    return cursor.createCursor();
}

void MyCAD::onExitThis()
{
    // Создаем диалоговое окно с вопросом о подтверждении выхода
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Подтверждение выхода", "Вы уверены, что хотите выйти?",
        QMessageBox::Yes | QMessageBox::No);

    // Если пользователь выбрал "Yes", закрываем окно
    if (reply == QMessageBox::Yes) {
        close();
    }
    // Если пользователь выбрал "No", ничего не делаем (окно остается открытым)
}

void MyCAD::onCloseThisTab()
{
    // Здесь будет логика, которая выполняется при нажатии на пункт "Закрыть"
    QMessageBox::information(this, "Закрыть", "Вы выбрали 'Закрыть' из меню!");
}

void MyCAD::setupTabWidgetStyle()
{
    // Применяем стиль к tabWidget
    tabWidget->setStyleSheet("QTabWidget { background-color: rgb(33, 40, 48); }");
}

void MyCAD::createNewWindow()
{
    // Если QTabWidget был скрыт, показываем его при создании первого чертежа
    if (!tabWidget->isVisible()) {
        tabWidget->show();
        updateMenusBasedOnTabWidgetVisibility();
    }
    DrawingWidget* newDrawingWidget = new DrawingWidget(this);
    int tabIndex = tabWidget->addTab(std::move(newDrawingWidget), tr("Чертеж %1").arg(tabWidget->count() + 1));
    //int tabIndex = tabWidget->addTab(newDrawingWidget, tr("Чертеж %1").arg(tabWidget->count() + 1));

    // Переключаемся на только что созданную вкладку
    tabWidget->setCurrentIndex(tabIndex);

    // Инициализируем данные для новой вкладки
    TabData newTabData;
    newTabData.delataX = 10; // Устанавливаем начальные значения
    newTabData.delataY = -10;
    tabDataList.push_back(newTabData); // Добавляем данные вкладки в список

    // Переключаемся на только что созданную вкладку
    tabWidget->setCurrentIndex(tabIndex);

    // Убедитесь, что QTabWidget занимает все пространство центрального виджета
    setCentralWidget(tabWidget);

    DrawingWidget* widget = dynamic_cast<DrawingWidget*>(tabWidget->widget(tabIndex));
    if (widget) {
        widget->MyMethod();  // Вызываем метод
    }
    else {
        // qDebug() << "Ошибка приведения типа!";
    }
}

void MyCAD::updateMenusBasedOnTabWidgetVisibility()
{
    if (tabWidget->isVisible()) {

        std::unique_ptr<MenuMain> menuMain = std::make_unique<MenuMain>(menuBar);
        connect(menuMain->GetNewWindowAction(), &QAction::triggered, this, &MyCAD::createNewWindow);
        connect(menuMain->GetCloseWindowAction(), &QAction::triggered, this, &MyCAD::onCloseThisTab);
        connect(menuMain->GetexitAction(), &QAction::triggered, this, &MyCAD::onExitThis);
        connect(menuMain->GetlineAction(), &QAction::triggered, this, &MyCAD::onDrawLine);
        connect(menuMain->GetcircleAction(), &QAction::triggered, this, &MyCAD::onDrawCircle);
    }
    else {
        std::unique_ptr<MenuInit> menuInit = std::make_unique<MenuInit>(menuBar);
        connect(menuInit->GetNewWindowAction(), &QAction::triggered, this, &MyCAD::createNewWindow);
        connect(menuInit->GetCloseWindowAction(), &QAction::triggered, this, &MyCAD::onCloseThisTab);
        connect(menuInit->GetexitAction(), &QAction::triggered, this, &MyCAD::onExitThis);
    }
}

void MyCAD::initialTabWidget()
{
}

void MyCAD::onDrawLine()
{
    clearSelection();
    // ondrawline = true;
    currentDrawMode = DrawMode::Line;
}

void MyCAD::onDrawCircle()
{
    clearSelection();
    //  ondrawcircle = true;
    currentDrawMode = DrawMode::Circle;
}

void MyCAD::updateGridPosition(const QPoint& delta)
{
    // Проверка, что индекс корректный и вкладки существуют
    if (chekTab()) {
        int currentIndex = tabWidget->currentIndex();
        // Обновляем значения смещения сетки на основе переданного delta
        tabDataList[currentIndex].delataX += delta.x();
        tabDataList[currentIndex].delataY += delta.y();
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
        for (const auto& shape : tabDataList[currentIndex].shapes) {
            shape->move(delta);
        }

    }
}

void MyCAD::drawGrid(QPainter& painter)
{

    if (chekTab()) {

        // Получаем текущий активный виджет во вкладке
        int currentIndex = tabWidget->currentIndex();

        QWidget* currentTab = tabWidget->widget(currentIndex);

        // Проверяем, что событие происходит на текущей активной вкладке
        if (currentTab) {

            int delataX = tabDataList[currentIndex].delataX;
            int delataY = tabDataList[currentIndex].delataY;
            // Устанавливаем параметры для рисования сетки
            int gridSize = 37;  // Размер ячейки сетки
            Grid grid(currentTab, gridSize, delataX, delataY);  // Создаем экземпляр класса сетки
            grid.draw(painter);     // Рисуем сетку
        }
    }
}

void MyCAD::DrawLine(QPainter& painter, QPoint localPos0)
{
    if (isdraw) {
        if (chekTab()) {           
            // Проверяем, что событие происходит на текущей активной вкладке
            if (!isDragging) {
                QPoint newpoint = GetCurrPoint();
                QColor Color(255, 155, 155);
                QPen Pen(Color, 1, Qt::SolidLine);
                painter.setPen(Pen);
                painter.drawLine(localPos0.x(), localPos0.y(), newpoint.x(), newpoint.y());
            }
        }
    }
}

void MyCAD::DrawCircle(QPainter& painter, QPoint localPos0)
{
    if (isdraw) {
        if (chekTab()) {           
            if ( !isDragging) {
                QPoint newpoint = GetCurrPoint();
                // Вычисляем радиус как максимальное расстояние по X или Y от центра до текущей позиции
                int radius = std::hypot(newpoint.x() - localPos0.x(), newpoint.y() - localPos0.y());
                QColor Color(255, 155, 155);
                QPen Pen(Color, 1, Qt::SolidLine);
                painter.setPen(Pen);
                // Рисуем круг, используя радиус
                painter.drawEllipse(localPos0, radius, radius);  // Рисуем круг с одинаковым радиусом по X и Y
                painter.setPen(DashPen(QColor(212, 161, 32), 10, 5));
                painter.drawLine(localPos0.x(), localPos0.y(), newpoint.x(), newpoint.y());
            }
        }
    }
}

QPen MyCAD::DashPen(QColor Color, qreal dashLength, qreal gapLength)
{
   
    QPen Pen2(Color, 1);
    // Увеличьте шаг, изменяя длину и расстояние между штрихами
    //  qreal dashLength = 10; // Длина штриха
    //  qreal gapLength = 5;   // Расстояние между штрихами
        QVector<qreal> dashPattern;
    dashPattern << dashLength << gapLength; // Определите паттерн
    Pen2.setDashPattern(dashPattern); // Установите паттерн в перо
    return Pen2;
}

void MyCAD::addShape(std::unique_ptr<Shape>&& shape) {
    if (chekTab()) {
        // Получаем индекс активной вкладки
        int currentIndex = tabWidget->currentIndex();
        // Добавляем фигуру в список фигур активной вкладки
        tabDataList[currentIndex].shapes.push_back(std::move(shape));
        // Перерисовываем активную вкладку
        QWidget* currentTab = tabWidget->widget(tabWidget->currentIndex());
        if (currentTab) {
            // qDebug() << "Calling repaint on:" << currentTab;
            currentTab->setEnabled(true);
            currentTab->update();
        }
    }
}

void MyCAD::clearvectors()
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

void MyCAD::clearSelection()
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
            for (const auto& shape : tabDataList[currentIndex].shapes) {
                shape->setSelected(false);
            }        
    }
}

void MyCAD::CrossCursorIn(QPainter& painter)
{
    QPoint newpoint = GetCurrPoint();
    // рисуем курсор перемещения
    painter.drawLine(newpoint.x() - 48, newpoint.y(), newpoint.x() + 48, newpoint.y());
    painter.drawLine(newpoint.x(), newpoint.y() - 48, newpoint.x(), newpoint.y() + 48);

}

void MyCAD::CrossCursorOut(QPainter& painter)
{
    QPoint newpoint = GetCurrPoint();

    // Горизонтальные линии
    painter.drawLine(newpoint.x() - 3, newpoint.y() - 3, newpoint.x() + 3, newpoint.y() - 3);
    painter.drawLine(newpoint.x() - 3, newpoint.y() + 3, newpoint.x() + 3, newpoint.y() + 3);

    // Вертикальные линии
    painter.drawLine(newpoint.x() - 3, newpoint.y() - 3, newpoint.x() - 3, newpoint.y() + 3);
    painter.drawLine(newpoint.x() + 3, newpoint.y() - 3, newpoint.x() + 3, newpoint.y() + 3);

    int cursorSize = 97;
    int squareSide = 3; // Половина стороны квадрата 6x6

    // Вертикальные линии перекрестия
    painter.drawLine(newpoint.x(), newpoint.y() - cursorSize / 2, newpoint.x(), newpoint.y() - squareSide); // Вверх
    painter.drawLine(newpoint.x(), newpoint.y() + squareSide, newpoint.x(), newpoint.y() + cursorSize / 2); // Вниз

    // Горизонтальные линии перекрестия
    painter.drawLine(newpoint.x() - cursorSize / 2, newpoint.y(), newpoint.x() - squareSide, newpoint.y()); // Влево
    painter.drawLine(newpoint.x() + squareSide, newpoint.y(), newpoint.x() + cursorSize / 2, newpoint.y()); // Вправо
}

void MyCAD::drawShapes(QPainter& painter) {
    // Проверяем, есть ли активная вкладка
    if (!chekTab()) {
        return;
    }

    // Получаем индекс активной вкладки и настройки
    int currentIndex = tabWidget->currentIndex();
    int delataX = tabDataList[currentIndex].delataX;
    int delataY = tabDataList[currentIndex].delataY;
    QPoint delta(delataX, delataY);
    QWidget* currentTab = tabWidget->widget(currentIndex);
    int widgetHeight = currentTab->height();

    // Отрисовка временных выделенных фигур, если мы не перетаскиваем объекты
    if (!isDragging && !selShapes.empty()) {
        drawTemporaryShapes(painter);
    }

    // Основная отрисовка фигур для текущей вкладки
    for (const auto& shape : tabDataList[currentIndex].shapes) {
        if (heightwindow_prev != 0) {
            QPoint delta(0, widgetHeight - heightwindow_prev);
            shape->move(delta);
        }
        shape->draw(painter);
    }

    heightwindow_prev = widgetHeight;
}

// Метод для отрисовки временных фигур
void MyCAD::drawTemporaryShapes(QPainter& painter) {
    auto tmpShapeIt = tmpShapes.begin();
    for (auto& shape : selShapes) {
        if (tmpShapeIt != tmpShapes.end()) {
            QPen pen = painter.pen();
            painter.setPen(getDashPenForShape(*tmpShapeIt));
            drawLineToCurrentPoint(painter, *tmpShapeIt);
            painter.setPen(pen);  // Возвращаем Pen
            shape->draw(painter);
            ++tmpShapeIt;
        }
    }
}

// Метод для рисования линии к текущей точке
void MyCAD::drawLineToCurrentPoint(QPainter& painter, const std::shared_ptr<Shape>& shape) {
    QPoint newpoint = GetCurrPoint();
    if (shape->getisStart()) {
        painter.drawLine(shape->getstartPoint(), newpoint);
    }

    else if (shape->getisEnd()) {
        painter.drawLine(shape->getendPoint(), newpoint);
    }
    else if (shape->getisMiddle()) {
        painter.drawLine(shape->getmiddlePoint(), newpoint);
    }
    else if (shape->getisLeft() || shape->getisTop() || shape->getisRight() || shape->getisBottom()) {
        painter.drawLine(shape->getstartPoint(), newpoint);
    }
}

// Метод для получения DashPen в зависимости от состояния фигуры
QPen MyCAD::getDashPenForShape(const std::shared_ptr<Shape>& shape)  {
    if (shape->getisLeft() || shape->getisTop() || shape->getisRight() || shape->getisBottom()) {
        return DashPen(QColor(161, 161, 161), 2, 2);
    }
    else {
        return DashPen(QColor(212, 161, 32), 10, 5);
    }
}
