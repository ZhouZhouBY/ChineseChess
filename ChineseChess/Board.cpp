#include "Board.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

#define GetRowCol(__row, __col, __id)\
    int __row = _s[__id]._row;\
    int __col = _s[__id]._col


Board::Board(QWidget *parent) : QWidget(parent) {
    QPixmap pixmap = QPixmap("D:\\project\\ChineseChess\\background.jpg").scaled(this->size());
    QPalette palette(this->palette());
    palette.setBrush(QPalette::Background, QBrush(pixmap));
    this->setAutoFillBackground(true);
    this->setPalette(palette);

    this->_r = 40;
    setMinimumSize(_r*20, _r*20+40);
    setMaximumSize(_r*30, _r*20+40);
    init(true);
}

void Board::init(bool bRedSide)
{
    for(int i=0; i<32; ++i)
    {
        _s[i].init(i);
    }
    if(bRedSide)
    {
        for(int i=0; i<32; ++i)
        {
            _s[i].rotate();
        }
    }

    _selectid = -1;
    _bRedTurn = true;
    _bSide = bRedSide;
    startPoint=center(-1,-1);
    endPoint=center(-1, -1);
    update();
}

void Board::drawPlate(QPainter &p) {
    for (int i = 0; i < 10; ++i) { //行
        if (i == 0 || i == 9) {
            p.setPen(QPen(Qt::black, 3, Qt::SolidLine));
        } else {
            p.setPen(QPen(Qt::black, 1, Qt::SolidLine));
        }
        p.drawLine(center(i, 0), center(i, 8));
    }
    for (int i = 0; i < 9; ++i) { //列
        if (i == 0 || i == 8) {
            p.setPen(QPen(Qt::black, 3, Qt::SolidLine));
            p.drawLine(center(0, i), center(9, i));
        } else {
            p.setPen(QPen(Qt::black, 1, Qt::SolidLine));
            p.drawLine(center(0, i), center(4, i));
            p.drawLine(center(5, i), center(9, i));
        }
    }

    int d = 2 * _r;
    QRect rect(d / 2, 5 * d - _r, 8 * d, d); // QRect(int left, int right, int width, int height)
    p.setFont(QFont("Times", d / 2, QFont::Bold)); // 设置字体

    // 楚河汉界
    QFontMetrics metrics = p.fontMetrics();
    int stringHeight = metrics.ascent() + metrics.descent(); // 不算line gap
    int stringWidth = metrics.width("楚 河 汉 界"); // 字符串的宽度
    int x = rect.x() + (rect.width() - stringWidth) / 2;
    int y = rect.y() + (rect.height() - stringHeight) / 2 + metrics.ascent();
    p.drawText(x, y, "楚 河 汉 界");
}

// 画九宫格
void Board::drawPlace(QPainter &p) {
    // 上九宫格
    p.drawLine(center(0, 3), center(2, 5));
    p.drawLine(center(2, 3), center(0, 5));

    //下九宫格
    p.drawLine(center(9, 3), center(7, 5));
    p.drawLine(center(7, 3), center(9, 5));
}

// 画角
void Board::drawInitPosition(QPainter &p, int row, int col) {
    QPoint pt = center(row, col);
    QPoint off = QPoint(_r / 6, _r / 6);
    int len = _r / 3;

    QPoint ptStart;
    QPoint ptEnd;

    if (col != 0) {
        // 左上角
        ptStart = QPoint(pt.x() - off.x(), pt.y() - off.y());
        ptEnd = ptStart + QPoint(-len, 0);
        p.drawLine(ptStart, ptEnd);
        ptEnd = ptStart + QPoint(0, -len);
        p.drawLine(ptStart, ptEnd);

        // 左下角
        ptStart = QPoint(pt.x() - off.x(), pt.y() + off.y());
        ptEnd = ptStart + QPoint(-len, 0);
        p.drawLine(ptStart, ptEnd);
        ptEnd = ptStart + QPoint(0, +len);
        p.drawLine(ptStart, ptEnd);
    }

    if(col != 8)
    {
        // 右下角
        ptStart = QPoint(pt.x() + off.x(), pt.y() + off.y());
        ptEnd = ptStart + QPoint(+len, 0);
        p.drawLine(ptStart, ptEnd);
        ptEnd = ptStart + QPoint(0, +len);
        p.drawLine(ptStart, ptEnd);

        // 右上角
        ptStart = QPoint(pt.x() + off.x(), pt.y() - off.y());
        ptEnd = ptStart + QPoint(+len, 0);
        p.drawLine(ptStart, ptEnd);
        ptEnd = ptStart + QPoint(0, -len);
        p.drawLine(ptStart, ptEnd);
    }
}

