#include "MyCAD.h"
#include <QMessageBox>
#include <QTabWidget>
#include <QWidget>
#include <QCursor>
#include <QTabBar>

#include <QBitmap>
#include <QPainter>
#include <QCursor>
#include <QMouseEvent>

MyCAD::MyCAD(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    ui.tabWidget->hide();

    // Устанавливаем курсор перекрестия для ui.tabWidget
   // ui.tabWidget->setCursor(QCursor(Qt::CrossCursor));

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
    painter.drawLine(cursorSize / 2, cursorSize / 2 + squareside, cursorSize / 2, cursorSize );
    painter.drawLine(cursorSize / 2 + squareside, cursorSize / 2, cursorSize , cursorSize / 2);

    painter.drawLine(cursorSize / 2 - squareside, cursorSize / 2 - squareside, cursorSize / 2 - squareside, cursorSize / 2 + squareside);
    painter.drawLine(cursorSize / 2 - squareside, cursorSize / 2 + squareside, cursorSize / 2 + squareside, cursorSize / 2 + squareside);
    painter.drawLine(cursorSize / 2 + squareside, cursorSize / 2 + squareside, cursorSize / 2 + squareside, cursorSize / 2 - squareside);
    painter.drawLine(cursorSize / 2 + squareside, cursorSize / 2 - squareside, cursorSize / 2 - squareside, cursorSize / 2 - squareside);
    painter.end();

    // Создаем курсор
    QCursor customCrossCursor(cursorPixmap);

    // Устанавливаем кастомный курсор для виджета
    ui.tabWidget->setCursor(customCrossCursor);


    // Проверяем, что ui.menuBar действительно инициализирован и доступен
    if (ui.menuBar) {
        updateMenusBasedOnTabWidgetVisibility();
    }
}

MyCAD::~MyCAD()
{}

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

    // Вызываем базовый обработчик события
    QMainWindow::mousePressEvent(event);
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

    // Переключаемся на только что созданную вкладку
    ui.tabWidget->setCurrentIndex(tabIndex);

    // Убедитесь, что QTabWidget занимает все пространство центрального виджета
    setCentralWidget(ui.tabWidget);
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

