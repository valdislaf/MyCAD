
#include "MyCAD.h"

int heightwindow_prev = 0;
bool disableCursor = false;

MyCAD::MyCAD(QWidget* parent)
    : EventHandling(parent)
{
    tabWidget = new QTabWidget(this);  // Инициализация tabWidget
    menuBar = new QMenuBar(this);  // Создаем QMenuBar
    drawingManager = new DrawingManager();
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
    delete drawingManager;
}


void MyCAD::onTabChanged(int index)
{
    clearSelection();
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
    
    // Переключаемся на только что созданную вкладку
    tabWidget->setCurrentIndex(tabIndex);

    // Инициализируем данные для новой вкладки
    TabData newTabData;
    newTabData.delataX = 10; // Устанавливаем начальные значения
    newTabData.delataY = -10;
   // tabDataList.push_back(newTabData); // Добавляем данные вкладки в список
    addTab(newTabData);
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

void MyCAD::onDrawLine()
{
    clearSelection();   
    currentDrawMode = DrawMode::Line;
}

void MyCAD::onDrawCircle()
{
    clearSelection();    
    currentDrawMode = DrawMode::Circle;
}

void MyCAD::updateGridPosition(const QPoint& delta)
{
    // Проверка, что индекс корректный и вкладки существуют
    if (chekTab()) {    
        // Обновляем значения смещения сетки на основе переданного delta
        setDelataX(delta.x());
        setDelataY(delta.y());
        if (isdraw && clickpoint != QPoint(INT_MIN, INT_MIN)) {
            clickpoint = QPoint(clickpoint.x() + delta.x(), clickpoint.y() + delta.y());
        }
        // Перерисовываем текущий активный виджет
        QWidget* currentTab = tabWidget->currentWidget();
        if (currentTab) {
            currentTab->update();  // Вызов перерисовки виджета
        }        
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

void MyCAD::drawGrid(QPainter& painter) {
    if (!isTabActive()) return;  // Объединенная проверка
    drawingManager->drawGrid(painter, tabWidget->currentWidget(), { getDelataX(), getDelataY() });
}

void MyCAD::DrawLine(QPainter& painter, QPoint localPos0) {
    if (!isDrawEnabled()) return;  // Проверка условий перенесена в отдельную функцию
    drawingManager->drawLine(painter, localPos0, GetCurrPoint());
}

void MyCAD::DrawCircle(QPainter& painter, QPoint localPos0) {
    if (!isDrawEnabled()) return;  
    drawingManager->drawCircle(painter, localPos0, GetCurrPoint());
}

// Вспомогательная функция для проверки активности вкладки и флага isdraw
const bool MyCAD::isDrawEnabled() const {
    return isdraw && isTabActive() && !isDragging;
}

// Вспомогательная функция для проверки активности текущей вкладки
const bool MyCAD::isTabActive() const {
    return tabWidget && chekTab() && tabWidget->currentWidget();
}

void MyCAD::CrossCursorIn(QPainter& painter)
{
    drawingManager->drawCrossCursorIn(painter, GetCurrPoint());
}

void MyCAD::CrossCursorOut(QPainter& painter)
{
    drawingManager->drawCrossCursorOut(painter, GetCurrPoint());
}

void MyCAD::CrossCursorHandle(QPainter& painter)
{
    drawingManager->drawCrossCursorOut(painter, GetHandlePoint());
}

void MyCAD::drawShapes(QPainter& painter) {
    // Проверяем, есть ли активная вкладка
    if (!chekTab()) {
        return;
    }

    // Получаем индекс активной вкладки и настройки
    int currentIndex = tabWidget->currentIndex();
    int delataX = getDelataX();
    int delataY = getDelataY();
    QPoint delta(delataX, delataY);
    QWidget* currentTab = tabWidget->widget(currentIndex);
    int widgetHeight = currentTab->height();

    // Отрисовка временных выделенных фигур, если мы не перетаскиваем объекты
    if (!isDragging && !selShapes.empty()) {
        drawingManager->drawTemporaryShapes(painter, tmpShapes, selShapes, GetCurrPoint());
    }

    // Основная отрисовка фигур для текущей вкладки
    for (const auto& shape : getShapes()) {

        if (heightwindow_prev != 0) {
            QPoint delta(0, widgetHeight - heightwindow_prev);
            shape->move(delta);
        }
        shape->draw(painter);
    }

    heightwindow_prev = widgetHeight;
}
