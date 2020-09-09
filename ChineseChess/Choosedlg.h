#ifndef CHOOSEDLG_H
#define CHOOSEDLG_H

#include <QDialog>
#include <QPushButton>

class ChooseDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ChooseDlg(QWidget *parent = 0);
    ~ChooseDlg();

    QPushButton* _buttons[5];
    int _selected;
signals:

public slots:
    void slotClicked();
};

#endif // CHOOSEDLG_H
