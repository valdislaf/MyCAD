#include <QBitmap>
#include <QCursor>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QTabBar>
#include <QTabWidget>
#include <QWidget>

#include "MyCAD.h"

bool isdraw = false;
bool ondrawline = false;
QPoint clickpoint = QPoint(0, 0);
std::shared_ptr<Shape> selectedShape = nullptr;
bool movingWholeLine = false;
bool ismovingWholeLine = false;

MyCAD::MyCAD(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    ui.tabWidget->hide();

    // Настройка стиля для ui.tabWidget
    setupTabWidgetStyle();

    // Проверяем, что ui.menuBar действительно инициализирован и доступен
    if (ui.menuBar) {
        updateMenusBasedOnTabWidgetVisibility();
    }

    // Подписываемся на сигнал смены вкладок
    connect(ui.tabWidget, &QTabWidget::currentChanged, this, &MyCAD::onTabChanged);
    setFocus();
}

MyCAD::~MyCAD()
{}

void MyCAD::onTabChanged(int index)
{
    clickpoint = QPoint(0, 0);
    isdraw = false; ondrawline = false;

    int currentIndex = ui.tabWidget->currentIndex();

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

    // Обработка изменения вкладки, например, загрузка настроек сетки для выбранной вкладки
    if (index >= 0) {
        QWidget* currentTab = ui.tabWidget->widget(index);
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
    int currentIndex = ui.tabWidget->currentIndex();
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
    // Отладочные сообщения
   /* qDebug() << "delataX: " << delataX;
    qDebug() << "widgetHeight + delataY: " << (widgetHeight + delataY);
    qDebug() << "widgetHeight: " << widgetHeight;
    qDebug() << "widgetWidth:  " << widgetWidth;*/

}

void MyCAD::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {


    }
    if (ondrawline) { isdraw = true; }
    if (isdraw) {
        if (event->button() == Qt::LeftButton)
        {
            // Проверяем, находится ли клик внутри tabWidget
            if (ui.tabWidget->rect().contains(event->pos()))
            {
                // Определяем индекс вкладки, по которой был клик
                int currentIndex = ui.tabWidget->currentIndex();
                if (currentIndex != -1)
                {
                    // Преобразуем глобальные координаты события в локальные координаты tabWidget
                    QPoint localPos = ui.tabWidget->mapFromGlobal(event->globalPosition().toPoint());
                    QRect tabBarRect = ui.tabWidget->geometry();

                    if (tabBarRect.contains(localPos))
                    {
                        QPoint globalPos = QCursor::pos(); // Получаем глобальные координаты мыши

                        // Предположим, что у вас есть указатель на текущую вкладку:
                        QWidget* currentTab = ui.tabWidget->currentWidget();

                        QPoint newpoint = currentTab->mapFromGlobal(globalPos);
                        if (clickpoint != QPoint(0, 0))
                        {
                            auto line = std::make_unique<Line>(clickpoint, newpoint);
                            addShape(std::move(line));  // Обратите внимание на вызов addShape
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
        int currentIndex = ui.tabWidget->currentIndex();

        if (currentIndex >= 0 && currentIndex < tabDataList.size()) {
            QPoint globalPos = QCursor::pos(); // Получаем глобальные координаты мыши

            // Предположим, что у вас есть указатель на текущую вкладку:
            QWidget* currentTab = ui.tabWidget->currentWidget();
            QPoint newpoint = currentTab->mapFromGlobal(globalPos);

            for (const auto& shape : tabDataList[currentIndex].shapes) {
                if (shape->contains(newpoint)) {
                    // Действие, если точка попала в фигуру, например, выделение
                    shape->setSelected(true);
                }
            }

        }
    }


    if (event->button() == Qt::MiddleButton) // Проверяем, что нажата средняя кнопка мыши
    {
        // ui.tabWidget->setCursor(QCursor(Qt::PointingHandCursor));
        isDragging = true;
        lastMousePosition = event->pos(); // Сохраняем позицию мыши
    }


    // Проверяем, находится ли клик внутри tabWidget


    if (event->button() == Qt::LeftButton) {

        if (ismovingWholeLine) {
            ismovingWholeLine = false; movingWholeLine = false;
        }
        else if (ui.tabWidget->rect().contains(event->pos()))
        {
            // Определяем индекс вкладки, по которой был клик
            int currentIndex = ui.tabWidget->currentIndex();
            if (currentIndex != -1)
            {
                QPoint globalPos = QCursor::pos(); // Получаем глобальные координаты мыши

                // Предположим, что у вас есть указатель на текущую вкладку:
                QWidget* currentTab = ui.tabWidget->currentWidget();

                QPoint newpoint = currentTab->mapFromGlobal(globalPos);

                for (const auto& shape : tabDataList[ui.tabWidget->currentIndex()].shapes) {
                    // Проверяем, попали ли мы на одну из ручек линии
                    HandleType handle = shape->getHandleAt(newpoint);

                    if (handle == HandleType::StartHandle) {
                        // Логика для перемещения начальной точки линии
                        selectedShape = std::move(shape);  // Устанавливаем выделенную фигуру
                        //  movingStart = true;
                    }
                    else if (handle == HandleType::EndHandle) {
                        // Логика для перемещения конечной точки линии
                        selectedShape = std::move(shape);  // Устанавливаем выделенную фигуру
                        // movingEnd = true;
                    }
                    else if (handle == HandleType::MiddleHandle) {
                        // Логика для перемещения всей линии
                        selectedShape = std::move(shape); // Устанавливаем выделенную фигуру
                        movingWholeLine = true;
                    }
                }


                if (movingWholeLine) {
                    ismovingWholeLine = true;
                    /*  QPoint delta = event->pos() - lastMousePosition;
                      selectedShape->move(delta);
                      lastMousePosition = event->pos();*/
                }
            }

        }
    }


    if (event->button() == Qt::LeftButton)
    {
        if (ui.tabWidget != nullptr) {
            int currentIndex = ui.tabWidget->currentIndex();
            if (currentIndex != -1)
            {
                lastMousePosition = ui.tabWidget->currentWidget()->mapFromGlobal(QCursor::pos());
            }
        }


    }
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
    int currentIndex = ui.tabWidget->currentIndex();

    // Проверка, что индекс корректный и вкладки существуют
    if (currentIndex != -1 && currentIndex < tabDataList.size()) {
        // Обновляем значения смещения сетки на основе переданного delta
        tabDataList[currentIndex].delataX += delta.x();
        tabDataList[currentIndex].delataY += delta.y();
        if (isdraw) {
            clickpoint = QPoint(clickpoint.x() + delta.x(), clickpoint.y() + delta.y());
        }
        // Перерисовываем текущий активный виджет
        QWidget* currentTab = ui.tabWidget->currentWidget();
        if (currentTab) {
            currentTab->update();  // Вызов перерисовки виджета
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
        isDragging = false; // Устанавливаем флаг перетаскивания в false
    }

    QMainWindow::mouseReleaseEvent(event); // Вызов базового метода

    // Перерисовываем активную вкладку
    QWidget* currentTab = ui.tabWidget->currentWidget();
    if (currentTab) {
        currentTab->update();  // Вызов перерисовки активного виджета

    }
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
    // Применяем стиль к ui.tabWidget
    ui.tabWidget->setStyleSheet("QTabWidget { background-color: rgb(33, 40, 48); }");
}

void MyCAD::createNewWindow()
{
    // Если QTabWidget был скрыт, показываем его при создании первого чертежа
    if (!ui.tabWidget->isVisible()) {
        ui.tabWidget->show();
        updateMenusBasedOnTabWidgetVisibility();
    }

    // Создаем unique_ptr на DrawingWidget с указанием родителя
    DrawingWidget* newDrawingWidget = new DrawingWidget(this);

    // Получаем сырый указатель из unique_ptr и передаем его в QTabWidget
    int tabIndex = ui.tabWidget->addTab(std::move(newDrawingWidget), tr("Чертеж %1").arg(ui.tabWidget->count() + 1));
    //int tabIndex = ui.tabWidget->addTab(newDrawingWidget, tr("Чертеж %1").arg(ui.tabWidget->count() + 1));

    // Переключаемся на только что созданную вкладку
    ui.tabWidget->setCurrentIndex(tabIndex);

    // Инициализируем данные для новой вкладки
    TabData newTabData;
    newTabData.delataX = 10; // Устанавливаем начальные значения
    newTabData.delataY = -10;
    tabDataList.push_back(newTabData); // Добавляем данные вкладки в список

    // Переключаемся на только что созданную вкладку
    ui.tabWidget->setCurrentIndex(tabIndex);

    // Убедитесь, что QTabWidget занимает все пространство центрального виджета
    setCentralWidget(ui.tabWidget);

    DrawingWidget* widget = dynamic_cast<DrawingWidget*>(ui.tabWidget->widget(tabIndex));
    if (widget) {
        widget->MyMethod();  // Вызываем метод

    }
    else {
        qDebug() << "Ошибка приведения типа!";
    }

}

bool MyCAD::event(QEvent* e) {
    if (e->type() == QEvent::HoverMove) {
        update();
        if (ismovingWholeLine) {
            QPoint globalPos = QCursor::pos(); // Получаем глобальные координаты мыши

            // Предположим, что у вас есть указатель на текущую вкладку:
            QWidget* currentTab = ui.tabWidget->currentWidget();

            QPoint newpoint = currentTab->mapFromGlobal(globalPos);


            QPoint delta = newpoint - lastMousePosition;
            selectedShape->move(delta);
            lastMousePosition = newpoint;
        }
    }


    qDebug() << "MyCAD Event type:" << e->type();
    return QWidget::event(e);  // Не забывайте передавать событие дальше
}

void MyCAD::initialTabWidget() {
    ui.menuBar->clear();
    QMenu* FileMenu = ui.menuBar->addMenu(tr("  &Файл  "));
    QAction* NewWindowAction = FileMenu->addAction(tr("&Создать"));
    connect(NewWindowAction, &QAction::triggered, this, &MyCAD::createNewWindow);

    FileMenu->addAction(tr("&Открыть"));
    QAction* CloseWindowAction = FileMenu->addAction(tr("&Закрыть"));
    connect(CloseWindowAction, &QAction::triggered, this, &MyCAD::onCloseThisTab);

    // Добавляем сепаратор
    FileMenu->addSeparator();
    // FileMenu стиль сепаратора
    FileMenu->setStyleSheet(
        "QMenu::separator {"
        "   height: 1px;"
        "   background-color: lightgray;"
        "}"
    );


    // Вы можете добавить дополнительные действия после сепаратора, если нужно
    FileMenu->addAction(tr("&Импорт"));
    FileMenu->addAction(tr("&Вставить"));
    FileMenu->addSeparator();
    FileMenu->addAction(tr("&Сохранить"));
    FileMenu->addAction(tr("&Сохранить как..."));

    FileMenu->addSeparator();
    FileMenu->addAction(tr("&Экспорт"));
    FileMenu->addSeparator();
    FileMenu->addAction(tr("&Печать"));
    FileMenu->addSeparator();
    FileMenu->addAction(tr("&Свойства"));
    FileMenu->addSeparator();

    QAction* exitAction = FileMenu->addAction(tr("&Выход"));
    connect(exitAction, &QAction::triggered, this, &MyCAD::onExitThis);

}

void MyCAD::updateMenusBasedOnTabWidgetVisibility()
{
    if (ui.tabWidget->isVisible()) {

        initialTabWidget();

        QMenu* EditMenu = ui.menuBar->addMenu(tr("  &Правка  "));
        QMenu* ViewMenu = ui.menuBar->addMenu(tr("  &Вид  "));
        QMenu* InsertMenu = ui.menuBar->addMenu(tr("  &Вставка  "));
        QMenu* FormatMenu = ui.menuBar->addMenu(tr("  &Формат  "));
        QMenu* ServiceMenu = ui.menuBar->addMenu(tr("  &Сервис  "));
        QMenu* DrawMenu = ui.menuBar->addMenu(tr("  &Рисование  "));
        // Создаем подменю для "Моделирования"
        QMenu* modelingSubMenu = DrawMenu->addMenu(tr("&Моделирование"));

        // Добавляем действия в подменю "Моделирование"
        modelingSubMenu->addAction(tr("&Политело"));
        modelingSubMenu->addAction(tr("&Ящик"));
        modelingSubMenu->addAction(tr("&Клин"));
        modelingSubMenu->addAction(tr("&Сфера"));
        modelingSubMenu->addAction(tr("&Цилиндр"));

        DrawMenu->addSeparator();
        QAction* lineAction = DrawMenu->addAction(tr("&Отрезок"));
        connect(lineAction, &QAction::triggered, this, &MyCAD::onDrawLine);
        DrawMenu->addAction(tr("&Луч"));
        DrawMenu->addAction(tr("&Прямая"));
        DrawMenu->addAction(tr("&Мультилиния"));

        QMenu* DimensionMenu = ui.menuBar->addMenu(tr("  &Размеры  "));
        QMenu* EditMenu2 = ui.menuBar->addMenu(tr("  &Редактировать  "));
        QMenu* ParameterizationMenu = ui.menuBar->addMenu(tr("  &Параметризация  "));
        QMenu* WindowMenu = ui.menuBar->addMenu(tr("  &Окно  "));
        QMenu* HelpMenu = ui.menuBar->addMenu(tr("  &Справка  "));
    }
    else {
        initialTabWidget();
        QMenu* WindowMenu = ui.menuBar->addMenu(tr("  &Окно  "));
        QMenu* HelpMenu = ui.menuBar->addMenu(tr("  &Справка  "));
    }

}


void MyCAD::onDrawLine() {

    ondrawline = true;

    int currentIndex = ui.tabWidget->currentIndex();
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

void MyCAD::drawGrid(QPainter& painter)
{

    if (!ui.tabWidget) {
        return;
    }

    // Получаем текущий активный виджет во вкладке
    int currentIndex = ui.tabWidget->currentIndex();
    if (currentIndex == -1) {
        return;
    }

    QWidget* currentTab = ui.tabWidget->widget(currentIndex);
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
        if (!ui.tabWidget) {
            return;
        }

        // Получаем текущий активный виджет во вкладке
        int currentIndex = ui.tabWidget->currentIndex();
        if (currentIndex == -1) {
            return;
        }

        QWidget* currentTab = ui.tabWidget->widget(currentIndex);
        // Проверяем, что событие происходит на текущей активной вкладке
        if (currentTab) {
            QPoint globalPos = QCursor::pos(); // Получаем глобальные координаты мыши

            // Предположим, что у вас есть указатель на текущую вкладку:
            QWidget* currentTab = ui.tabWidget->currentWidget();

            // Преобразуем глобальные координаты в локальные относительно текущей вкладки
            QPoint localPos = currentTab->mapFromGlobal(globalPos);

            QColor Color(255, 255, 255);  // Цвет основной сетки
            QPen Pen(Color, 1, Qt::SolidLine);
            painter.setPen(Pen);
            painter.drawLine(localPos0.x(), localPos0.y(), localPos.x(), localPos.y());
        }
    }
}

void MyCAD::addShape(std::unique_ptr<Shape>&& shape) {
    // Получаем индекс активной вкладки
    int currentIndex = ui.tabWidget->currentIndex();

    if (currentIndex >= 0 && currentIndex < tabDataList.size()) {
        // Добавляем фигуру в список фигур активной вкладки
        tabDataList[currentIndex].shapes.push_back(std::move(shape));

        // Перерисовываем активную вкладку
        QWidget* currentTab = ui.tabWidget->widget(ui.tabWidget->currentIndex());
        if (currentTab) {
            qDebug() << "Calling repaint on:" << currentTab;
            currentTab->setEnabled(true);
            currentTab->update();
        }
    }
}

void MyCAD::drawShapes(QPainter& painter) {
    // Получаем индекс активной вкладки
    int currentIndex = ui.tabWidget->currentIndex();

    if (currentIndex >= 0 && currentIndex < tabDataList.size()) {
        // Рисуем фигуры только для активной вкладки
        for (const auto& shape : tabDataList[currentIndex].shapes) {
            shape->draw(painter);  // Вызов метода отрисовки фигуры
        }
    }
}

void MyCAD::keyPressEvent(QKeyEvent* event) {
    // Проверяем, что нажата клавиша ESC
    if (isdraw) {
        if (event->key() == Qt::Key_Escape) {
            setCursor(createCustomCrossCursor());
            clickpoint = QPoint(0, 0);
            isdraw = false; ondrawline = false;
        }
        else {
            // Передаем событие базовому классу
            QMainWindow::keyPressEvent(event);

        }
    }

    int currentIndex = ui.tabWidget->currentIndex();
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
