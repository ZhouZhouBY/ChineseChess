#ifndef BOARD_H
#define BOARD_H

#include <QWidget>
#include <QFrame>
#include "Stone.h"
#include "Step.h"

class Board : public QWidget
{
    Q_OBJECT
public:
    explicit Board(QWidget *parent = nullptr);

    //游戏数据
    Stone _s[32];
    int _r;
    QPoint _off;
    int _selectid;
    bool _bSide;
    bool _bRedTurn; // 执棋方
    QPoint startPoint;
    QPoint endPoint;
    bool _PushKey=false;

    int _row11;
    int _col11;
    void init(bool bRedSide);

    // 画棋盘
    void paintEvent(QPaintEvent *);
    void drawPlate(QPainter& p);
    void drawPlace(QPainter& p);
    void drawInitPosition(QPainter& p);
    void drawInitPosition(QPainter& p, int row, int col);
    void drawStone(QPainter& p);
    void drawStone(QPainter& p, int id);

    // 辅助划线
    QPoint center(int row, int col);
    QPoint center(int id);
    QPoint topLeft(int row, int col);
    QPoint topLeft(int id);
    QRect cell(int row, int col);
    QRect cell(int id);

    bool getClickRowCol(QPoint pt, int& row, int& col);
    int getStoneId(int row, int col);

    // 棋子状态
    QString name(int id);
    bool red(int id);
    bool isDead(int id);
    bool sameColor(int id1, int id2);
    void killStone(int id);
    void reliveStone(int id);
    void moveStone(int moveid, int row, int col);
    virtual void  getWin(int id);

    // 走棋
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *);
    void click(QPoint pt);
    virtual void click(int id, int row, int col);
    void trySelectStone(int id);
    void tryMoveStone(int killid, int row, int col);
    void moveStone(int moveid, int killid, int row, int col);
    void saveStep(int moveid, int killid, int row, int col, QVector<Step*>& steps);

    // 规则判断
    bool canSelect(int id);
    bool canMove(int moveid, int killid, int row, int col);
    bool canMoveChe(int moveid, int killid, int row, int col);
    bool canMoveMa(int moveid, int killid, int row, int col);
    bool canMovePao(int moveid, int killid, int row, int col);
    bool canMoveBing(int moveid, int killid, int row, int col);
    bool canMoveJiang(int moveid, int killid, int row, int col);
    bool canMoveShi(int moveid, int killid, int row, int col);
    bool canMoveXiang(int moveid, int killid, int row, int col);

    // 规则辅助
    int getStoneCountAtLine(int row1, int col1, int row2, int col2);
    int relation(int row1, int col1, int row2, int col2);
    bool isBottomSide(int id);

    // 辅助估值函数
    int MobilityValues(int id);

    void turnColor();

    virtual void checkmate();//将军

signals:

public slots:
};

#endif // BOARD_H