void Board::drawInitPosition(QPainter &p) {
    drawInitPosition(p, 3, 0);
    drawInitPosition(p, 3, 2);
    drawInitPosition(p, 3, 4);
    drawInitPosition(p, 3, 6);
    drawInitPosition(p, 3, 8);

    drawInitPosition(p, 6, 0);
    drawInitPosition(p, 6, 2);
    drawInitPosition(p, 6, 4);
    drawInitPosition(p, 6, 6);
    drawInitPosition(p, 6, 8);

    drawInitPosition(p, 2, 1);
    drawInitPosition(p, 2, 7);

    drawInitPosition(p, 7, 1);
    drawInitPosition(p, 7, 7);
}

// 画棋子
void Board::drawStone(QPainter &p, int id) {
    if (isDead(id)) return;
    int d = 2 * _r;
    QColor color;
  if (red(id)) {
        color = Qt::red;
    } else {
        color = Qt::black;
    }
    p.setFont(QFont("Times", d / 2, QFont::Bold));
    p.setPen(QPen(QBrush(color), 2));

    if (id == _selectid) {
        p.setBrush(Qt::green);
    } else {
        p.setBrush(Qt::yellow);
    }

    p.drawEllipse(cell(id));
    p.drawText(cell(id), name(id), QTextOption(Qt::AlignCenter));

    p.setRenderHint(QPainter::Antialiasing);
}

void Board::drawStone(QPainter &p) {
    for(int i=0; i<32; i++)
    {
        drawStone(p, i);
    }
}

void Board::paintEvent(QPaintEvent *) {
    int r = height() / 20;
    _r = r;
    _off = QPoint(r + 1, r + 1);

    QPainter p(this);
    p.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    p.save();
    drawPlate(p);
    p.restore();

    p.save();
    drawPlace(p);
    p.restore();

    p.save();
    drawInitPosition(p);
    p.restore();


    p.save();
    drawStone(p);
    p.restore();

//    drawArrow(p);

}

QPoint Board::center(int row, int col) {
    QPoint pt(_r * col * 2, _r * row * 2);
    return pt + _off;
}

QPoint Board::center(int id) {
    return center(_s[id]._row, _s[id]._col);
}

bool Board::isDead(int id) {
    if (id == -1) {
        return true;
    }
    return _s[id]._dead;
}

bool Board::red(int id) {
    return _s[id]._red;
}

QRect Board::cell(int id) {
    return QRect(topLeft(id), QSize(_r * 2 - 1, _r * 2 - 1));
}

QRect Board::cell(int row, int col) {
    return QRect(topLeft(row, col), QSize(_r * 2 - 1, _r * 2 - 1));
}

QString Board::name(int id) {
    return _s[id].name();
}

QPoint Board::topLeft(int row, int col) {
    return center(row, col) - QPoint(_r, _r);
}

QPoint Board::topLeft(int id) {
    return center(id) - QPoint(_r, _r);
}

bool Board::getClickRowCol(QPoint pt, int &row, int &col) {
    for (row = 0; row <= 9; ++row) {
        for (col = 0; col <= 9; ++col) {
            QPoint distance = center(row, col) - pt;
            if (distance.x() * distance.x() + distance.y() * distance.y() < _r * _r) {
                return true;
            }
        }
    }
    return false;
}

int Board::getStoneId(int row, int col) {
    for (int i = 0; i < 32; ++i) {
        if (_s[i]._row == row && _s[i]._col == col && !isDead(i)) {
            return i;
        }
    }
    return -1;
}

void Board::mousePressEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        click(event->pos());
        if (_selectid > -1) {
            _row11 = _s[_selectid]._row;
            _col11 = _s[_selectid]._col;
        }
    }
}

void Board::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        if (_selectid == -1) {
            return;
        } else {
            int x, y;
            getClickRowCol(event->pos(), x, y);
            _s[_selectid]._row = x;
            _s[_selectid]._col = y;
        }
        update();
    }
}

void Board::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        if (_selectid > -1) {
            _s[_selectid]._row = _row11;
            _s[_selectid]._col = _col11;
            click(event->pos());
        }
        update();
    }
}

void Board::click(QPoint pt) {
    int row, col;
    bool bClicked = getClickRowCol(pt, row, col);
    if (!bClicked) {
        return;
    }
    int id = getStoneId(row, col);
    click(id, row, col);
}

