#pragma once
#include <QWidget>
#include <QPainter>
#include <QPoint>

enum HandleType {
    None,
    StartHandle,
    EndHandle,
    MiddleHandle,
    LeftHandle,
    TopHandle,
    RightHandle,
    BottomHandle
};

enum Type {
    line,
    circle
};
// Абстрактный базовый класс для всех геометрических объектов
class Shape : public QWidget {

    Q_OBJECT

public:
    virtual ~Shape() = default;

    // Чисто виртуальные методы, которые должны быть реализованы в наследниках
    virtual void draw(QPainter& painter) const = 0;
    virtual void move(const QPoint& delta) = 0;
    virtual void moveStart(const QPoint& delta) = 0;
    virtual void moveEnd(const QPoint& delta) = 0;
    virtual void moveRadius(const int radius) = 0;
    virtual bool contains(const QPoint& point) const = 0;
    virtual void setSelected(bool selected) = 0;
    virtual void setMoveSelected(bool selected) = 0;
    virtual  HandleType getHandleAt(const QPoint& point) = 0;
    // Метод клонирования
   virtual std::shared_ptr<Shape> clone() const = 0;
    virtual void setCoords(const QPoint& startPoint, const QPoint& endPoint, bool isSelected) = 0;
    virtual void setCentre(const QPoint& startPoint, const int radius, bool isSelected) = 0;
    virtual QPoint getstartPoint()const = 0;
    virtual QPoint getendPoint()const = 0;
    virtual QPoint getleftPoint()const = 0;
    virtual QPoint gettopPoint()const = 0;
    virtual QPoint getrightPoint()const = 0;
    virtual QPoint getbottomPoint()const = 0;
    virtual int getradius()const = 0;

    virtual bool getisSelected()const = 0;
    virtual void resetColor()  = 0;

    virtual bool getisStart() const = 0;
    virtual bool getisEnd()const = 0;
    virtual bool getisMiddle()const = 0;
    virtual bool getisLeft() const = 0;
    virtual bool getisRight()const = 0;
    virtual bool getisTop()const = 0;
    virtual bool getisBottom()const = 0;
    virtual Type name() const = 0;
    virtual void setisover(bool iscursorhovershape) = 0; //курсор над 
};

// Класс отрезка, наследник Shape
class Line : public Shape {

    Q_OBJECT

public:
    Line(const QPoint& startPoint, const QPoint& endPoint);

    Line(const Line& other);
    ~Line();

    void draw(QPainter& painter) const override;
    void move(const QPoint& delta) override;
    void moveStart(const QPoint& delta) override;
    void moveEnd(const QPoint& delta) override;
    void moveRadius(const int radius)override;
    bool contains(const QPoint& point) const override;
    void setSelected(bool selected) override;
    void setMoveSelected(bool selected)override;
    HandleType getHandleAt(const QPoint& point)  override;
    // Метод клонирования
    std::shared_ptr<Shape> clone() const override;
    void setCoords(const QPoint& startPoint, const QPoint& endPoint, bool isSelected) override;
    void setCentre(const QPoint& startPoint, const int radius, bool isSelected) override;
    QPoint getstartPoint()const override;
    QPoint getendPoint()const override;
     QPoint getleftPoint()const override;
     QPoint gettopPoint()const override;
     QPoint getrightPoint()const override;
     QPoint getbottomPoint()const override;
     int getradius()const override;
    bool getisSelected()const override;
    void resetColor() override;
     bool getisStart()const override;
     bool getisEnd()const override;
     bool getisMiddle()const override;
      bool getisLeft()const override;
      bool getisRight()const override;
      bool getisTop()const override;
      bool getisBottom()const override;
      Type name() const override;
      void setisover(bool iscursorhovershape) override;
private:
   
    QPoint startPoint;
    QPoint endPoint;
    bool isSelected = false;  // Флаг выделения
    bool moveSelected = false;  // Флаг снятия выделения при перемещении
    QColor ColorStartPoint = QColor(0, 127, 255);
    QColor ColorEndPoint = QColor(0, 127, 255);
    QColor ColorMiddlePoint = QColor(0, 127, 255);
    // Размеры квадратов на концах и в середине линии
    static constexpr int handleSize = 10;

    QRect getStartHandle() const;  // Возвращает область квадрата в начале линии
    QRect getEndHandle() const;    // Возвращает область квадрата в конце линии
    QRect getMiddleHandle() const; // Возвращает область квадрата в середине линии
    bool isStart = false;  // Флаг выделения
    bool isEnd = false;  // Флаг выделения
    bool isMiddle = false;  // Флаг выделения
    bool iscursorhovershape = false; //курсор над 

};

// В дальнейшем можно добавлять другие классы (Circle, Rectangle и т.д.)

class Circle : public Shape {

    Q_OBJECT

public:
    Circle(const QPoint& startPoint, const int radius);

    Circle(const Circle& other);
    ~Circle();

    void draw(QPainter& painter) const override;
    void move(const QPoint& delta) override;
    void moveStart(const QPoint& delta) override;
    void moveEnd(const QPoint& delta) override;
    void moveRadius(const int radius)override;
    bool contains(const QPoint& point) const override;
    void setSelected(bool selected) override;
    void setMoveSelected(bool selected)override;
    HandleType getHandleAt(const QPoint& point)  override;
    // Метод клонирования
    std::shared_ptr<Shape> clone() const override;
    void setCoords(const QPoint& startPoint, const QPoint& endPoint, bool isSelected) override;
    void setCentre(const QPoint& startPoint, const int radius, bool isSelected) override;
    QPoint getstartPoint()const override;
    QPoint getendPoint()const override;
    QPoint getleftPoint()const override;
    QPoint gettopPoint()const override;
    QPoint getrightPoint()const override;
    QPoint getbottomPoint()const override;
    int getradius()const override;

    bool getisSelected()const override;
    void resetColor() override;
    bool getisStart()const override;
    bool getisEnd()const override;
    bool getisMiddle()const override;
    bool getisLeft()const override;
    bool getisRight()const override;
    bool getisTop()const override;
    bool getisBottom()const override;
    Type name() const override;
    void setisover(bool iscursorhovershape) override;

private:
    QPoint startPoint;
    QPoint leftPoint;
    QPoint topPoint;
    QPoint rightPoint;
    QPoint bottomPoint;
    int radius;
    bool isSelected = false;  // Флаг выделения
    bool moveSelected = false;  // Флаг снятия выделения при перемещении
    QColor ColorStartPoint = QColor(0, 127, 255);
    QColor ColorLeftPoint = QColor(0, 127, 255);
    QColor ColorTopPoint = QColor(0, 127, 255);
    QColor ColorRightPoint = QColor(0, 127, 255);
    QColor ColorBottomPoint = QColor(0, 127, 255);

    // Размеры квадратов на концах и в середине линии
    static constexpr int handleSize = 10;

    QRect getStartHandle() const;  // Возвращает область квадрата в начале линии
    QRect getLeftHandle() const;
    QRect getToptHandle() const;
    QRect getRighttHandle() const;
    QRect getBottomtHandle() const;
    bool isStart = false;  // Флаг выделения
    bool isLeft = false;  // Флаг выделения
    bool isTop = false;  // Флаг выделения
    bool isRight = false;  // Флаг выделения
    bool isBottom = false;  // Флаг выделения
    bool iscursorhovershape = false; //курсор над 
};      