#ifndef SINGLEGAME_H
#define SINGLEGAME_H
#include "Board.h"

class SingleGame : public Board {
    Q_OBJECT
public:
    SingleGame();

    int _level;
    virtual void click(int id, int row, int col);

    Step* getBestMove();
    void getAllPossibleMove(QVector<Step*>& steps);

    void fakeMove(Step* step);
    void unfakeMove(Step* step);
    int calScore();

    int getMinScore(int level, int curMaxScore);
    int getMaxScore(int level, int curMinScore);
public slots:
    void computerMove();
};

#endif // SINGLEGAME_H