void Board::click(int id, int row, int col) {
//    qDebug() << "click2";
    if (this->_selectid == -1) {
        trySelectStone(id);
    } else {
        if (_s[_selectid]._value < 100) {
            _s[_selectid]._value = MobilityValues(_selectid);
        }
        tryMoveStone(id, row, col);
        checkmate(); // 将军
    }
    getWin(id);
}

void Board::tryMoveStone(int killid, int row, int col) {
//    qDebug() << "tryMoveStone";
    if (killid != -1 && sameColor(killid, _selectid)) {
        trySelectStone(killid);
        return;
    }

    bool ret = canMove(_selectid, killid, row, col);
    if (ret) {
//        if (!_PushKey) {
//            if (_bSide) {
//                informationShow(_selectid, row, col);
//            } else {
//                informationShowInver(_selectid, row, col);
//            }
//        }
        startPoint = center(_selectid);
        endPoint = center(_selectid);
        moveStone(_selectid, killid, row, col);

        _selectid = -1;
        update();
    }
}


void Board::trySelectStone(int id) {
//    qDebug() << "trySelectStone";
    if (id == -1) {
        return;
    }

    if (!canSelect(id)) {
        return;
    }

    _selectid = id;
    update();
}

bool Board::canSelect(int id) {
    return _bRedTurn == _s[id]._red;
}

int Board:: MobilityValues(int id)
{
//    6,  12,  6,  15, 0,  1,   1
    switch (_s[id]._type)
    {
    case Stone::CHE:
        return 6;

    case Stone::MA:
        return 12;

    case Stone::PAO:
        return 6;

    case Stone::BING:
        return 15;

    case Stone::JIANG:
        return 0;

    case Stone::SHI:
        return 1;

    case Stone::XIANG:
        return 1;

    }
    return 0;
}

void Board::checkmate() {
//    qDebug() << "checkmate";
}

void Board::getWin(int id) {
    if (id == -1) {
        return;
    }
    if (_s[id]._type == Stone::JIANG && _s[id]._dead == true) {
        if (_s[id]._red == _bRedTurn) {
            qDebug() << "red is win!";
        } else {
            qDebug() << "black is win!";
        }
        Board::init(_bSide);
        update();
    }
//    if (id==-1)
//        return;
//    if(_s[id]._type==Stone::JIANG&&_s[id]._dead==true)
//    {
//            QMessageBox::StandardButton ret;
//            if(_s[id]._red==_bRedTurn){


//                ret=QMessageBox::question(NULL,"GameOver","DEFEATED /n 点击进入下一局比赛");

//                if(ret==QMessageBox::Yes)
//                {
//                    Board:: init(_bSide);


//                }
//                else {
//                    Board:: init(_bSide);

//                }
//            }
//            else {
//                ret=QMessageBox::question(NULL,"GameOver","YOU WIN \n 点击进入下一局比赛");

//                if(ret==QMessageBox::Yes)
//                {
//                    Board:: init(_bSide);
//                }
//                else {
//                    Board:: init(_bSide);

//                }
//            }
//            _back->setStyleSheet("background-color:red;color:white");
//            this->_steps.clear();
//            this->_plte->clear();
//           update();
//    }
}

bool Board::sameColor(int id1, int id2) {
    if (id1 == -1 || id2 == -1) {
        return false;
    }
    return red(id1) == red(id2);
}

bool Board::canMove(int moveid, int killid, int row, int col) {
    if(sameColor(moveid, killid)) {
        return false;
    }

    switch (_s[moveid]._type)
    {
    case Stone::CHE:
        return canMoveChe(moveid, killid, row, col);

    case Stone::MA:
        return canMoveMa(moveid, killid, row, col);

    case Stone::PAO:
        return canMovePao(moveid, killid, row, col);

    case Stone::BING:
        return canMoveBing(moveid, killid, row, col);

    case Stone::JIANG:
        return canMoveJiang(moveid, killid, row, col);

    case Stone::SHI:
        return canMoveShi(moveid, killid, row, col);

    case Stone::XIANG:
        return canMoveXiang(moveid, killid, row, col);

    }
}

bool Board::canMoveChe(int moveid, int, int row, int col) {
    GetRowCol(row1, col1, moveid);
    int ret = getStoneCountAtLine(row1, col1, row, col);
    return ret == 0;
}

bool Board::canMoveMa(int moveid, int, int row, int col) {
    GetRowCol(row1, col1, moveid);
    int r = relation(row1, col1, row, col);
    if (r != 12 && r != 21) {
        return false;
    }
    if (r == 12) {
        if (getStoneId(row1, (col + col1) / 2) != -1) {
            return false;
        }
    } else {
        if (getStoneId((row + row1) / 2, col1) != -1) {
            return false;
        }
    }

    return true;
}

