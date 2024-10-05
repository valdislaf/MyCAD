#include "MenuMain.h"

MenuMain::MenuMain(QMenuBar* menuBar_) : menuBar(menuBar_)
{
    menuBar->clear();
    FileMenu = menuBar->addMenu(tr("  &Файл  "));
    NewWindowAction = FileMenu->addAction(tr("&Создать"));
    FileMenu->addAction(tr("&Открыть"));
    CloseWindowAction = FileMenu->addAction(tr("&Закрыть"));
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
    exitAction = FileMenu->addAction(tr("&Выход"));
    EditMenu = menuBar->addMenu(tr("  &Правка  "));
    ViewMenu = menuBar->addMenu(tr("  &Вид  "));
    InsertMenu = menuBar->addMenu(tr("  &Вставка  "));
    FormatMenu = menuBar->addMenu(tr("  &Формат  "));
    ServiceMenu = menuBar->addMenu(tr("  &Сервис  "));
    DrawMenu = menuBar->addMenu(tr("  &Рисование  "));
    // Создаем подменю для "Моделирования"
    modelingSubMenu = DrawMenu->addMenu(tr("&Моделирование"));
    // Добавляем действия в подменю "Моделирование"
    modelingSubMenu->addAction(tr("&Политело"));
    modelingSubMenu->addAction(tr("&Ящик"));
    modelingSubMenu->addAction(tr("&Клин"));
    modelingSubMenu->addAction(tr("&Сфера"));
    modelingSubMenu->addAction(tr("&Цилиндр"));
    DrawMenu->addSeparator();
    lineAction = DrawMenu->addAction(tr("&Отрезок"));
    DrawMenu->addAction(tr("&Луч"));
    DrawMenu->addAction(tr("&Прямая"));
    DrawMenu->addAction(tr("&Мультилиния"));
    DrawMenu->addSeparator();
    modecircleSubMenu = DrawMenu->addMenu(tr("&Круг"));
    circleAction = modecircleSubMenu->addAction(tr("&Центр, радиус"));
    circleAction2 = modecircleSubMenu->addAction(tr("&Центр, диаметр"));
    DimensionMenu = menuBar->addMenu(tr("  &Размеры  "));
    EditMenu2 = menuBar->addMenu(tr("  &Редактировать  "));
    ParameterizationMenu = menuBar->addMenu(tr("  &Параметризация  "));
    WindowMenu = menuBar->addMenu(tr("  &Окно  "));
    HelpMenu = menuBar->addMenu(tr("  &Справка  "));
}

MenuMain::~MenuMain()
{ 
      qDebug() << "destruct ~MenuMain()";
}

QAction* MenuMain::GetNewWindowAction()
{
    return NewWindowAction;
}

QAction* MenuMain::GetCloseWindowAction()
{
    return CloseWindowAction;
}

QAction* MenuMain::GetexitAction()
{
    return exitAction;
}

QAction* MenuMain::GetlineAction()
{
    return lineAction;
}

QAction* MenuMain::GetcircleAction()
{
    return circleAction;
}
