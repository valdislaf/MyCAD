#pragma once
#include <QWidget>
#include <QPainter>
#include <QPoint>

enum HandleType {
    None,
    StartHandle,
    EndHandle,
    MiddleHandle
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
    virtual bool contains(const QPoint& point) const = 0;
    virtual void setSelected(bool selected) = 0;
    virtual  HandleType getHandleAt(const QPoint& point) = 0;
    // Метод клонирования
    virtual std::shared_ptr<Shape> clone() const = 0;
    virtual void setCoords(const QPoint& startPoint, const QPoint& endPoint, bool isSelected) = 0;
    virtual void setCentre(const QPoint& startPoint, const int radius, bool isSelected) = 0;
    virtual QPoint getstartPoint()const = 0;
    virtual QPoint getendPoint()const = 0;
    virtual bool getisSelected()const = 0;
    virtual void resetColor() = 0;

    virtual bool getisStart() = 0;
    virtual bool getisEnd() = 0;
    virtual bool getisMiddle() = 0;
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
    bool contains(const QPoint& point) const override;
    void setSelected(bool selected) override;
    HandleType getHandleAt(const QPoint& point)  override;
    // Метод клонирования
    std::shared_ptr<Shape> clone() const override;
    void setCoords(const QPoint& startPoint, const QPoint& endPoint, bool isSelected) override;
    void setCentre(const QPoint& startPoint, const int radius, bool isSelected) override;
    QPoint getstartPoint()const override;
    QPoint getendPoint()const override;
    bool getisSelected()const override;
    void resetColor() override;
     bool getisStart()override;
     bool getisEnd() override;
     bool getisMiddle()override;
private:
    QPoint startPoint;
    QPoint endPoint;
    bool isSelected = false;  // Флаг выделения
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
    bool contains(const QPoint& point) const override;
    void setSelected(bool selected) override;
    HandleType getHandleAt(const QPoint& point)  override;
    // Метод клонирования
    std::shared_ptr<Shape> clone() const override;
    void setCoords(const QPoint& startPoint, const QPoint& endPoint, bool isSelected) override;
    void setCentre(const QPoint& startPoint, const int radius, bool isSelected) override;
    QPoint getstartPoint()const override;
    QPoint getendPoint()const override;
    bool getisSelected()const override;
    void resetColor() override;
    bool getisStart()override;
    bool getisEnd() override;
    bool getisMiddle()override;
private:
    QPoint startPoint;
    int radius;
    bool isSelected = false;  // Флаг выделения
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
};