bool Board::canMoveXiang(int moveid, int, int row, int col) {
    GetRowCol(row1, col1, moveid);
    int r = relation(row1, col1, row, col);
    if (r != 22) {
        return false;
    }

    int rEye = (row + row1) / 2;
    int cEye = (col + col1) / 2;
    if (getStoneId(rEye, cEye) != -1) {
        return false;
    }
    
    if (isBottomSide(moveid)) {
        if (row < 4) {
            return false;
        }
    } else {
        if (row > 5) {
            return false;
        }
    }

    return true;
}

bool Board::canMoveShi(int moveid, int, int row, int col) {
    GetRowCol(row1, col1, moveid);
    int r = relation(row1, col1, row, col);
    if (r != 11) {
        return false;
    }

    if (col < 3 || col > 5) {
        return false;
    }
    if (isBottomSide(moveid)) {
        if (row < 7) {
            return false;
        }
    } else {
        if (row > 2) {
            return false;
        }
    }

    return true;
}

bool Board::canMoveJiang(int moveid, int killid, int row, int col) {
    if (killid != -1 && _s[killid]._type == Stone::JIANG) {
        return canMoveChe(moveid, killid, row, col);
    }
    GetRowCol(row1, col1, moveid);
    int r = relation(row1, col1, row, col);
    if (r != 1 && r != 10) {
        return false;
    }

    if (col < 3 || col > 5) {
        return false;
    }
    if (isBottomSide(moveid)) {
        if (row < 7) {
            return false;
        }
    } else {
        if (row > 2) {
            return false;
        }
    }

    return true;
}

bool Board::canMovePao(int moveid, int killid, int row, int col) {
    GetRowCol(row1, col1, moveid);
    int ret = getStoneCountAtLine(row, col, row1, col1);
    if (killid != -1) {
        return ret == 1;
    } else {
        return ret == 0;
    }
}

bool Board::canMoveBing(int moveid, int, int row, int col) {
    GetRowCol(row1, col1, moveid);
    int r = relation(row1, col1, row, col);
    if (r != 1 && r != 10) {
        return false;
    }

    if (isBottomSide(moveid)) {
        if (row > row1) {
            return false;
        }
        if (row1 >= 5 && row == row1) {
            return false;
        }
    } else {
        if (row < row1) {
            return false;
        }
        if (row <= 4 && row == row1) {
            return false;
        }
    }

    return true;
}

void Board::moveStone(int moveid, int row, int col) {
    _s[moveid]._row = row;
    _s[moveid]._col = col;
//    turnColor();
    _bRedTurn = !_bRedTurn;
    update();
}

void Board::moveStone(int moveid, int killid, int row, int col) {
    killStone(killid);
    moveStone(moveid, row, col);
}

void Board::turnColor() { //悔棋按钮改变颜色
    if(_bRedTurn)
    {
//         _back->setStyleSheet("background-color:black;color:white");

    }else {

//        _back->setStyleSheet("background-color:red;color:white");
    }
}

void Board::killStone(int id) {
    if (id == -1) {
        return;
    }
    _s[id]._dead = true;
}

void Board::reliveStone(int id) {
    if (id == -1) {
        return;
    }
    _s[id]._dead = false;
}

int Board::getStoneCountAtLine(int row1, int col1, int row2, int col2) {
    int ret = 0;
    if (row1 != row2 && col1 != col2) {
        return -1;
    }
    if (row1 == row2 && col1 == col2) {
        return -1;
    }

    if (row1 == row2) {
        int min = col1 < col2 ? col1 : col2;
        int max = col1 > col2 ? col1 : col2;
        for (int col = min + 1; col < max; ++col) {
            if (getStoneId(row1, col) != -1) {
                ++ret;
            }
        }
    } else {
        int min = row1 < row2 ? row1 : row2;
        int max = row1 > row2 ? row1 : row2;
        for (int row = min + 1; row < max; ++row) {
            if (getStoneId(row, col1) != -1) {
                ++ret;
            }
        }
    }

    return ret;
}


int Board::relation(int row1, int col1, int row, int col) {
    return qAbs(row1 - row) * 10 + qAbs(col1 - col);
}

bool Board::isBottomSide(int id) {
    return _bSide == _s[id]._red;
}

void Board::saveStep(int moveid, int killid, int row, int col, QVector<Step *> &steps) {
    GetRowCol(row1, col1, moveid);
    Step* step = new Step;
    step->_colFrom = col1;
    step->_colTo = col;
    step->_rowFrom = row1;
    step->_rowTo = row;
    step->_moveid = moveid;
    step->_killid = killid;

    steps.append(step);
}
