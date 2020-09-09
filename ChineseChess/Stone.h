#ifndef STONE_H
#define STONE_H

#include <QRect>
#include <QPainter>


class Stone
{
public:
    Stone();
    ~Stone();

    enum TYPE{CHE, MA, XIANG, SHI, JIANG, PAO, BING};

    void init(int id);

    int _row;
    int _col;
    TYPE _type;
    bool _dead;
    bool _red;
    int _id;
    int _value;

    QString name();

    void rotate();
};

#endif // STONE_H
