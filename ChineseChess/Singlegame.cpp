#include "Singlegame.h"
#include "Step.h"
#include <QTimer>

SingleGame::SingleGame() {
    QPixmap pixmap = QPixmap("D:\\project\\ChineseChess\\background.jpg").scaled(this->size());
    QPalette palette(this->palette());
    palette.setBrush(QPalette::Background, QBrush(pixmap));
    this->setAutoFillBackground(true);
    this->setPalette(palette);

    _level = 3;
}

void SingleGame::click(int id, int row, int col) {
    if (!this->_bRedTurn) {
        return;
    }

    Board::click(id, row, col);

    if (!this->_bRedTurn) { // 黑棋电脑走
        //启动定时器，0.1秒后电脑在思考
        QTimer::singleShot(100, this, SLOT(computerMove()));
    }
}

void SingleGame::computerMove() {
    Step* step = getBestMove();
    moveStone(step->_moveid, step->_killid, step->_rowTo, step->_colTo);
    delete step;
    update();
}

void SingleGame::getAllPossibleMove(QVector<Step *> &steps) {
    int min = 16, max = 32;
    if (this->_bRedTurn) {
        min = 0;
        max = 16;
    }
    for (int i = min; i < max; ++i) {
        if (isDead(i)) {
            continue;
        }
        for (int row = 0; row <= 9; ++row) {
            for (int col = 0; col <= 8; ++col) {
                int killid = this->getStoneId(row, col);
                if (sameColor(killid, i)) {
                    continue;
                }
                if (canMove(i, killid, row, col)) {
                    saveStep(i, killid, row, col, steps);
                }
            }
        }
    }
}


void SingleGame::fakeMove(Step *step) {
    killStone(step->_killid);
    moveStone(step->_moveid, step->_rowTo, step->_colTo);
}

void SingleGame::unfakeMove(Step *step) {
    reliveStone(step->_killid);
    moveStone(step->_moveid, step->_rowFrom, step->_colFrom);
}

// 评价局面分
int SingleGame::calScore() {
    int redTotalScore = 0;
    int blackTotalScore = 0;

    //enum TYPE{CHE, MA, XIANG, SHI, JIANG, PAO, BING};
    static int chessScore[] = {1000, 499, 100, 100, 15000, 501, 200};

    // 黑棋分的总数 - 红棋分的总数
    for (int i = 0; i < 16; ++i) {
        if (_s[i]._dead) {
            continue;
        }
        redTotalScore += chessScore[_s[i]._type];
    }
    for (int i = 16; i < 32; ++i) {
        if (_s[i]._dead) {
            continue;
        }
        blackTotalScore += chessScore[_s[i]._type];
    }

    return blackTotalScore - redTotalScore;
}

int SingleGame::getMinScore(int level, int curMaxScore) {
    if (level == 0) {
        return calScore();
    }
    QVector<Step*> steps;
    getAllPossibleMove(steps); // 红棋的所有可能移动
    int minScore = INT_MAX;
    while (steps.count()) {
        Step* step = steps.back();
        steps.removeLast();

        fakeMove(step);
        int score = getMaxScore(level - 1, minScore);
        unfakeMove(step);
        delete step;

        if (score <= curMaxScore) { // 剪枝
            while (steps.count()) {
                Step* step = steps.back();
                steps.removeLast();
                delete step;
            }
            return score;
        }

        if (score < minScore) {
            minScore = score;
        }
    }
    return minScore;
}

int SingleGame::getMaxScore(int level, int curMinScore) {
    if (level == 0) {
        return calScore();
    }
    QVector<Step*> steps;
    getAllPossibleMove(steps); // 红棋的所有可能移动
    int maxScore = INT_MIN;
    while (steps.count()) {
        Step* step = steps.back();
        steps.removeLast();

        fakeMove(step);
        int score = getMinScore(level - 1, maxScore);
        unfakeMove(step);
        delete step;

        if (score >= curMinScore) { // 剪枝
            while (steps.count()) {
                Step* step = steps.back();
                steps.removeLast();
                delete step;
            }
        }

        if (score > maxScore) {
            maxScore = score;
        }


    }
    return maxScore;
}

Step* SingleGame::getBestMove() {
    // 1.看有哪些步骤可以走
    QVector<Step*> steps;
    getAllPossibleMove(steps);

    // 2.试着走
    // 3.评估走的结果
    int maxScore = INT_MIN;
    Step* ret = NULL;
    while (steps.count()) {
        Step* step = steps.back();
        steps.removeLast();

        fakeMove(step);
//        int score = calScore(); // 思考一步
//        int score = getMinScore(); // 思考两步
        int score = getMinScore(_level, maxScore);
        unfakeMove(step);
        if (score > maxScore) {
            maxScore = score;
            if (ret) {
                delete ret;
            }
            ret = step;
        } else {
            delete step;
        }
    }

    // 4.取最好结果作为参考
    return ret;
}
