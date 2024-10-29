#pragma once
#include <QWidget>
#include <QPainter>
#include <QPoint>
#include "HandleManager.h"


// Абстрактный базовый класс для всех геометрических объектов
class Shape : public QWidget {

    Q_OBJECT

private:
    ShapePoints points;
    HandleManager* handleManager;
    bool iscursorhovershape = false; //курсор над
    bool isSelected = false;  // Флаг выделения
    bool moveSelected = false;  // Флаг снятия выделения при перемещении
    Colors colors;
    Selected selected;

public:
    const ShapePoints& getPoints() const;
    void setPoints(const ShapePoints& newPoints);
    void setHandleSize(int size);
    HandleManager* getHandle()const;
    ~Shape();
    Colors getColors()const;
	void setColorsStart(QColor color);
    void setColorEnd(QColor color);
    void setColorMiddle(QColor color);
    void setColorsLeft(QColor color);
    void setColorsTop(QColor color);
    void setColorsRight(QColor color);
    void setColorsBottom(QColor color);


    Selected getSelected() const;
    void setisStart(bool selected);
    void setisMiddle(bool selected);
    void setisEnd(bool selected);
    void setisLeft(bool selected);
    void setisTop(bool selected);
    void setisRight(bool selected);
    void setisBottom(bool selected);
    // Чисто виртуальные методы, которые должны быть реализованы в наследниках

    virtual void draw(QPainter& painter) const = 0;

    virtual void move(const QPoint& delta);
    virtual void moveStart(const QPoint& delta);
   
    virtual void moveEnd(const QPoint& delta);
    virtual void moveRadius(const int radius);


    virtual bool contains(const QPoint& point) = 0;

    virtual void setSelected(bool selected) ;
    virtual bool getisSelected()const;

    virtual void setMoveSelected(bool selected) ;
    virtual bool getisMoveSelected()const;

    virtual  HandleType getHandleAt(const QPoint& point) = 0;
    // Метод клонирования
    virtual std::shared_ptr<Shape> clone() const = 0;


    virtual void setCoords(const QPoint& startPoint, const QPoint& endPoint, bool isSelected);
    virtual void setCentre(const QPoint& startPoint, const int radius, bool isSelected);


    virtual QPoint getstartPoint()const;
    virtual QPoint getendPoint()const;
    virtual QPoint getmiddlePoint()const;
    virtual int getradius()const;
    virtual void setcolorPoint()const;
    virtual  QColor getColorPoint()const;
    virtual void resetColor();

    virtual bool getisStart() const;
    virtual bool getisEnd()const;
    virtual bool getisMiddle()const;
    virtual bool getisLeft() const ;
    virtual bool getisRight()const ;
    virtual bool getisTop()const ;
    virtual bool getisBottom()const;
    virtual Type name() const = 0;

    virtual void setisover(bool iscursorhovershape_); //курсор над 
    virtual bool getisover()const ; //курсор над ?

    virtual  QRect getStartHandle() const;  // Возвращает область квадрата в начале линии
    virtual  QRect getEndHandle() const;   // Возвращает область квадрата в конце линии
    virtual  QRect getMiddleHandle() const ; // Возвращает область квадрата в середине линии
    virtual  QRect getLeftHandle() const;
    virtual  QRect getToptHandle() const;
    virtual  QRect getRighttHandle() const;
    virtual  QRect getBottomtHandle() const;


    //virtual bool isHandleSelected(HandleType handle) const = 0;
    virtual  bool isHandleSelected(const HandleType& handle, const QPoint& point) const = 0;
    virtual void captureCursorForAllHandles(QTabWidget* tabWidget, const QPoint& point, const ShapePoints& points) = 0;
};

// Класс отрезка, наследник Shape
class Line : public Shape {

    Q_OBJECT

public:
    Line(const QPoint& startPoint, const QPoint& endPoint);

    Line(const Line& other);
 

    void draw(QPainter& painter) const override;
    void move(const QPoint& delta) override;
    void moveStart(const QPoint& delta) override;
    void moveEnd(const QPoint& delta) override;
   
    bool contains(const QPoint& point)  override;

    HandleType getHandleAt(const QPoint& point)  override;
    void setHandleState(HandleType handleType);
    // Метод клонирования
    std::shared_ptr<Shape> clone() const override;
    void setCoords(const QPoint& startPoint, const QPoint& endPoint, bool isSelected) override;
    
    QPoint getstartPoint()const override;
    QPoint getendPoint()const override;
    QPoint getmiddlePoint()const override;    

   
   
    bool getisStart()const override;
    bool getisEnd()const override;
    bool getisMiddle()const override;  
    Type name() const override;
   
    QRect getStartHandle() const override;  // Возвращает область квадрата в начале линии
    QRect getEndHandle() const override;    // Возвращает область квадрата в конце линии
    QRect getMiddleHandle() const override; // Возвращает область квадрата в середине линии
    
    bool isHandleSelected(const HandleType& handle, const QPoint& point) const override;
    void captureCursorForAllHandles(QTabWidget* tabWidget, const QPoint& point, const ShapePoints& points) override;

private:

  
    // Размеры квадратов на концах и в середине линии
    static constexpr int handleSize = 10;
  
};

// В дальнейшем можно добавлять другие классы (Circle, Rectangle и т.д.)

class Circle : public Shape {

    Q_OBJECT

public:
    Circle(const QPoint& startPoint, const int radius);

    Circle(const Circle& other);
   

    void draw(QPainter& painter) const override;
    void move(const QPoint& delta) override;
    void moveStart(const QPoint& delta) override;
    void moveRadius(const int radius)override;
    bool contains(const QPoint& point)  override;

    HandleType getHandleAt(const QPoint& point)  override;
    void setHandleState(HandleType handleType);
    // Метод клонирования

    std::shared_ptr<Shape> clone() const override; 

    void setCentre(const QPoint& startPoint, const int radius, bool isSelected) override;

    QPoint getstartPoint()const override;
    int getradius()const override;
   
    bool getisStart()const override;  
    bool getisLeft()const override;
    bool getisRight()const override;
    bool getisTop()const override;
    bool getisBottom()const override;
    Type name() const override;

    QRect getStartHandle() const override;  // Возвращает область квадрата в начале линии    
    QRect getLeftHandle() const override;
    QRect getToptHandle() const override;
    QRect getRighttHandle() const override;
    QRect getBottomtHandle() const override;

    bool isHandleSelected(const HandleType& handle, const QPoint& point) const override;

    void captureCursorForAllHandles(QTabWidget* tabWidget, const QPoint& point, const ShapePoints& points) override;
    

private:
  
    // Размеры квадратов на концах и в середине линии
    static constexpr int handleSize = 10;

   
   
    
};