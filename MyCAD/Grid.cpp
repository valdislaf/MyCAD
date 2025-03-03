#include "Grid.h"

Grid::Grid(QWidget* currentTab, int gridSize, int delataX, int delataY) :
    currentTab(currentTab), 
    gridSize(gridSize), 
    delataX(delataX), 
    delataY(delataY)
{}

void Grid::draw(QPainter& painter)
{
    int widgetWidth = currentTab->width();
    int widgetHeight = currentTab->height();


    //int xmax = 0; //int ymax = 0;
    // Создаем QPen для основных линий сетки
    QColor mainGridColor(38, 44, 55);  // Цвет основной сетки
    QPen mainGridPen(mainGridColor, 1, Qt::SolidLine);

    // Создаем QPen для линий, которые отображаются каждые 5 шагов
    QColor highlightedGridColor(48, 54, 65);  // Цвет выделенной сетки
    QPen highlightedGridPen(highlightedGridColor, 1, Qt::SolidLine);

    // Рисуем вертикальные линии сетки вправо от начала координат
    for (int x = 0; x < widgetWidth + std::abs(delataX); x += gridSize) {
        if ((x / gridSize) % 5 == 0) {
            painter.setPen(highlightedGridPen);
        }
        else {
            painter.setPen(mainGridPen);
        }
        painter.drawLine(x + delataX, 0, x + delataX, widgetHeight + std::abs(delataY));
       // xmax = x;
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
       // xmax = x;
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

void Grid::CoordinateAxes(QPainter& painter, QWidget* currentTab)
{
    
    int widgetHeight = currentTab->height();
    int widgetWidth = currentTab->width();
    painter.setPen(QPen(Qt::white, 1)); // Устанавливаем цвет и толщину линий
    int cursorSize = 100;
    // Рисуем перекрестие
    int squareside = 5; // сторона внутреннего квадрата
 
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