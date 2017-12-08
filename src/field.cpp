#include "field.h"

Field::Field(QWidget *parent) :
    QWidget(parent)
{
    mazeWidthDimension = 20;
    mazeHeightDimension = 20;
    widthCell = 20;
    heightCell = 20;
    shiftForPoints = 2;
    lengthLi = 0;
    lengthAStar = 0;

    xPosStart = shiftForPoints;
    yPosStart = shiftForPoints + (heightCell * mazeHeightDimension) / 2;

    xPosFinish = (widthCell * mazeWidthDimension) / 2 -
                (mazeWidthDimension - shiftForPoints) + 40;

    yPosFinish = (heightCell * mazeHeightDimension) / 2 -
                (mazeHeightDimension - shiftForPoints) + 20;

    pathLiFound = false;
    pathAStarFound = false;

    xStart =(xPosStart- xPosStart % heightCell ) / heightCell;
    yStart = ( yPosStart- yPosStart % widthCell ) / widthCell;
    xFinish =(xPosFinish- xPosFinish % heightCell ) / heightCell;
    yFinish = ( yPosFinish- yPosFinish % widthCell ) / widthCell;

    cell = new Cell**[mazeWidthDimension];
    for (int i = 0; i < mazeWidthDimension; i++)
    {
        cell[i] = new Cell*[mazeHeightDimension];
    }

    dWF = new int*[mazeWidthDimension];
    for (int i = 0; i < mazeWidthDimension; ++i)
    {
        dWF[i] = new int[mazeHeightDimension];
    }

    wall = new bool*[mazeWidthDimension];
    for (int i = 0; i < mazeWidthDimension; ++i)
    {
        wall[i] = new bool [mazeHeightDimension];
    }

    for (int i = 0; i < mazeWidthDimension; i++)
    {
        for (int j = 0; j < mazeHeightDimension; j++)
        {
            cell[i][j] = new Cell(false, false, false, false);
        }
    }

    for (int i = 0; i < mazeWidthDimension; ++i)
    {
        for (int j = 0; j < mazeHeightDimension; ++j)
        {
            wall[i][j] = false;
        }
    }
}

Field::~Field()
{
    for (int i = 0; i < mazeWidthDimension; i++)
    {
        for (int j = 0; j < mazeHeightDimension; j++)
        {
            delete cell[i][j];
        }
    }

    for (int i = 0; i < mazeWidthDimension; i++)
    {
        delete cell[i];
    }
    delete [] cell;

    for (int i = 0; i < mazeWidthDimension; ++i)
    {
        delete [] dWF[i];
    }
    delete [] dWF;

    for (int i = 0; i < mazeWidthDimension; ++i)
    {
        delete [] wall[i];
    }
    delete [] wall;
}

void Field::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    //рисуем прямоугольник
    painter.setBrush(QBrush(Qt::white));
    painter.drawRect(0, 0, this->size().width(), this->size().height());

    // рисуем ячейки
    painter.setPen(QPen(Qt::black));
    for (int i = 0; i < mazeHeightDimension; i++)
    {
        for (int j = 0; j < mazeWidthDimension; j++)
        {

                painter.drawLine(j * widthCell,  i * heightCell,
                                 j * widthCell,  (i + 1) * heightCell);

                painter.drawLine(j * widthCell,  i * heightCell,
                                 (j + 1) * widthCell, i * heightCell);

                painter.drawLine((j + 1) * widthCell,  i * heightCell,
                                 (j + 1) * widthCell, (i + 1) * heightCell);

                painter.drawLine(j * widthCell,  (i + 1) * heightCell,
                                 (j + 1) * widthCell, (i + 1) * heightCell);
        }
    }

    // рисуем стенки
    painter.setPen(QPen(Qt::black));
    painter.setBrush(Qt::gray);
    for (int i = 0; i < mazeWidthDimension; i++)
    {
        for (int j = 0; j < mazeHeightDimension; j++)
        {
            if (wall[i][j] == true)
            {
                painter.drawRect(j * widthCell, i * heightCell, widthCell, heightCell);
            }
        }
    }

    // если есть путь между точками
    if (pathLiFound || pathAStarFound)
    {
        painter.setPen(QPen(Qt::black));


        for (int i = 0; i < mazeWidthDimension; ++i)
        {
            for (int j = 0; j < mazeHeightDimension; ++j)
            {
                if (dWF[i][j] == -3)
                {
                    ++lengthLi;
                    painter.setBrush(QBrush(QColor(248, 219, 64)));
                    painter.drawRect(j * widthCell + 4, i * heightCell + 4, 11, 11);

                }
                else if (dWF[i][j] == -5)
                {
                    ++lengthAStar;
                    painter.setBrush(QBrush(QColor(00, 191, 255)));
                    painter.drawRect(j * widthCell + 4, i * heightCell + 4, 11, 11);
                }
            }
        }


    }

    painter.setPen(QPen(Qt::black));
    painter.setBrush(QBrush(QColor(156, 9, 50)));
    painter.drawEllipse(xPosStart, yPosStart, 15, 15);

    painter.setPen(QPen(Qt::black));
    painter.setBrush(QBrush(QColor(66, 208, 10)));
    painter.drawEllipse(xPosFinish, yPosFinish, 15, 15);
}

