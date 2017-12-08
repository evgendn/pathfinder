#ifndef CELL_H
#define CELL_H

#include <string>

class Cell
{
private:
    bool leftEdge;
    bool rightEdge;
    bool topEdge;
    bool bottomEdge;

public:
    Cell(bool, bool, bool, bool);
    inline void setLeftEdge(bool b) { leftEdge = b; }
    inline void setRightEdge(bool b) { rightEdge = b; }
    inline void setTopEdge(bool b) { topEdge = b; }
    inline void setBottomEdge(bool b) { bottomEdge = b; }
    inline bool getLeftEdge() { return leftEdge; }
    inline bool getRightEdge() { return rightEdge; }
    inline bool getTopEdge() { return topEdge; }
    inline bool getBottomEdge() { return bottomEdge; }
    bool print() { return rightEdge + topEdge +
                            + leftEdge + bottomEdge; }

   Cell &operator = (const bool &val);
};

#endif // CELL_H
