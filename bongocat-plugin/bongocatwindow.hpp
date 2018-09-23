#ifndef BONGOCATWINDOW_H
#define BONGOCATWINDOW_H

#include "../include/qmpcorepublic.hpp"
#include <QtWidgets/QMainWindow>

enum BongoState {
    AllHandsDown,
    AllHandsUp,
    LeftHandDown,
    RightHandDown
};

class BongoCatWindow : public QMainWindow, public ICallBack
{
    Q_OBJECT
public:
    explicit BongoCatWindow(QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
    ~BongoCatWindow() override;

    void voiceChanged(uint count);

    // ICallBack interface
    void callBack(void *callerdata, void *) override;

protected:
    void paintEvent(QPaintEvent *) override;
    void keyPressEvent(QKeyEvent *event) override;

    QPixmap bongo;
    QPixmap l1, r1, l2, r2, m1, m2;
    BongoState state = AllHandsUp;
    bool isLastLeftHand = false;
    uint curPolyCnt = 0;
};


#endif // BONGOCATWINDOW_H