void Field::mousePressEvent(QMouseEvent *e)
{
    int xPos = 0, yPos = 0;
    if (e->button() == 1) // лкм
    {
        xPos = ( e->pos().x() - e->pos().x() % heightCell ) / heightCell;
        yPos = ( e->pos().y() - e->pos().y() % widthCell ) / widthCell;

        wall[yPos][xPos] = true;

        // проверки для клеток, чтобы не выйти за границы массива

        // 0 x 0
        if (xPos == 0 && yPos == 0)
        {
           cell[yPos + 1][xPos]->setTopEdge(true);
           cell[yPos][xPos + 1]->setLeftEdge(true);
        }

        // n x 0
        else if (xPos == mazeWidthDimension - 1 && yPos == 0)
        {
           cell[yPos + 1][xPos - 1]->setTopEdge(true);
           cell[yPos][xPos - 1]->setRightEdge(true);
        }

        // 0 x n
        else if ( xPos == 0 && yPos == mazeHeightDimension - 1)
        {
           cell[yPos - 1][xPos]->setTopEdge(true);
           cell[yPos - 1][xPos +1]->setLeftEdge(true);
        }

        // n x n
        else if (xPos == mazeWidthDimension - 1 &&
                 yPos == mazeHeightDimension - 1)
        {
           cell[yPos - 1][xPos]->setBottomEdge(true);
           cell[yPos][xPos - 1]->setRightEdge(true);
        }

        // левая стенка
        else if (xPos == 0 && (yPos != 0 && yPos != mazeHeightDimension - 1))
        {
            cell[yPos - 1][xPos]->setBottomEdge(true);
            cell[yPos][xPos + 1]->setLeftEdge(true);
            cell[yPos + 1][xPos]->setTopEdge(true);
        }

        // правая стенка
        else if ( (yPos != 0 && yPos != mazeHeightDimension - 1) &&
                  xPos == mazeWidthDimension - 1)
        {
            cell[yPos][xPos - 1]->setRightEdge(true);
            cell[yPos - 1][xPos]->setBottomEdge(true);
            cell[yPos + 1][xPos]->setTopEdge(true);
        }

        // верхняя стенка
        else if (yPos == 0 &&
                 (xPos != 0 && xPos != mazeWidthDimension - 1))
        {
            cell[yPos + 1][xPos]->setTopEdge(true);
            cell[yPos][xPos - 1]->setRightEdge(true);
            cell[yPos][xPos + 1]->setLeftEdge(true);
        }

        // нижняя стенка
        else if (yPos == mazeHeightDimension - 1 &&
                 (xPos != 0 && xPos != mazeWidthDimension - 1))
        {
            cell[yPos - 1][xPos]->setBottomEdge(true);
            cell[yPos][xPos - 1]->setRightEdge(true);
            cell[yPos][xPos + 1]->setLeftEdge(true);
        }

        // все клетки в границах поля
        else if (yPos > 0 && yPos < mazeHeightDimension - 1 &&
            xPos > 0 && xPos < mazeWidthDimension - 1)
        {
            cell[yPos][xPos + 1]->setLeftEdge(true);
            cell[yPos][xPos - 1]->setRightEdge(true);
            cell[yPos + 1][xPos]->setTopEdge(true);
            cell[yPos - 1][xPos]->setBottomEdge(true);
        }
    }

    else if (e->button() == 2) // пкм
    {
        xPos = ( e->pos().x() - e->pos().x() % heightCell ) / heightCell;
        yPos =( e->pos().y() - e->pos().y() % widthCell ) / widthCell;

        wall[yPos][xPos] = false;

        // 0 x 0
        if (xPos == 0 && yPos == 0)
        {
           cell[yPos + 1][xPos]->setTopEdge(false);
           cell[yPos][xPos + 1]->setLeftEdge(false);
        }

        // n x 0
        else if (xPos == mazeWidthDimension - 1 && yPos == 0)
        {
           cell[yPos + 1][xPos - 1]->setTopEdge(false);
           cell[yPos][xPos - 1]->setRightEdge(false);
        }

        // 0 x n
        else if ( xPos == 0 && yPos == mazeHeightDimension - 1)
        {
           cell[yPos - 1][xPos]->setTopEdge(false);
           cell[yPos - 1][xPos +1]->setLeftEdge(false);
        }

        // n x n
        else if (xPos == mazeWidthDimension - 1 &&
                 yPos == mazeHeightDimension - 1)
        {
           cell[yPos - 1][xPos]->setBottomEdge(false);
           cell[yPos][xPos - 1]->setRightEdge(false);
        }

        // левая стенка
        else if (xPos == 0 && (yPos != 0 && yPos != mazeHeightDimension - 1))
        {
            cell[yPos - 1][xPos]->setBottomEdge(false);
            cell[yPos][xPos + 1]->setLeftEdge(false);
            cell[yPos + 1][xPos]->setTopEdge(false);
        }

        // правая стенка
        else if ( (yPos != 0 && yPos != mazeHeightDimension - 1) &&
                  xPos == mazeWidthDimension - 1)
        {
            cell[yPos][xPos - 1]->setRightEdge(false);
            cell[yPos - 1][xPos]->setBottomEdge(false);
            cell[yPos + 1][xPos]->setTopEdge(false);
        }

        // верхняя стенка
        else if (yPos == 0 &&
                 (xPos != 0 && xPos != mazeWidthDimension - 1))
        {
            cell[yPos + 1][xPos]->setTopEdge(false);
            cell[yPos][xPos - 1]->setRightEdge(false);
            cell[yPos][xPos + 1]->setLeftEdge(false);
        }

        // нижняя стенка
        else if (yPos == mazeHeightDimension - 1 &&
                 (xPos != 0 && xPos != mazeWidthDimension - 1))
        {
            cell[yPos - 1][xPos]->setBottomEdge(false);
            cell[yPos][xPos - 1]->setRightEdge(false);
            cell[yPos][xPos + 1]->setLeftEdge(false);
        }

        // все клетки в границах поля
        else
        {
            cell[yPos][xPos + 1]->setLeftEdge(false);
            cell[yPos][xPos - 1]->setRightEdge(false);
            cell[yPos + 1][xPos]->setTopEdge(false);
            cell[yPos - 1][xPos]->setBottomEdge(false);
        }
    }
    repaint();
}

