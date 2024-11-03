#include "MyCAD.h"

int heightwindow_prev = 0;
bool disableCursor = false;

MyCAD::MyCAD(QWidget* parent)
    : EventHandling(parent)
{

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
     //   int currentIndex = tabWidget->currentIndex();
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

void MyCAD::drawGrid(QPainter& painter) {
    if (!isTabActive()) return;  // Объединенная проверка

    int gridSize = 37;
    Grid grid(tabWidget->currentWidget(), gridSize, getDelataX(), getDelataY());
    grid.draw(painter);
}

void MyCAD::DrawLine(QPainter& painter, QPoint localPos0) {
    if (!isDrawEnabled()) return;  // Проверка условий перенесена в отдельную функцию

    painter.setPen(createSolidPen(QColor(255, 155, 155), 1));
    painter.drawLine(localPos0, GetCurrPoint());
}

void MyCAD::DrawCircle(QPainter& painter, QPoint localPos0) {
    if (!isDrawEnabled()) return;

    QPoint newpoint = GetCurrPoint();
    int radius = std::hypot(newpoint.x() - localPos0.x(), newpoint.y() - localPos0.y());

    painter.setPen(createSolidPen(QColor(255, 155, 155), 1));
    painter.drawEllipse(localPos0, radius, radius);

    painter.setPen(DashPen(QColor(212, 161, 32), 10, 5));
    painter.drawLine(localPos0, newpoint);
}

// Вспомогательная функция для создания однотонной ручки
QPen MyCAD::createSolidPen(const QColor& color, int width) const {
    return QPen(color, width, Qt::SolidLine);
}

// Вспомогательная функция для проверки активности вкладки и флага isdraw
const bool MyCAD::isDrawEnabled() const {
    return isdraw && isTabActive() && !isDragging;
}

// Вспомогательная функция для проверки активности текущей вкладки
const bool MyCAD::isTabActive() const {
    return tabWidget && chekTab() && tabWidget->currentWidget();
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

void MyCAD::CrossCursorHandle(QPainter& painter, QPoint newpoint)
{
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
    int delataX = getDelataX();
    int delataY = getDelataY();
    QPoint delta(delataX, delataY);
    QWidget* currentTab = tabWidget->widget(currentIndex);
    int widgetHeight = currentTab->height();

    // Отрисовка временных выделенных фигур, если мы не перетаскиваем объекты
    if (!isDragging && !selShapes.empty()) {
        drawTemporaryShapes(painter);
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
