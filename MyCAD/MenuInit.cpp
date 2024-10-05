#include "MenuInit.h"

MenuInit::MenuInit(QMenuBar* menuBar_) : menuBar(menuBar_)
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
    WindowMenu = menuBar->addMenu(tr("  &Окно  "));
    HelpMenu = menuBar->addMenu(tr("  &Справка  "));
}

MenuInit::~MenuInit()
{
   qDebug() << "destruct ~MenuInit()";
}

QAction* MenuInit::GetNewWindowAction()
{
    return NewWindowAction;
}

QAction* MenuInit::GetCloseWindowAction()
{
    return CloseWindowAction;
}

QAction* MenuInit::GetexitAction()
{
    return exitAction;
}
