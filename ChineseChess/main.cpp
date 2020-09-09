#include <QApplication>
#include "Board.h"
#include "Singlegame.h"
#include "NetGame.h"
#include <QMessageBox>
#include "Choosedlg.h"
#include "Mainwindows.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    ChooseDlg dlg;
    dlg.setWindowOpacity(0.8);

    if(dlg.exec() != QDialog::Accepted)
        return 0;

    MainWindows wnd(dlg._selected);


//    SqliteDatabase b;
//    b.show();
    return app.exec();

//    bool bServer = false;
//    QMessageBox::StandardButton ret;
//    ret = QMessageBox::question(NULL, "server or client", "作为服务器启动");
//    if (ret == QMessageBox::Yes) {
//        bServer = true;
//    }
//    NetGame board(bServer);
//    Board board;
//    board.show();
//    return app.exec();
}