bool contains(const std::list<Node*> &list, const Node &key)
{
    std::list<Node*>::const_iterator it = list.begin();

    for (it; it != list.end(); ++it)
    {
        if (*(*it) == key)
        {
            return true;
        }
    }

    return false;
}

bool compare(Node *a, Node *b)
{
    return a->getPriority() < b->getPriority();
}

std::list<Node*> Field::getNeighbours(const Node *node)
{
    std::list<Node*> result;
    std::vector<Node*> neighbourNode;

    neighbourNode.push_back(new Node(node->getX() + 1, node->getY() , node->getCost()));
    neighbourNode.push_back(new Node(node->getX() - 1, node->getY() , node->getCost()));
    neighbourNode.push_back(new Node(node->getX(), node->getY() + 1 , node->getCost()));
    neighbourNode.push_back(new Node(node->getX(), node->getY() - 1 , node->getCost()));

    for (int i = 0; i < 4; ++i)
    {

        if (neighbourNode[i]->getY() < 0 || neighbourNode[i]->getY() > mazeHeightDimension - 1)
        {
            continue;
        }

       if (neighbourNode[i]->getX() < 0 || neighbourNode[i]->getX() > mazeWidthDimension - 1)
       {
           continue;
       }


       if (wall[neighbourNode[i]->getY()][neighbourNode[i]->getX()] == true)
       {
           continue;
       }

       result.push_back(neighbourNode[i]);
    }

    return result;
}

