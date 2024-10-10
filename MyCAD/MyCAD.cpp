#include <QBitmap>
#include <QCursor>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QTabBar>
#include <QTabWidget>
#include <QWidget>
#include <QMenuBar>

#include "MyCAD.h"
#include <MenuInit.h>
#include <MenuMain.h>

bool isdraw = false;
bool ondrawline = false;
bool ondrawcircle = false;
bool updrawcircle = false;
int heightwindow_prev = 0;
QPoint clickpoint = QPoint(0, 0);
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
    : QMainWindow(parent)
{
    //setupUi(this);
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
    // Размеры курсора
    int cursorSize = 97;
    QPixmap cursorPixmap(cursorSize, cursorSize);
    cursorPixmap.fill(Qt::transparent);

    QPainter painter(&cursorPixmap);
    painter.setPen(QPen(Qt::white, 1)); // Устанавливаем цвет и толщину линий

    // Рисуем перекрестие
    int squareside = 3; // сторона внутреннего квадрата
    painter.drawLine(cursorSize / 2, 0, cursorSize / 2, cursorSize / 2 - squareside);
    painter.drawLine(0, cursorSize / 2, cursorSize / 2 - squareside, cursorSize / 2);
    painter.drawLine(cursorSize / 2, cursorSize / 2 + squareside, cursorSize / 2, cursorSize);
    painter.drawLine(cursorSize / 2 + squareside, cursorSize / 2, cursorSize, cursorSize / 2);

    painter.drawLine(cursorSize / 2 - squareside, cursorSize / 2 - squareside, cursorSize / 2 - squareside, cursorSize / 2 + squareside);
    painter.drawLine(cursorSize / 2 - squareside, cursorSize / 2 + squareside, cursorSize / 2 + squareside, cursorSize / 2 + squareside);
    painter.drawLine(cursorSize / 2 + squareside, cursorSize / 2 + squareside, cursorSize / 2 + squareside, cursorSize / 2 - squareside);
    painter.drawLine(cursorSize / 2 + squareside, cursorSize / 2 - squareside, cursorSize / 2 - squareside, cursorSize / 2 - squareside);
    painter.end();

    // Создаем курсор
    return QCursor(cursorPixmap);
}

QCursor MyCAD::createCustomCrossCursorIn()
{

    // Размеры курсора
    int cursorSize = 97;
    QPixmap cursorPixmap(cursorSize, cursorSize);
    cursorPixmap.fill(Qt::transparent);

    QPainter painter(&cursorPixmap);
    painter.setPen(QPen(Qt::white, 1)); // Устанавливаем цвет и толщину линий

    // Рисуем перекрестие
    int squareside = 0; // сторона внутреннего квадрата
    painter.drawLine(cursorSize / 2, 0, cursorSize / 2, cursorSize / 2 - squareside);
    painter.drawLine(0, cursorSize / 2, cursorSize / 2 - squareside, cursorSize / 2);
    painter.drawLine(cursorSize / 2, cursorSize / 2 + squareside, cursorSize / 2, cursorSize);
    painter.drawLine(cursorSize / 2 + squareside, cursorSize / 2, cursorSize, cursorSize / 2);

    painter.drawLine(cursorSize / 2 - squareside, cursorSize / 2 - squareside, cursorSize / 2 - squareside, cursorSize / 2 + squareside);
    painter.drawLine(cursorSize / 2 - squareside, cursorSize / 2 + squareside, cursorSize / 2 + squareside, cursorSize / 2 + squareside);
    painter.drawLine(cursorSize / 2 + squareside, cursorSize / 2 + squareside, cursorSize / 2 + squareside, cursorSize / 2 - squareside);
    painter.drawLine(cursorSize / 2 + squareside, cursorSize / 2 - squareside, cursorSize / 2 - squareside, cursorSize / 2 - squareside);
    painter.end();

    // Создаем курсор
    return QCursor(cursorPixmap);
}

