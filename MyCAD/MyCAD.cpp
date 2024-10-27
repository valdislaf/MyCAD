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
bool ondrawline = false;
bool ondrawcircle = false;
bool updrawcircle = false;
int heightwindow_prev = 0;
QPoint clickpoint = QPoint(INT_MIN, INT_MIN);
std::vector<std::shared_ptr<Shape>>selShapes;
std::vector<std::shared_ptr<Shape>>tmpShapes;
std::vector<bool> movingWholeLines;
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
        movingWholeLines.push_back(handle == HandleType::MiddleHandle);
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

void MyCAD::mousePressEvent(QMouseEvent* event)
{

    bool circleflag = false;

    if (ondrawcircle) { isdraw = true; }
    else  if (ondrawline) { isdraw = true; }
    if (isdraw) {
        if (event->button() == Qt::LeftButton)
        {

            if (ondrawcircle)
            {
                updrawcircle = true;
            }

            // Проверяем, находится ли клик внутри tabWidget
            if (tabWidget->rect().contains(event->pos()))
            {
                // Определяем индекс вкладки, по которой был клик
                int currentIndex = tabWidget->currentIndex();
                if (currentIndex != -1)
                {

                    // Преобразуем глобальные координаты события в локальные координаты tabWidget
                    QPoint localPos = tabWidget->mapFromGlobal(event->globalPosition().toPoint());
                    QRect tabBarRect = tabWidget->geometry();

                    if (tabBarRect.contains(localPos))
                    {
                        QPoint globalPos = QCursor::pos(); // Получаем глобальные координаты мыши

                        // Предположим, что у вас есть указатель на текущую вкладку:
                        QWidget* currentTab = tabWidget->currentWidget();


                        QPoint newpoint = currentTab->mapFromGlobal(globalPos);
                        if (clickpoint != QPoint(INT_MIN, INT_MIN))
                        {
                            if (ondrawline) // если рисуем линию
                            {
                                auto line = std::make_unique<Line>(clickpoint, newpoint);
                                addShape(std::move(line));
                            }
                            if (updrawcircle) // если рисуем круг
                            {
                                int radius = std::hypot(newpoint.x() - clickpoint.x(), newpoint.y() - clickpoint.y());
                                auto circle = std::make_unique<Circle>(clickpoint, radius);
                                addShape(std::move(circle));
                                ondrawcircle = false;
                                updrawcircle = false;
                                clearSelection();
                                circleflag = true;
                            }
                        }
                        // Преобразуем глобальные координаты в локальные относительно текущей вкладки
                        clickpoint = newpoint;
                        // QMessageBox::information(this, "Tab Click", "Клик по вкладке с индексом: " + QString::number(currentIndex));
                    }
                }
            }
        }
    }

    if (event->button() == Qt::MiddleButton) // Проверяем, что нажата средняя кнопка мыши
    {
        // tabWidget->setCursor(QCursor(Qt::PointingHandCursor));
        isDragging = true;
        lastMousePosition = event->pos(); // Сохраняем позицию мыши
    }

    // Проверяем, находится ли клик внутри tabWidget

    if (event->button() == Qt::LeftButton) {
        if (!selShapes.empty() && !isdraw && !movingStarts.empty()) {
            for (int i = 0; i < selShapes.size(); i++) {

                if (movingStarts.empty())
                {
                    int stop = 0;
                }

                if (movingWholeLines[i]) {
                    if (tmpShapes[i]->name() == Type::line) {
                        tmpShapes[i]->setCoords(selShapes[i]->getstartPoint(), selShapes[i]->getendPoint(), tmpShapes[i]->getisSelected());
                    }
                }
                if (movingStarts[i]) {

                    if (tmpShapes[i]->name() == Type::line) {
                        tmpShapes[i]->setCoords(selShapes[i]->getstartPoint(), tmpShapes[i]->getendPoint(), tmpShapes[i]->getisSelected());
                    }
                    if (tmpShapes[i]->name() == Type::circle) {
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
                //  selShapes[i].reset();
            }
            int currentIndex = tabWidget->currentIndex();
            // Проверяем, что currentIndex находится в допустимых пределах
            if (currentIndex >= 0 && currentIndex < tabDataList.size()) {
                // Проверяем, что список фигур не пуст
                if (!tabDataList[currentIndex].shapes.empty()) {
                    // Снимаем выделение со всех фигур
                    for (const auto& shape : tabDataList[currentIndex].shapes) {
                        shape->resetColor();
                    }
                }
            }
            clearvectors();
            update();
        }

        else  if (tabWidget->rect().contains(event->pos()))
        {
            // Определяем индекс вкладки, по которой был клик
            int currentIndex = tabWidget->currentIndex();
            if (currentIndex != -1)
            {
                QPoint globalPos = QCursor::pos(); // Получаем глобальные координаты мыши

                // Предположим, что у вас есть указатель на текущую вкладку:
                QWidget* currentTab = tabWidget->currentWidget();

                QPoint newpoint = currentTab->mapFromGlobal(globalPos);

                for (const auto& shape : tabDataList[tabWidget->currentIndex()].shapes) {
                    // Проверяем, попали ли мы на одну из ручек линии
                    HandleType handle = shape->getHandleAt(newpoint);

                  //HandleType handle2 = shape->getHandle().Type(newpoint, shape->getisSelected());
                  //  Q_ASSERT(handle == handle2);

                   
                    if (shape->getisSelected() && shape->isHandleSelected(handle, newpoint)) {
                        selShapes.push_back(shape->clone());
                        selShapes.back()->setSelected(shape->getisSelected());
                        tmpShapes.push_back(shape);
                        movingPush(handle, shape->getisSelected());
                    }

                }

            }

        }
    }

    if (event->button() == Qt::LeftButton)
    {
        if (tabWidget != nullptr) {
            int currentIndex = tabWidget->currentIndex();
            if (currentIndex != -1)
            {
                lastMousePosition = tabWidget->currentWidget()->mapFromGlobal(QCursor::pos());
            }
        }

        if (!isdraw && !circleflag) {
            int currentIndex = tabWidget->currentIndex();

            if (currentIndex >= 0 && currentIndex < tabDataList.size()) {
                QPoint globalPos = QCursor::pos(); // Получаем глобальные координаты мыши

                // Предположим, что у вас есть указатель на текущую вкладку:
                QWidget* currentTab = tabWidget->currentWidget();
                QPoint newpoint = currentTab->mapFromGlobal(globalPos);
                bool isanyshapeselectedandhandled = false;
                for (const auto& shape : tabDataList[tabWidget->currentIndex()].shapes)
                {
                    if (shape->getisSelected() && shape->getHandleAt(newpoint)!= HandleType::None)
                    {
                        isanyshapeselectedandhandled = true;
                    }
                }

                // Проходим в обратном порядке по вектору и ищем пересечение
                for (auto it = tabDataList[currentIndex].shapes.rbegin(); it != tabDataList[currentIndex].shapes.rend(); ++it) {
                    if ((*it)->contains(newpoint)) {
                        // Если точка попала в фигуру, выделяем её
                        HandleType handle = (*it)->getHandleAt(newpoint);
                        if (handle == HandleType::None || selShapes.size() == 0) { // Только если не попали в уже выделенную область ручки или нет выделенных
                            if (!isanyshapeselectedandhandled) { (*it)->setSelected(true); }
                        }
                        // Выделяем только последнюю добавленную фигуру, которая попала под точку
                        break;
                    }
                }
            }
        }
    }

    QMainWindow::mousePressEvent(event);
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

void MyCAD::updateGridPosition(const QPoint& delta)
{
    int currentIndex = tabWidget->currentIndex();

    // Проверка, что индекс корректный и вкладки существуют
    if (currentIndex != -1 && currentIndex < tabDataList.size()) {
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

void MyCAD::mouseReleaseEvent(QMouseEvent* event)
{

    if (event->button() == Qt::MiddleButton) // Проверяем, что отпущена средняя кнопка мыши
    {
        if (tabWidget != nullptr) {
            int currentIndex = tabWidget->currentIndex();
            if (currentIndex != -1)
            {
                lastMousePosition = tabWidget->currentWidget()->mapFromGlobal(QCursor::pos());
            }
        }
        isDragging = false; // Устанавливаем флаг перетаскивания в false
    }
    if (tabWidget != nullptr) {
        // Перерисовываем активную вкладку
        QWidget* currentTab = tabWidget->currentWidget();
        if (currentTab) {
            currentTab->update();  // Вызов перерисовки активного виджета

        }
    }
    if (event->button() == Qt::LeftButton) // Проверяем, что отпущена средняя кнопка мыши
    {

    }
    QMainWindow::mouseReleaseEvent(event); // Вызов базового метода
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

void MyCAD::captureCursorforHandle(QWidget* currentTab, const QRect& handleRect, const QPoint& cursorPos) {
    // Проверяем, находится ли курсор в области handleRect
    if (handleRect.contains(cursorPos)) {
        // Получаем центр квадрата Handle
        QPoint centerHandle = handleRect.center();

        // Преобразуем локальные координаты в глобальные
        QPoint globalPos = currentTab->mapToGlobal(centerHandle);

        // Если курсор не в центре, возвращаем его обратно
        if (cursorPos != centerHandle) {
            QCursor::setPos(globalPos);
        }
    }
}


bool MyCAD::event(QEvent* e) {

    if (e->type() == QEvent::HoverMove) {

        int currentIndex = tabWidget->currentIndex();

        if (currentIndex >= 0 && currentIndex < tabDataList.size()) {

            if (!tabDataList[currentIndex].shapes.empty()) {
                for (int i = 0; i < tabDataList[currentIndex].shapes.size(); i++) {
                    QWidget* currentTab = tabWidget->currentWidget();
                    QPoint newpoint = currentTab->mapFromGlobal(QCursor::pos());
                    // Если shapes[i] выделен и курсор над ним
                    if (tabDataList[currentIndex].shapes[i]->getisSelected())
                    {
                        captureCursorforHandle(currentTab, tabDataList[currentIndex].shapes[i]->getStartHandle(), newpoint);
                        captureCursorforHandle(currentTab, tabDataList[currentIndex].shapes[i]->getEndHandle(), newpoint);
                        captureCursorforHandle(currentTab, tabDataList[currentIndex].shapes[i]->getMiddleHandle(), newpoint);
                        captureCursorforHandle(currentTab, tabDataList[currentIndex].shapes[i]->getLeftHandle(), newpoint);
                        captureCursorforHandle(currentTab, tabDataList[currentIndex].shapes[i]->getRighttHandle(), newpoint);
                        captureCursorforHandle(currentTab, tabDataList[currentIndex].shapes[i]->getToptHandle(), newpoint);
                        captureCursorforHandle(currentTab, tabDataList[currentIndex].shapes[i]->getBottomtHandle(), newpoint);
                    }
                }
            }
        }

        if (!isdraw) {
            if (currentIndex >= 0 && currentIndex < tabDataList.size()) {
                QPoint globalPos = QCursor::pos(); // Получаем глобальные координаты мыши

                // Предположим, что у вас есть указатель на текущую вкладку:
                QWidget* currentTab = tabWidget->currentWidget();
                QPoint newpoint = currentTab->mapFromGlobal(globalPos);
                bool isanyshapeselectedandhandled = false;
                // Сначала сбрасываем подсветку для всех фигур
                for (const auto& shape : tabDataList[currentIndex].shapes) {

                    if (selShapes.size() != 0 && (!movingStarts.empty()))
                    {
                        isanyshapeselectedandhandled = true;
                        break;
                    }
                    shape->setisover(false);  // Сброс свойства подсветки
                }

                // Проходим в обратном порядке по вектору и ищем пересечение
                for (auto it = tabDataList[currentIndex].shapes.rbegin(); it != tabDataList[currentIndex].shapes.rend(); ++it) {
                    if (!(*it)->contains(newpoint)) {
                        (*it)->setisover(false);  // Если не пересекается, не подсвечиваем
                    }
                    else {
                        if (!isanyshapeselectedandhandled && !(*it)->getisSelected()) { (*it)->setisover(true); } // Если пересекается, подсвечиваем последнюю
                        break;  // Останавливаемся на первой найденной (последней добавленной) фигуре
                    }
                }
            }
        }
        update();


        if (!ondrawline && !isDragging) {
            if (!selShapes.empty() && !movingStarts.empty()) {
                //setCursor(createCustomCrossCursorIn());
                QPoint globalPos = QCursor::pos();
                QWidget* currentTab = tabWidget->currentWidget();
                QPoint newpoint = currentTab->mapFromGlobal(globalPos);
                QPoint delta = newpoint - lastMousePosition;
                if (selShapes.size() != movingStarts.size())
                {
                    int stop = 0;
                }

                for (int i = 0; i < selShapes.size(); i++) {

                    if (movingStarts.empty() || movingWholeLines.empty() || movingEnds.empty() || movingLefts.empty() || movingTops.empty() || movingRights.empty() || movingBottoms.empty())
                    {
                        int stop = 0;
                    }
                    bool temp = selShapes[i]->getisSelected();
                    if (selShapes[i]->getisSelected()) {
                        selShapes[i]->setMoveSelected(true);
                        if (movingWholeLines[i]) {

                            selShapes[i]->move(delta);
                           // selShapes[i]->getHandle().adddeltatoStart(delta);
                           // selShapes[i]->getHandle().adddeltatoEnd(delta);
                        }
                        if (movingStarts[i]) {
                            selShapes[i]->moveStart(delta);
                          //  selShapes[i]->getHandle().adddeltatoStart(delta);
                        }
                        // перемещение конца линии
                        if (movingEnds[i]) {
                            selShapes[i]->moveEnd(delta);
                           // selShapes[i]->getHandle().adddeltatoEnd(delta);
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

    }

    return QWidget::event(e);  // Не забывайте передавать событие дальше
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
    ondrawline = true;
}

void MyCAD::onDrawCircle()
{
    clearSelection();
    ondrawcircle = true;
}

void MyCAD::drawGrid(QPainter& painter)
{

    if (!tabWidget) {
        return;
    }

    // Получаем текущий активный виджет во вкладке
    int currentIndex = tabWidget->currentIndex();
    if (currentIndex == -1) {
        return;
    }

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

void MyCAD::DrawLine(QPainter& painter, QPoint localPos0)
{
    if (isdraw) {
        if (!tabWidget) {
            return;
        }

        // Получаем текущий активный виджет во вкладке
        int currentIndex = tabWidget->currentIndex();
        if (currentIndex == -1) {
            return;
        }
        //   QPen penInit = painter.pen();
        QWidget* currentTab = tabWidget->widget(currentIndex);
        // Проверяем, что событие происходит на текущей активной вкладке
        if (currentTab && !isDragging) {
            QPoint globalPos = QCursor::pos(); // Получаем глобальные координаты мыши

            // Предположим, что у вас есть указатель на текущую вкладку:
            QWidget* currentTab = tabWidget->currentWidget();

            // Преобразуем глобальные координаты в локальные относительно текущей вкладки
            QPoint localPos = currentTab->mapFromGlobal(globalPos);

            QColor Color(255, 155, 155);
            QPen Pen(Color, 1, Qt::SolidLine);
            painter.setPen(Pen);
            painter.drawLine(localPos0.x(), localPos0.y(), localPos.x(), localPos.y());
        }
    }
}
void MyCAD::DrawCircle(QPainter& painter, QPoint localPos0)
{
    if (isdraw) {
        if (!tabWidget) {
            return;
        }

        // Получаем текущий активный виджет во вкладке
        int currentIndex = tabWidget->currentIndex();
        if (currentIndex == -1) {
            return;
        }

        QWidget* currentTab = tabWidget->widget(currentIndex);
        // Проверяем, что событие происходит на текущей активной вкладке
        if (currentTab && !isDragging) {
            QPoint globalPos = QCursor::pos(); // Получаем глобальные координаты мыши

            // Преобразуем глобальные координаты в локальные относительно текущей вкладки
            QPoint localPos = currentTab->mapFromGlobal(globalPos);

            // Вычисляем радиус как максимальное расстояние по X или Y от центра до текущей позиции
            int radius = std::hypot(localPos.x() - localPos0.x(), localPos.y() - localPos0.y());

            QColor Color(255, 155, 155);
            QPen Pen(Color, 1, Qt::SolidLine);
            painter.setPen(Pen);

            // Рисуем круг, используя радиус
            painter.drawEllipse(localPos0, radius, radius);  // Рисуем круг с одинаковым радиусом по X и Y

            painter.setPen(DashPen(QColor(212, 161, 32), 10, 5));
            painter.drawLine(localPos0.x(), localPos0.y(), localPos.x(), localPos.y());

        }
    }
}
QPen MyCAD::DashPen(QColor Color, qreal dashLength, qreal gapLength)
{
    //QColor Color2(212, 161, 32);
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
    // Получаем индекс активной вкладки
    int currentIndex = tabWidget->currentIndex();

    if (currentIndex >= 0 && currentIndex < tabDataList.size()) {
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
    movingWholeLines.clear();
    movingStarts.clear();
    movingEnds.clear();
    movingLefts.clear();
    movingTops.clear();
    movingRights.clear();
    movingBottoms.clear();

}

void MyCAD::clearSelection()
{

    // setCursor(createCustomCrossCursor());
    clickpoint = QPoint(INT_MIN, INT_MIN);
    isdraw = false;
    ondrawline = false;
    ondrawcircle = false;

    clearvectors();

    update();


    int currentIndex = tabWidget->currentIndex();
    // Проверяем, что currentIndex находится в допустимых пределах
    if (currentIndex >= 0 && currentIndex < tabDataList.size()) {
        // Проверяем, что список фигур не пуст
        if (!tabDataList[currentIndex].shapes.empty()) {
            // Снимаем выделение со всех фигур
            for (const auto& shape : tabDataList[currentIndex].shapes) {
                shape->setSelected(false);
            }
        }
    }
}

void MyCAD::CrossCursorIn(QPainter& painter)
{

    QWidget* currentTab = tabWidget->widget(tabWidget->currentIndex());
    QPoint globalPos = QCursor::pos(); // Получаем глобальные координаты мыши
    // Преобразуем глобальные координаты в локальные относительно текущей вкладки
    QPoint localPos = currentTab->mapFromGlobal(globalPos);
    // рисуем курсор перемещения
    painter.drawLine(localPos.x() - 48, localPos.y(), localPos.x() + 48, localPos.y());
    painter.drawLine(localPos.x(), localPos.y() - 48, localPos.x(), localPos.y() + 48);

}

void MyCAD::CrossCursorOut(QPainter& painter)
{

    QWidget* currentTab = tabWidget->widget(tabWidget->currentIndex());
    QPoint globalPos = QCursor::pos(); // Получаем глобальные координаты мыши
    // Преобразуем глобальные координаты в локальные относительно текущей вкладки
    QPoint localPos = currentTab->mapFromGlobal(globalPos);

    // Горизонтальные линии
    painter.drawLine(localPos.x() - 3, localPos.y() - 3, localPos.x() + 3, localPos.y() - 3);
    painter.drawLine(localPos.x() - 3, localPos.y() + 3, localPos.x() + 3, localPos.y() + 3);

    // Вертикальные линии
    painter.drawLine(localPos.x() - 3, localPos.y() - 3, localPos.x() - 3, localPos.y() + 3);
    painter.drawLine(localPos.x() + 3, localPos.y() - 3, localPos.x() + 3, localPos.y() + 3);

    int cursorSize = 97;
    int squareSide = 3; // Половина стороны квадрата 6x6

    // Вертикальные линии перекрестия
    painter.drawLine(localPos.x(), localPos.y() - cursorSize / 2, localPos.x(), localPos.y() - squareSide); // Вверх
    painter.drawLine(localPos.x(), localPos.y() + squareSide, localPos.x(), localPos.y() + cursorSize / 2); // Вниз

    // Горизонтальные линии перекрестия
    painter.drawLine(localPos.x() - cursorSize / 2, localPos.y(), localPos.x() - squareSide, localPos.y()); // Влево
    painter.drawLine(localPos.x() + squareSide, localPos.y(), localPos.x() + cursorSize / 2, localPos.y()); // Вправо
}

void MyCAD::drawShapes(QPainter& painter) {

    // Получаем индекс активной вкладки
    int currentIndex = tabWidget->currentIndex();
    int delataX = tabDataList[currentIndex].delataX;
    int delataY = tabDataList[currentIndex].delataY;
    QPoint delta(delataX, delataY);
    QWidget* currentTab = tabWidget->widget(tabWidget->currentIndex());
    int widgetHeight = currentTab->height();

    if (currentIndex >= 0 && currentIndex < tabDataList.size()) {
        if (!isDragging) {
            if (!selShapes.empty()) {
                auto tmpShapeIt = tmpShapes.begin();
                for (auto& shape : selShapes)
                {
                    if (tmpShapeIt != tmpShapes.end()) {
                        QPen pen = painter.pen();
                        QPoint globalPos = QCursor::pos(); // Получаем глобальные координаты мыши
                        // Преобразуем глобальные координаты в локальные относительно текущей вкладки
                        QPoint localPos = currentTab->mapFromGlobal(globalPos);
                        painter.setPen(DashPen(QColor(212, 161, 32), 10, 5));
                        if ((*tmpShapeIt)->getisStart()) {
                            painter.drawLine((*tmpShapeIt)->getstartPoint().x(), (*tmpShapeIt)->getstartPoint().y(), localPos.x(), localPos.y());
                        }
                        else if ((*tmpShapeIt)->getisEnd()) {
                            painter.drawLine((*tmpShapeIt)->getendPoint().x(), (*tmpShapeIt)->getendPoint().y(), localPos.x(), localPos.y());
                        }
                        else if ((*tmpShapeIt)->getisMiddle()) {
                            painter.drawLine((*tmpShapeIt)->getmiddlePoint().x(), (*tmpShapeIt)->getmiddlePoint().y(), localPos.x(), localPos.y());
                        }

                        else if ((*tmpShapeIt)->getisLeft() || (*tmpShapeIt)->getisTop() || (*tmpShapeIt)->getisRight() || (*tmpShapeIt)->getisBottom()) {
                            painter.setPen(DashPen(QColor(161, 161, 161), 2, 2));
                            painter.drawLine((*tmpShapeIt)->getstartPoint().x(), (*tmpShapeIt)->getstartPoint().y(), localPos.x(), localPos.y());
                        }
                        // возвращаем Pen
                        painter.setPen(pen);
                        shape->draw(painter);
                        tmpShapeIt++;
                    }
                }
            }
        }
        // Рисуем фигуры только для активной вкладки
        for (const auto& shape : tabDataList[currentIndex].shapes) {
            if (heightwindow_prev != 0) {
                QPoint delta(0, widgetHeight - heightwindow_prev);
                shape->move(delta);
            }
            shape->draw(painter);  // Вызов метода отрисовки фигуры
        }
    }
    heightwindow_prev = widgetHeight;
}

void MyCAD::keyPressEvent(QKeyEvent* event) {
    // Проверяем, что нажата клавиша ESC  
    if (event->key() == Qt::Key_Escape) {
        clearSelection();
    }

    QMainWindow::keyPressEvent(event);
}