void Field::clearDWF()
{
    for (int i = 0; i < mazeWidthDimension; ++i)
    {
        for (int j = 0; j < mazeHeightDimension; ++j)
        {
            dWF[i][j] = 0;
        }
    }
}

int Field::width() const
{
    return mazeWidthDimension * widthCell;
}

int Field::height() const
{
    return mazeHeightDimension * heightCell;
}

void Field::pathFindingLi()
{

    clearDWF();
    pathLiFound = false;

    for (int i = 0; i < mazeWidthDimension; ++i)
    {
        for (int j = 0; j < mazeHeightDimension; ++j)
        {
            dWF[i][j] = -1;
        }
    }

    dWF[ yPosStart / mazeHeightDimension ][ xPosStart / mazeWidthDimension ] = 0;
    dWF[ yPosFinish / mazeHeightDimension ][ xPosFinish / mazeWidthDimension ] = -2;

    int attribute = 0;


    while ( !pathLiFound )
    {
        for (int i = 0; i < mazeWidthDimension; ++i)
        {
            for (int j = 0; j < mazeHeightDimension; ++j)
            {
                if (dWF[i][j] == attribute)
                {
                    if (i != 0) // ячейка сверху
                    {
                        if ( !cell[i][j]->getTopEdge() )
                        {
                            if (dWF[i - 1][j] == -1)
                            {
                                dWF[i - 1][j] = attribute + 1;
                            }
                            else if (dWF[i - 1][j] == -2)
                            {
                                pathLiFound = true;
                                dWF[i - 1][j] = attribute + 1;
                            }
                        }
                    }

                    if (i != mazeHeightDimension - 1) // ячейка снизу
                    {
                        if ( !cell[i][j]->getBottomEdge() )
                        {
                            if (dWF[i + 1][j] == -1)
                            {
                                dWF[i + 1][j] = attribute + 1;
                            }
                            else if (dWF[i + 1][j] == -2)
                            {
                                pathLiFound = true;
                                dWF[i + 1][j] = attribute + 1;
                            }
                        }
                    }

                    if (j != 0) // ячейка слева
                    {
                        if ( !cell[i][j]->getLeftEdge() )
                        {
                            if (dWF[i][j - 1] == -1)
                            {
                                dWF[i][j - 1] = attribute + 1;
                            }
                            else if (dWF[i][j - 1] == -2)
                            {
                                pathLiFound = true;
                                dWF[i][j - 1] = attribute + 1;
                            }
                        }
                    }

                    if (j != mazeWidthDimension - 1) // ячейка справа
                    {
                        if ( !cell[i][j]->getRightEdge() )
                        {
                            if (dWF[i][j + 1] == -1)
                            {
                                dWF[i][j + 1] = attribute + 1;
                            }
                            else if (dWF[i][j + 1] == -2)
                            {
                                pathLiFound = true;
                                dWF[i][j + 1] = attribute + 1;
                            }
                        }
                    }
                }
            }
        }

        ++attribute;
    }

    int i = yPosFinish / mazeHeightDimension, j = xPosFinish / mazeWidthDimension; // ибо while
    dWF[i][j] = -3;

    while (attribute != 0)
    {
        --attribute;
        if (j != 0) // ячейка слева
        {
            if ( !cell[i][j]->getLeftEdge() )
            {
                if (dWF[i][j - 1] == attribute)
                {
                    dWF[i][j - 1] = -3;
                    --j;
                    ++lengthLi;
                    continue;
                }
            }
        }

        if (j != mazeWidthDimension - 1) // ячейка справа
        {
            if ( !cell[i][j]->getRightEdge() )
            {
                if (dWF[i][j + 1] == attribute)
                {
                    dWF[i][j + 1] = -3;
                    ++j;
                    ++lengthLi;
                    continue;
                }
            }
        }

        if (i != 0) // ячейка сверху
        {
            if ( !cell[i][j]->getTopEdge() )
            {
                if (dWF[i - 1][j] == attribute)
                {
                    dWF[i - 1][j] = -3;
                    --i;
                    ++lengthLi;
                    continue;
                }
            }
        }

        if (i != mazeHeightDimension - 1) // ячейка снизу
        {
            if ( !cell[i][j]->getBottomEdge() )
            {
                if (dWF[i + 1][j] == attribute)
                {
                    dWF[i + 1][j] = -3;
                    ++i;
                    ++lengthLi;
                    continue;
                }
            }
        }
    }

    repaint();
}

