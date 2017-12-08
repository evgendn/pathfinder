#ifndef NODE_H
#define NODE_H


class Node
{
    int xPos;
    int yPos;
    int cost;
    int priority;
    int manhattanDistance;


public:
    Node(const int &_xPos, const int &_yPos, const int &_cost)
    {
        xPos = _xPos;
        yPos = _yPos;
        cost = _cost;
        priority = 0;
        manhattanDistance = 0;
    }

    Node()
    {
        xPos = 0;
        yPos = 0;
        cost = 0;
        priority = 0;
        manhattanDistance = 0;
    }

    int getX() const
    {
        return xPos;
    }

    int getY() const
    {
        return yPos;
    }

    int getCost() const
    {
        return cost;
    }

    int getPriority() const
    {
        return priority;
    }

    void setXPos(const int &x)
    {
        xPos = x;
    }

    void setYPos(const int &y)
    {
        yPos = y;
    }

    void setPriority()
    {
        priority = cost + manhattanDistance ;
    }

    void setCost(const int &_cost)
    {
        cost += _cost;
    }

    // вычисление манхеттенского расстояния
    void computeManhattanDistance(const int & xDest, const int & yDest)
    {
        manhattanDistance = abs(xDest - xPos) + abs(yDest - yPos);
    }

    friend bool operator==(const Node & a, const Node & b)
    {
        return a.getX() == b.getX() && a.getY() == b.getY();
    }

    friend bool operator!=(const Node & a, const Node & b)
    {
        return a.getX() != b.getX() && a.getY() != b.getY();
    }

    Node &operator=(const Node &node)
    {
        if (this == &node)
        {
            return *this;
        }

        priority = node.priority;
        xPos = node.xPos;
        yPos = node.yPos;
        cost = node.cost;
        manhattanDistance = node.manhattanDistance;

        return *this;
    }

    friend bool operator < (const Node &a, const Node &b)
    {
        return a.priority < b.priority;
    }
};

#endif // NODE_H
