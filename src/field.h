#ifndef FIELD_H
#define FIELD_H

#include <iostream>
#include <list>
#include <map>
#include <QtDebug>
#include <QMouseEvent>
#include <QPainter>
#include <QWidget>
#include <vector>

#include "cell.h"
#include "node.h"

class Field : public QWidget
{
    Q_OBJECT

private:
    int mazeWidthDimension; // количество клеток по горизонтали
    int mazeHeightDimension; // количество клеток по вертикали
    int widthCell; // ширина клетки
    int heightCell; // высота клетки
    int  shiftForPoints;
    Cell ***cell;
    bool **wall; // массив из значений стена\ничего
    int **dWF; // дискретное рабочее поле(как в вики)

    int xPosStart, yPosStart, xPosFinish, yPosFinish; // координаты стартовой и финишной ячейки
    bool pathLiFound; // путь по алгоритму Ли найден
    bool pathAStarFound; // путь по алгоритму a* найден
    int xStart;
    int yStart;
    int xFinish;
    int yFinish;
    int lengthLi;
    int lengthAStar;

    std::list<Node*> getNeighbours(const Node *node); // поиск соседей клетки для астар
    std::list<Node *> buildingPath(std::map<Node *, Node *> cameFrom, Node *currentNode);
    void clearDWF();
    void clearCells();

public:
    explicit Field(QWidget *parent = 0);
    ~Field();
    virtual void paintEvent(QPaintEvent*);
    virtual void mousePressEvent(QMouseEvent *e);

    int width() const;
    int height() const;

    int getLentghLi() const
    {
        return lengthLi;
    }

    int getLentghAStar() const
    {
        return lengthAStar;
    }

signals:
    void signalLi();
    void signalAStar();

public slots:
    void pathFindingLi();
    void pathFindingAStar();
    void clearPath();
    void clearWalls();
    void clearAll();

};

#endif // MAZE_H