std::list<Node*> Field::buildingPath(std::map<Node*, Node* > cameFrom,
                                     Node *currentNode)
{
    std::list<Node*> builtPath(1, currentNode);

    if (cameFrom[currentNode] == 0)
    {
        return builtPath;
    }

    builtPath = buildingPath(cameFrom, cameFrom[currentNode]);
    builtPath.push_back(currentNode);
    return builtPath;
}





void Field::pathFindingAStar()
{
    clearDWF();
    pathAStarFound = false;
    std::list<Node*> closed;

    for (int i = 0; i < mazeHeightDimension; ++i) {
        for (int j = 0; j < mazeWidthDimension; ++j) {
            if (wall[i][j] == true)
            {
                closed.push_front(new Node(j,i,0));
            }
        }
    }

    Node *start = new Node(xStart, yStart, 0);
    Node *finish = new Node(xFinish, yFinish, 1);

    std::list<Node*> opened;
    opened.push_front(start);

    start->computeManhattanDistance(xFinish, yFinish);
    start->setPriority();

    std::map<Node*, Node* > cameFrom; // ключ - родитель : значение - сын
    std::list<Node*> neighbourList; // список соседей клетки dir = 4

    Node *currentPoint = new Node(start->getX(), start->getY(), start->getCost());

    while (!opened.empty())
    {
        // ищем в открытом списке клетку с наименьшей стоимостью F
        currentPoint = opened.front();

        for (std::list<Node*>::iterator it = opened.begin(); it != opened.end(); ++it )
        {
            if (currentPoint->getPriority() > (*it)->getPriority())
            {
                currentPoint = *it;
            }
        }

        if (*currentPoint == *finish)
        {
            std::list<Node*> path = buildingPath(cameFrom, currentPoint);
            std::list<Node*>::const_iterator it = path.begin();
            for (it; it != path.end(); ++it)
            {
//                std::cout << (*it)->getX() << ", " << (*it)->getY() <<std::endl;
                dWF[(*it)->getY()][(*it)->getX()] = -5;
                ++lengthAStar;
            }
            path.clear();
            pathAStarFound = true;
        }

        closed.push_front(currentPoint);
        opened.remove(currentPoint);

        neighbourList = getNeighbours(currentPoint);
        std::list<Node*>::iterator neighbour = neighbourList.begin();

        for (neighbour; neighbour != neighbourList.end(); ++neighbour)
        {
            // если узел находится в закрытом списке, игнорим его
            if (contains(closed, **neighbour))
            {
                continue;
            }

            // если узел еще не в открытом списке, добавляем его туда
            // делаем текущую родительской и считаем функцию
            else if (!contains(opened, **neighbour))
            {
                cameFrom[*neighbour] = currentPoint;
                (*neighbour)->computeManhattanDistance(xFinish, yFinish);
                (*neighbour)->setCost(1);
                (*neighbour)->setPriority();
                opened.push_front(*neighbour);
            }

            // если клетка уже находится в открытом списке, то
            // проверяем, не дешевле ли будет путь через эту клетку(через стоимость G)
            else if (contains(opened, **neighbour))
            {
                if ((*neighbour)->getCost() * 2  < currentPoint->getCost())
                {
                    cameFrom[*neighbour] = currentPoint;
                    (*neighbour)->computeManhattanDistance(xFinish, yFinish);
                    (*neighbour)->setCost(1);
                    (*neighbour)->setPriority();
                }
            }
        }
        opened.sort(compare);
    }

    std::cout << lengthAStar << " ";

    opened.clear();
    closed.clear();
    cameFrom.clear();
    neighbourList.clear();
    delete currentPoint;
    delete start;
    delete finish;

    repaint();
}

