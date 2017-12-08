#include "cell.h"

Cell::Cell(bool left, bool right, bool top, bool bottom):
    leftEdge(left), rightEdge(right), topEdge(top), bottomEdge(bottom) {}

Cell &Cell::operator =(const bool &val)
{
    this->leftEdge = val;
    this->rightEdge = val;
    this->topEdge = val;
    this->bottomEdge = val;

    return *this;
}

