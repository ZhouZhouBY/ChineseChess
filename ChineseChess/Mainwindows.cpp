#include "Mainwindows.h"

MainWindows::MainWindows(int gameType, QWidget *parent) : QWidget(parent) {
    _gameType = gameType;

    if(_gameType == 0) // 人机对战
    {
        SingleGame* game = new SingleGame;

        game->show();

    }
    else if(_gameType == 1) // 人人对战
    {
        Board* game = new Board;

        game->show();
    }
    else if(_gameType == 2) // 机机对战
    {
//        NetGame* game = new NetGame(true);

//        game->show();
    }
    else if(_gameType == 3) // 联机对战（服务端）
    {
        NetGame* game = new NetGame(true);

        game->show();
    }
    else if(_gameType == 4) // 联机对战（客户端）
    {
        NetGame* game = new NetGame(false);

        game->show();

    }
}
