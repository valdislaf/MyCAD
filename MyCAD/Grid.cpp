#include "Grid.h"

Grid::Grid(QWidget* currentTab, int gridSize, int delataX, int delataY) :
    currentTab(currentTab), 
    gridSize(gridSize), 
    delataX(delataX), 
    delataY(delataY)
{}

int Grid::setDelataX(int dx)
{
	return delataX = dx;
}

int Grid::setDelataY(int dy)
{
    return delataY = dy;
}

void Grid::setGridSize(int size) {
    gridSize = size;
}

void Grid::setTab(QWidget* currTab) {
    currentTab = currTab;
}


void Grid::draw(QPainter& painter)
{
  //if (gridSize < 25) { gridSize = 37; }
   // int widgetWidth = 100;// currentTab->width();
   // int widgetHeight = 100;// currentTab->height();
    int widgetWidth =  currentTab->width();
    int widgetHeight = currentTab->height();
    // Создаем QPen для основных линий сетки
    QColor mainGridColor(38, 44, 55);  // Цвет основной сетки
    QPen mainGridPen(mainGridColor, 2, Qt::DotLine);

    // Создаем QPen для линий, которые отображаются каждые 5 шагов
    QColor highlightedGridColor(48, 54, 69);  // Цвет выделенной сетки
    QPen highlightedGridPen(highlightedGridColor, 2, Qt::SolidLine);
    int otstup = 10;
    int yl = widgetHeight + std::abs(delataY);
    int xl = widgetWidth + std::abs(delataX);
    int yl2 = std::abs(delataY) + widgetHeight;

    // Рисуем вертикальные линии сетки вправо от начала координат
    for (int x = 0; x < xl; x += gridSize) {
        if ((x + delataX > otstup) && (x + delataX < widgetWidth- otstup)
            ) {
            painter.setPen(mainGridPen);
           
            if (yl > widgetHeight- otstup) {yl = widgetHeight- otstup;}
            if (yl < otstup) { yl = otstup; }

            painter.drawLine(x + delataX, otstup, x + delataX, yl);
           /* if (x == 5* gridSize) {
                qDebug() << "x " << (x + delataX); 
            }*/
        }
    }
    // Рисуем вертикальные линии сетки вправо от начала координат
    for (int x = 0; x < xl; x += gridSize * 5) {
        if ((x + delataX > otstup) && (x + delataX < widgetWidth - otstup)
            ) {
            painter.setPen(highlightedGridPen);
           
            if (yl > widgetHeight - otstup) { yl = widgetHeight - otstup; }
            if (yl < otstup) { yl = otstup; }
            painter.drawLine(x + delataX, otstup, x + delataX, yl);
        }
    }
    // Рисуем вертикальные линии сетки влево от начала координат
    for (int x = 0; x > -std::abs(delataX); x -= gridSize) {
        if ((x + delataX > otstup) && (x + delataX < widgetWidth- otstup)
            ) {
            painter.setPen(mainGridPen);
         
            if (yl > widgetHeight - otstup) { yl = widgetHeight - otstup; }
            if (yl < otstup) { yl = otstup; }
            painter.drawLine(x + delataX, otstup, x + delataX, yl);
        }
    }
    // Рисуем вертикальные линии сетки влево от начала координат
    for (int x = 0; x > -std::abs(delataX); x -= gridSize*5) {
        if ((x + delataX > otstup) && (x + delataX < widgetWidth - otstup)
            ) {
            painter.setPen(highlightedGridPen);
         
            if (yl > widgetHeight - otstup) { yl = widgetHeight - otstup; }
            if (yl < otstup) { yl = otstup; }
            painter.drawLine(x + delataX, otstup, x + delataX, yl);
        }
    }
    // Рисуем горизонтальные линии сетки вверх от начала координат
    int y5 = 0;
    for (int y = widgetHeight; y > -std::abs(delataY); y -= gridSize) {
        if ((y + delataY > otstup) && (y + delataY < widgetHeight - otstup)
            ) {
            painter.setPen(mainGridPen);
           
            if (xl > widgetWidth - otstup) { xl = widgetWidth - otstup; }
            if (xl < otstup) { xl = otstup; }
            painter.drawLine(otstup, y + delataY,xl , y + delataY);
            /*if (y == widgetHeight- 5 * gridSize) {
                qDebug() << "y " << (y + delataY)- widgetHeight;
            }*/
            y5 += gridSize;
        }
    }
    // Рисуем горизонтальные линии сетки вверх от начала координат
     y5 = 0;
    for (int y = widgetHeight; y > -std::abs(delataY); y -= gridSize*5) {
        if ((y + delataY > otstup) && (y + delataY < widgetHeight - otstup)
            ) {
            painter.setPen(highlightedGridPen);
        
            if (xl > widgetWidth - otstup) { xl = widgetWidth - otstup; }
            if (xl < otstup) { xl = otstup; }
            painter.drawLine(otstup, y + delataY, xl, y + delataY);
            y5 += gridSize;
        }
    }
     // Рисуем горизонтальные линии сетки вниз от начала координат
    y5 = 0;
    for (int y = widgetHeight; y < yl2; y += gridSize) {
        if ((y + delataY > otstup) && (y + delataY < widgetHeight - otstup)
            ) {
            painter.setPen(mainGridPen);
          
            if (xl > widgetWidth - otstup) { xl = widgetWidth - otstup; }
            if (xl < otstup) { xl = otstup; }
            painter.drawLine(otstup, y + delataY, xl, y + delataY);
            y5 += gridSize;
        }
    }
    // Рисуем горизонтальные линии сетки вниз от начала координат
    y5 = 0;
    for (int y = widgetHeight; y < yl2; y += gridSize*5) {
        if ((y + delataY > otstup) && (y + delataY < widgetHeight - otstup)
            ) {
            painter.setPen(highlightedGridPen);

           
            if (xl > widgetWidth - otstup) { xl = widgetWidth - otstup; }
            if (xl < otstup) { xl = otstup; }
            painter.drawLine(otstup, y + delataY, xl, y + delataY);
            y5 += gridSize;
        }
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

