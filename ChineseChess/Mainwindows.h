#ifndef MAINWINDOWS_H
#define MAINWINDOWS_H

#include <QDialog>
#include "Choosedlg.h"
#include <QWidget>
#include "Board.h"
#include "Netgame.h"
#include "Singlegame.h"

class MainWindows : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindows(int gameType, QWidget *parent = nullptr);

    int _gameType;

signals:

public slots:
};

#endif // MAINWINDOWS_H
