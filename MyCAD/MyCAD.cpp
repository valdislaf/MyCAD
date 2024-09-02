#include <QBitmap>
#include <QCursor>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QTabBar>
#include <QTabWidget>
#include <QWidget>


#include "MyCAD.h"

MyCAD::MyCAD(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    ui.tabWidget->hide();

    // Настройка стиля для ui.tabWidget
    setupTabWidgetStyle();

    // Устанавливаем курсор перекрестия для ui.tabWidget
    // ui.tabWidget->setCursor(QCursor(Qt::CrossCursor));

    // Устанавливаем пользовательский курсор
    QCursor customCrossCursor = createCustomCrossCursor();

    // Устанавливаем кастомный курсор для виджета
    ui.tabWidget->setCursor(customCrossCursor);


    // Проверяем, что ui.menuBar действительно инициализирован и доступен
    if (ui.menuBar) {
        updateMenusBasedOnTabWidgetVisibility();
    }

    // Подписываемся на сигнал смены вкладок
    connect(ui.tabWidget, &QTabWidget::currentChanged, this, &MyCAD::onTabChanged);
    TabData newTabData;
    newTabData.delataX = 10; // Устанавливаем начальные значения
    newTabData.delataY = -10;
    tabDataList.append(newTabData); // Добавляем данные вкладки в список

}

MyCAD::~MyCAD()
{}

void MyCAD::onTabChanged(int index)
{
   
    // Обработка изменения вкладки, например, загрузка настроек сетки для выбранной вкладки
    if (index >= 0) {
        QWidget* currentTab = ui.tabWidget->widget(index);
        int a = 0;
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

void MyCAD::drawGrid()
{
    // Получаем текущий активный виджет во вкладке
    int currentIndex = ui.tabWidget->currentIndex();
    if (currentIndex == -1) {
        return;  // Если нет активной вкладки, ничего не делаем
    }

    QWidget* currentTab = ui.tabWidget->widget(currentIndex);
    if (!currentTab) {
        return;  // Если текущая вкладка не определена, ничего не делаем
    }

    // Устанавливаем кастомное рисование
    currentTab->installEventFilter(this);
}

bool MyCAD::eventFilter(QObject* obj, QEvent* event)
{
    /*int delataX = 100;
    int delataY = -100;*/

    if (event->type() == QEvent::Paint) {
        QWidget* widget = dynamic_cast<QWidget*>(obj);

        if (!widget) {
            return false;
        }

        // Получаем текущий активный виджет во вкладке
        int currentIndex = ui.tabWidget->currentIndex();
        if (currentIndex == -1) {
            return false;
        }

        QWidget* currentTab = ui.tabWidget->widget(currentIndex);
        int delataX = tabDataList[currentIndex].delataX;
        int delataY = tabDataList[currentIndex].delataY;
        // Проверяем, что событие происходит на текущей активной вкладке
        if (widget == currentTab) {
            QPainter painter(widget);

            // Устанавливаем параметры для рисования сетки
            int gridSize = 37;  // Размер ячейки сетки
            int widgetWidth = widget->width();
            int widgetHeight = widget->height();


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

            return true;  // Сообщаем, что событие обработано
        }
    }

    // Для остальных случаев используем стандартную обработку
    return QMainWindow::eventFilter(obj, event);
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
    int delataX  =tabDataList[currentIndex].delataX;
    int delataY= tabDataList[currentIndex].delataY;

    if (delataX<0 || (widgetHeight + delataY) < 0 || delataX > widgetWidth || delataY>widgetHeight)
    {
        delataX = 10;
        delataY = -10;
    }

    painter.drawLine( delataX, widgetHeight + delataY, delataX, widgetHeight - cursorSize / 2 + delataY);
    painter.drawLine( delataX, widgetHeight + delataY, cursorSize / 2 + delataX, widgetHeight + delataY);

    painter.drawLine(-squareside + delataX, widgetHeight - squareside + delataY, -squareside + delataX, widgetHeight + squareside + delataY);
    painter.drawLine(-squareside + delataX, widgetHeight + squareside + delataY, squareside + delataX, widgetHeight + squareside + delataY);
    painter.drawLine(squareside + delataX, widgetHeight + squareside + delataY, squareside + delataX, widgetHeight - squareside + delataY);
    painter.drawLine(squareside + delataX, widgetHeight - squareside + delataY, -squareside + delataX, widgetHeight - squareside + delataY);
    // Отладочные сообщения
    qDebug() << "delataX: " << delataX;
    qDebug() << "widgetHeight + delataY: " << (widgetHeight + delataY);
    qDebug() << "widgetHeight: " << widgetHeight;
    qDebug() << "widgetWidth:  " << widgetWidth;

}

void MyCAD::mousePressEvent(QMouseEvent* event)
{
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
                    QMessageBox::information(this, "Tab Click", "Клик по вкладке с индексом: " + QString::number(currentIndex));
                }
            }
        }
    }

    if (event->button() == Qt::MiddleButton) // Проверяем, что нажата средняя кнопка мыши
    {
        ui.tabWidget->setCursor(QCursor(Qt::PointingHandCursor));
        isDragging = true;
        lastMousePosition = event->pos(); // Сохраняем позицию мыши
    }

    // Вызываем базовый обработчик события
    QMainWindow::mousePressEvent(event);
}

