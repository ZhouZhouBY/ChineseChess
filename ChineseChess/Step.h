#ifndef STEP_H
#define STEP_H

#include <QObject>

class Step : public QObject
{
    Q_OBJECT
public:
    explicit Step(QObject *parent = nullptr);
    ~Step();
    
    int _moveid;  // 移动棋子的ID
    int _killid;  // 吃掉棋子的ID，悔棋时需要将其_dead属性改变
    int _rowFrom; // 起先的行
    int _colFrom; // 起先的列
    int _rowTo;   // 现在的行
    int _colTo;   // 现在的列
    
    //bool _redTurnPre;//上一步执棋方

signals:

public slots:
};

#endif // STEP_H