void MyCAD::CoordinateAxes(QPainter& painter, QWidget* currentTab)
{
    int currentIndex = tabWidget->currentIndex();
    int widgetHeight = currentTab->height();
    int widgetWidth = currentTab->width();
    painter.setPen(QPen(Qt::white, 1)); // Устанавливаем цвет и толщину линий
    int cursorSize = 100;
    // Рисуем перекрестие
    int squareside = 5; // сторона внутреннего квадрата
    int delataX = tabDataList[currentIndex].delataX;
    int delataY = tabDataList[currentIndex].delataY;

    if (delataX<0 || (widgetHeight + delataY) < 0 || delataX > widgetWidth || (widgetHeight + delataY)>widgetHeight)
    {
        delataX = 10;
        delataY = -10;
    }

    // Y    
    painter.drawLine(delataX + 8, widgetHeight + delataY - 61, delataX + 4, widgetHeight + delataY - 66);
    painter.drawLine(delataX + 8, widgetHeight + delataY - 61, delataX + 12, widgetHeight + delataY - 66);
    painter.drawLine(delataX + 8, widgetHeight + delataY - 61, delataX + 8, widgetHeight + delataY - 54);

    // X  
    painter.drawLine(delataX + 54, widgetHeight + delataY - 12, delataX + 63, widgetHeight + delataY - 3);
    painter.drawLine(delataX + 63, widgetHeight + delataY - 12, delataX + 54, widgetHeight + delataY - 3);


    painter.drawLine(delataX, widgetHeight + delataY, delataX, widgetHeight - cursorSize / 2 + delataY);
    painter.drawLine(delataX, widgetHeight + delataY, cursorSize / 2 + delataX, widgetHeight + delataY);

    painter.drawLine(-squareside + delataX, widgetHeight - squareside + delataY, -squareside + delataX, widgetHeight + squareside + delataY);
    painter.drawLine(-squareside + delataX, widgetHeight + squareside + delataY, squareside + delataX, widgetHeight + squareside + delataY);
    painter.drawLine(squareside + delataX, widgetHeight + squareside + delataY, squareside + delataX, widgetHeight - squareside + delataY);
    painter.drawLine(squareside + delataX, widgetHeight - squareside + delataY, -squareside + delataX, widgetHeight - squareside + delataY);

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
                            if (clickpoint != QPoint(0, 0))
                            {
                                if (ondrawline) // если рисуем линию
                                {
                                    auto line = std::make_unique<Line>(clickpoint, newpoint);
                                    addShape(std::move(line));  // Обратите внимание на вызов addShape
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
    else {
        //int currentIndex = tabWidget->currentIndex();

        //if (currentIndex >= 0 && currentIndex < tabDataList.size()) {
        //    QPoint globalPos = QCursor::pos(); // Получаем глобальные координаты мыши

        //    // Предположим, что у вас есть указатель на текущую вкладку:
        //    QWidget* currentTab = tabWidget->currentWidget();
        //    QPoint newpoint = currentTab->mapFromGlobal(globalPos);

        //    for (const auto& shape : tabDataList[currentIndex].shapes) {
        //        if (shape->contains(newpoint)) {
        //            // Действие, если точка попала в фигуру, например, выделение
        //            shape->setSelected(true);
        //        }
        //    }

        //}
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
                      // int currentIndex = tabWidget->currentIndex();
                      // if (currentIndex == -1) {
                      //     return;
                      // }
                      // QPoint globalPos = QCursor::pos(); // Получаем глобальные координаты мыши
                      // // Преобразуем глобальные координаты в локальные относительно текущей вкладки
                      // QPoint localPos = tabWidget->widget(currentIndex)->mapFromGlobal(globalPos);
                      // // Вычисляем радиус как максимальное расстояние по X или Y от центра до текущей позиции
                      // int radius = std::hypot(localPos.x() - tmpShapes[i]->getstartPoint().x(), localPos.y() - tmpShapes[i]->getstartPoint().y());
                      //tmpShapes[i]->setCentre(tmpShapes[i]->getstartPoint(), radius, tmpShapes[i]->getisSelected());

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

                    if (handle == HandleType::StartHandle && shape->getisStart()) {
                        // Логика для перемещения начальной точки линии  
                        if (shape->getisSelected()) {
                            selShapes.push_back(shape->clone());
                            selShapes.back()->setSelected(shape->getisSelected());
                            tmpShapes.push_back(shape);
                            if (shape->getisStart()) {
                                movingPush(handle, shape->getisSelected());   // && shape->getisSelected()???
                            }
                        }
                    }
                    else if (handle == HandleType::EndHandle  && shape->getisEnd()) {
                        // Логика для перемещения конечной точки линии 
                        if (shape->getisSelected()) {
                            selShapes.push_back(shape->clone());
                            selShapes.back()->setSelected(shape->getisSelected());
                            tmpShapes.push_back(shape);
                            if (shape->getisEnd()) {
                                movingPush(handle, shape->getisSelected());
                            }
                        }
                    }
                    else if (handle == HandleType::MiddleHandle  && shape->getisMiddle()) {
                        if (shape->getisSelected()) {
                            selShapes.push_back(shape->clone());
                            selShapes.back()->setSelected(shape->getisSelected());
                            tmpShapes.push_back(shape);
                            if (shape->getisMiddle()) {
                                movingPush(handle, shape->getisSelected());
                            }
                        }
                    }
                    else if (handle == HandleType::LeftHandle && shape->getisLeft()) {
                        if (shape->getisSelected()) {
                            selShapes.push_back(shape->clone());
                            selShapes.back()->setSelected(shape->getisSelected());
                            tmpShapes.push_back(shape);
                            if (shape->getisLeft()) {
                                movingPush(handle, shape->getisSelected());
                            }
                        }
                    }
                    else if (handle == HandleType::TopHandle && shape->getisTop()) {
                        if (shape->getisSelected()) {
                            selShapes.push_back(shape->clone());
                            selShapes.back()->setSelected(shape->getisSelected());
                            tmpShapes.push_back(shape);
                            if (shape->getisTop()) {
                                movingPush(handle, shape->getisSelected());
                            }
                        }
                    }
                    else if (handle == HandleType::RightHandle && shape->getisRight()) {
                        if (shape->getisSelected()) {
                            selShapes.push_back(shape->clone());
                            selShapes.back()->setSelected(shape->getisSelected());
                            tmpShapes.push_back(shape);
                            if (shape->getisRight()) {
                                movingPush(handle, shape->getisSelected());
                            }
                        }
                    }
                    else if (handle == HandleType::BottomHandle && shape->getisBottom()) {
                        if (shape->getisSelected()) {
                            selShapes.push_back(shape->clone());
                            selShapes.back()->setSelected(shape->getisSelected());
                            tmpShapes.push_back(shape);
                            if (shape->getisBottom()) {
                                movingPush(handle, shape->getisSelected());
                            }
                        }
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

                for (const auto& shape : tabDataList[currentIndex].shapes) {
                    if (shape->contains(newpoint)) {
                        // Действие, если точка попала в фигуру, например, выделение
                        shape->setSelected(true);
                    }
                }

            }
        }
    }
   // if (isdraw) { ondrawcircle = false; }
    // Вызываем базовый обработчик события
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
        if (isdraw) {
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

bool MyCAD::event(QEvent* e) {
   
    if (e->type() == QEvent::HoverMove) {

        int currentIndex = tabWidget->currentIndex();

        if (currentIndex >= 0 && currentIndex < tabDataList.size()) {
            QPoint globalPos = QCursor::pos(); // Получаем глобальные координаты мыши

            // Предположим, что у вас есть указатель на текущую вкладку:
            QWidget* currentTab = tabWidget->currentWidget();
            QPoint newpoint = currentTab->mapFromGlobal(globalPos);

            for (const auto& shape : tabDataList[currentIndex].shapes) {
                if (shape->contains(newpoint)) {
                    // Действие, если точка попала в фигуру, например, выделение
                   // shape->setSelected(true);

                    // если навели то true 
                    shape->setisover(true); //должно быть свойство shape
                }else{ shape->setisover(false); }
            }

        }


        update();
        if (!ondrawline && !isDragging) {
            if (!selShapes.empty() && !movingStarts.empty()) {
                setCursor(createCustomCrossCursorIn());
                QPoint globalPos = QCursor::pos();
                QWidget* currentTab = tabWidget->currentWidget();
                QPoint newpoint = currentTab->mapFromGlobal(globalPos);
                QPoint delta = newpoint - lastMousePosition;
                if (selShapes.size() != movingStarts.size())
                {
                    int stop = 0;
                }

                for (int i = 0; i < selShapes.size(); i++) {

                    if (movingStarts.empty()|| movingWholeLines.empty() || movingEnds.empty() || movingLefts.empty() || movingTops.empty() || movingRights.empty() || movingBottoms.empty()  )
                    {
                        int stop = 0;
                    }
                    bool temp = selShapes[i]->getisSelected();
                    if (selShapes[i]->getisSelected()) {
                        selShapes[i]->setMoveSelected(true);
                        if (movingWholeLines[i]) {

                            selShapes[i]->move(delta);
                        }
                        if (movingStarts[i]) {
                            selShapes[i]->moveStart(delta);
                        }
                        // перемещение конца линии
                        if (movingEnds[i]) {
                            selShapes[i]->moveEnd(delta);
                        }
                        if (movingLefts[i] || movingTops[i] || movingRights[i] || movingBottoms[i]) {
                            int radius = std::hypot(newpoint.x() - selShapes[i]->getstartPoint().x(), newpoint.y() - selShapes[i]->getstartPoint().y());
                            selShapes[i]->moveRadius(radius);
                        }
                    }
                   /* qDebug() << "movingLefts[i]"<< movingLefts[i];
                    qDebug() << "movingTops[i]" << movingTops[i];
                    qDebug() << "movingRights[i]" << movingRights[i];
                    qDebug() << "movingBottoms[i]" << movingBottoms[i];*/
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
    int delataX = tabDataList[currentIndex].delataX;
    int delataY = tabDataList[currentIndex].delataY;
    // Проверяем, что событие происходит на текущей активной вкладке
    if (currentTab) {

        // Устанавливаем параметры для рисования сетки
        int gridSize = 37;  // Размер ячейки сетки
        int widgetWidth = currentTab->width();
        int widgetHeight = currentTab->height();


        int xmax = 0; int ymax = 0;
        // Создаем QPen для основных линий сетки
        QColor mainGridColor(38, 44, 55);  // Цвет основной сетки
        QPen mainGridPen(mainGridColor, 2, Qt::DotLine);

        // Создаем QPen для линий, которые отображаются каждые 5 шагов
        QColor highlightedGridColor(48, 54, 69);  // Цвет выделенной сетки
        QPen highlightedGridPen(highlightedGridColor, 2, Qt::SolidLine);

        // Рисуем вертикальные линии сетки вправо от начала координат
        for (int x = 0; x < widgetWidth + std::abs(delataX); x += gridSize) {
            if ((x / gridSize) % 5 == 0) {
                painter.setPen(highlightedGridPen);
            }
            else {
                painter.setPen(mainGridPen);
            }
            painter.drawLine(x + delataX, 0, x + delataX, widgetHeight + std::abs(delataY));
            xmax = x;
        }

        // Рисуем вертикальные линии сетки влево от начала координат
        for (int x = 0; x > -std::abs(delataX); x -= gridSize) {
            if ((x / gridSize) % 5 == 0) {
                painter.setPen(highlightedGridPen);
            }
            else {
                painter.setPen(mainGridPen);
            }
            painter.drawLine(x + delataX, 0, x + delataX, widgetHeight + std::abs(delataY));
            xmax = x;
        }

        // Рисуем горизонтальные линии сетки вверх от начала координат
        int y5 = 0;
        for (int y = widgetHeight; y > -std::abs(delataY); y -= gridSize) {
            if ((y5 / gridSize) % 5 == 0) {
                painter.setPen(highlightedGridPen);
            }
            else {
                painter.setPen(mainGridPen);
            }

            painter.drawLine(0, y + delataY, widgetWidth + std::abs(delataX), y + delataY);
            y5 += gridSize;
        }
        // ymax = widgetHeight;
         // Рисуем горизонтальные линии сетки вниз от начала координат
        y5 = 0;
        for (int y = widgetHeight; y < std::abs(delataY) + widgetHeight; y += gridSize) {
            if ((y5 / gridSize) % 5 == 0) {
                painter.setPen(highlightedGridPen);
            }
            else {
                painter.setPen(mainGridPen);
            }

            painter.drawLine(0, y + delataY, widgetWidth + std::abs(delataX), y + delataY);
            y5 += gridSize;
        }

        // Рисуем оси координат
        QColor XColor(130, 0, 0);
        painter.setPen(QPen(XColor, 1));
        painter.drawLine(0 + delataX, widgetHeight + delataY, widgetWidth + std::abs(delataX), widgetHeight + delataY);  // Ось X
        QColor YColor(0, 130, 0);
        painter.setPen(QPen(YColor, 1));
        painter.drawLine(0 + delataX, 0, 0 + delataX, widgetHeight + delataY);  // Ось Y
        CoordinateAxes(painter, currentTab);
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

            QColor Color2(212, 161, 32);
            QPen Pen2(Color2, 1);

            // Увеличьте шаг, изменяя длину и расстояние между штрихами
            qreal dashLength = 10; // Длина штриха
            qreal gapLength = 5;   // Расстояние между штрихами

            QVector<qreal> dashPattern;
            dashPattern << dashLength << gapLength; // Определите паттерн

            Pen2.setDashPattern(dashPattern); // Установите паттерн в перо

            painter.setPen(Pen2);
            painter.drawLine(localPos0.x(), localPos0.y(), localPos.x(), localPos.y());
        }
    }
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

void MyCAD::clearSelection()
{

    setCursor(createCustomCrossCursor());
    clickpoint = QPoint(0, 0);
    isdraw = false;
    ondrawline = false;
    ondrawcircle = false;
    
    movingEnds.clear();
    movingWholeLines.clear();
    selShapes.clear();
    tmpShapes.clear();
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
                for (auto& shape : selShapes)
                {
                    shape->draw(painter);
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
    if (isdraw) {
        if (event->key() == Qt::Key_Escape) {
            clearSelection();
        }
        else {
            // Передаем событие базовому классу
            QMainWindow::keyPressEvent(event);

        }
    }
    else if (event->key() == Qt::Key_Escape) {
        clearSelection();
    }
}