void MyCAD::mouseMoveEvent(QMouseEvent* event)
{
    if (isDragging) // Если мышь перетаскивается
    {
        QPoint delta = event->pos() - lastMousePosition; // Рассчитываем смещение
        updateGridPosition(delta); // Обновляем позицию сетки
        lastMousePosition = event->pos(); // Обновляем последнюю позицию мыши
        drawGrid(); // Перерисовываем сетку
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
       
        // Перерисовываем текущий активный виджет
        QWidget* currentTab = ui.tabWidget->currentWidget();



        if (currentTab) {
            currentTab->update();  // Вызов перерисовки виджета
        }
    }
}


void MyCAD::mouseReleaseEvent(QMouseEvent* event)
{
    // Устанавливаем пользовательский курсор
    QCursor customCrossCursor = createCustomCrossCursor();

    // Устанавливаем кастомный курсор для виджета
    ui.tabWidget->setCursor(customCrossCursor);

    if (event->button() == Qt::MiddleButton) // Проверяем, что отпущена средняя кнопка мыши
    {
        isDragging = false; // Устанавливаем флаг перетаскивания в false
    }
    QMainWindow::mouseReleaseEvent(event); // Вызов базового метода
}

void MyCAD::onDrawLine()
{
    // Здесь будет логика, которая выполняется при нажатии на пункт "Отрезок"
    QMessageBox::information(this, "Отрезок", "Вы выбрали 'Отрезок' из меню!");
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

    // Создаем новый виджет, который будет представлять собой новый чертеж
    QWidget* newDrawingWidget = new QWidget();

    // Опционально: настройка виджета для рисования

    // Добавляем новую вкладку с виджетом
    int tabIndex = ui.tabWidget->addTab(newDrawingWidget, tr("Чертеж %1").arg(ui.tabWidget->count()));

    // Инициализируем данные для новой вкладки
    TabData newTabData;
    newTabData.delataX = 10; // Устанавливаем начальные значения
    newTabData.delataY = -10;
    tabDataList.append(newTabData); // Добавляем данные вкладки в список

    // Переключаемся на только что созданную вкладку
    ui.tabWidget->setCurrentIndex(tabIndex);

    // Убедитесь, что QTabWidget занимает все пространство центрального виджета
    setCentralWidget(ui.tabWidget);

    // Перерисовываем текущий активный виджет
    QWidget* currentTab = ui.tabWidget->currentWidget();
    if (currentTab) {
        currentTab->update();  // Вызов перерисовки виджета
    }

    // Показ сетки
    drawGrid();
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

