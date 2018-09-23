#include "bongocatwindow.hpp"

#include <QPainter>
#include <QKeyEvent>


BongoCatWindow::BongoCatWindow(QWidget *parent, Qt::WindowFlags flags)
    : QMainWindow (parent, flags)
{
    bongo = QPixmap(":/bongocat/bongo.png");
    l1 = QPixmap(":/bongocat/l1.png");
    l2 = QPixmap(":/bongocat/l2.png");
    r1 = QPixmap(":/bongocat/r1.png");
    r2 = QPixmap(":/bongocat/r2.png");
    m1 = QPixmap(":/bongocat/m1.png");
    m2 = QPixmap(":/bongocat/m2.png");
    this->setStyleSheet("background: #fff");
    this->resize(800, 450);
}

BongoCatWindow::~BongoCatWindow()
{
    // :)
}

void BongoCatWindow::voiceChanged(uint count)
{
    if (count > 2) {
        count = 2;
    }

    switch(count) {
    case 0:
        state = AllHandsUp;
        break;
    case 1:
        state = isLastLeftHand ? RightHandDown : LeftHandDown;
        isLastLeftHand = !isLastLeftHand;
        break;
    case 2:
        state = AllHandsDown;
        break;
    default:
        // compiler happy :)
        break;
    }

    update();
}

void BongoCatWindow::callBack(void *callerdata, void *)
{
    Q_CHECK_PTR(callerdata);

    SEventCallBackData* data = static_cast<SEventCallBackData*>(callerdata);
    switch(data->type&0xF0) {
    case 0x80: // Note off
        if (this->curPolyCnt > 0) {
            curPolyCnt--;
        }
        voiceChanged(curPolyCnt);
        break;
    case 0x90: // Note on
        // TODO: 0 as note off?
        curPolyCnt++;
        voiceChanged(curPolyCnt);
        break;
    default:
        // compiler happy :)
        break;
    }
}

void BongoCatWindow::paintEvent(QPaintEvent *)
{
    const QRect rect(0,0,800,450);
    QPainter painter(this);

    painter.drawPixmap(rect, bongo);

    switch (state) {
    case AllHandsDown:
        painter.drawPixmap(rect, l2);
        painter.drawPixmap(rect, r2);
        break;
    case AllHandsUp:
        painter.drawPixmap(rect, l1);
        painter.drawPixmap(rect, r1);
        break;
    case LeftHandDown:
        painter.drawPixmap(rect, l2);
        painter.drawPixmap(rect, r1);
        break;
    case RightHandDown:
        painter.drawPixmap(rect, l1);
        painter.drawPixmap(rect, r2);
        break;
    }

    painter.drawPixmap(rect, m1);
}

void BongoCatWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_0:
        voiceChanged(0);
        break;
    case Qt::Key_1:
        voiceChanged(1);
        break;
    case Qt::Key_2:
        voiceChanged(2);
        break;
    default:
        // compiler happy :)
        break;
    }
}
