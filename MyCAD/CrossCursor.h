#pragma once
#include <QPainter>
#include <QCursor>

class CrossCursor {

public:
	CrossCursor(int size, QPen pen, int squareside);
	QCursor createCursor() const;

private:
	int cursorSize;
	QPen pen;
	int squareside;
};