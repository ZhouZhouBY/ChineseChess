#ifndef NETGAME_H
#define NETGAME_H

#include "Board.h"
#include <QTcpServer>
#include <QTcpSocket>

/*
    可能给对方发送的消息
    1. 执红方还是黑方， 服务器发出，客户端接收
       第一个字节固定是1， 第二个字节是1或0，  1表示接收方走红棋， 0表示接收方走黑棋
    2. 点击信息，主要是 点击的坐标点 和 点击棋子的id
       第一个字节固定是2， 第二个字节是row, 第三个字节是col, 第四个字节是点击棋子的id
*/
class NetGame : public Board
{
    Q_OBJECT
public:
    NetGame(bool _bServer);

    QTcpServer* _server;
    QTcpSocket* _socket;

    void click(int id, int row, int col);

public slots:
    void slotNewConnection();
    void slotRecv();
};

#endif // NETGAME_H