void Field::clearCells()
{
    int xPos = 0, yPos = 0;

    for (yPos = 0; yPos < mazeHeightDimension; ++yPos)
    {
        for (xPos = 0; xPos < mazeWidthDimension; ++xPos)
        {
            // 0 x 0
            if (xPos == 0 && yPos == 0)
            {
               cell[yPos + 1][xPos]->setTopEdge(false);
               cell[yPos][xPos + 1]->setLeftEdge(false);
            }

            // n x 0
            else if (xPos == mazeWidthDimension - 1 && yPos == 0)
            {
               cell[yPos + 1][xPos - 1]->setTopEdge(false);
               cell[yPos][xPos - 1]->setRightEdge(false);
            }

            // 0 x n
            else if ( xPos == 0 && yPos == mazeHeightDimension - 1)
            {
               cell[yPos - 1][xPos]->setTopEdge(false);
               cell[yPos - 1][xPos +1]->setLeftEdge(false);
            }

            // n x n
            else if (xPos == mazeWidthDimension - 1 &&
                     yPos == mazeHeightDimension - 1)
            {
               cell[yPos - 1][xPos]->setBottomEdge(false);
               cell[yPos][xPos - 1]->setRightEdge(false);
            }

            // левая стенка
            else if (xPos == 0 && (yPos != 0 && yPos != mazeHeightDimension - 1))
            {
                cell[yPos - 1][xPos]->setBottomEdge(false);
                cell[yPos][xPos + 1]->setLeftEdge(false);
                cell[yPos + 1][xPos]->setTopEdge(false);
            }

            // правая стенка
            else if ( (yPos != 0 && yPos != mazeHeightDimension - 1) &&
                      xPos == mazeWidthDimension - 1)
            {
                cell[yPos][xPos - 1]->setRightEdge(false);
                cell[yPos - 1][xPos]->setBottomEdge(false);
                cell[yPos + 1][xPos]->setTopEdge(false);
            }

            // верхняя стенка
            else if (yPos == 0 &&
                     (xPos != 0 && xPos != mazeWidthDimension - 1))
            {
                cell[yPos + 1][xPos]->setTopEdge(false);
                cell[yPos][xPos - 1]->setRightEdge(false);
                cell[yPos][xPos + 1]->setLeftEdge(false);
            }

            // нижняя стенка
            else if (yPos == mazeHeightDimension - 1 &&
                     (xPos != 0 && xPos != mazeWidthDimension - 1))
            {
                cell[yPos - 1][xPos]->setBottomEdge(false);
                cell[yPos][xPos - 1]->setRightEdge(false);
                cell[yPos][xPos + 1]->setLeftEdge(false);
            }

            // все клетки в границах поля
            else
            {
                cell[yPos][xPos + 1]->setLeftEdge(false);
                cell[yPos][xPos - 1]->setRightEdge(false);
                cell[yPos + 1][xPos]->setTopEdge(false);
                cell[yPos - 1][xPos]->setBottomEdge(false);
            }
        }
    }
}

void Field::clearPath()
{
    clearDWF();
    emit signalAStar();
    emit signalLi();
    lengthAStar = 0;
    lengthLi = 0;
    repaint();
}

void Field::clearWalls()
{
    for (int i = 0; i < mazeWidthDimension; ++i)
    {
        for (int j = 0; j < mazeHeightDimension; ++j)
        {
            wall[i][j] = false;
        }
    }

    clearCells();
    repaint();
}

void Field::clearAll()
{
    clearPath();
    clearWalls();
}




