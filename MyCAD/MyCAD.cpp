#include "MyCAD.h"
#include <QMessageBox>

MyCAD::MyCAD(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);   

    // Проверяем, что ui.menuBar действительно инициализирован и доступен
    if (ui.menuBar) {
        QMenu* FileMenu = ui.menuBar->addMenu(tr("  &Файл  "));
        FileMenu->addAction(tr("&Создать"));
        FileMenu->addAction(tr("&Открыть"));
        FileMenu->addAction(tr("&Закрыть"));

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

        // Можно добавить действия и в это меню
    }
}

MyCAD::~MyCAD()
{}

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
