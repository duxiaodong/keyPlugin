#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
//#include <linux/input.h>
#include <QDebug>
#include "MyKeyHandler.h"
#include <QWSServer>
#include<QTimer>
#include<QDebug>
struct InputData
{
    unsigned int dummy1;
    unsigned int dummy2;
    unsigned short type;
    unsigned short code;
    unsigned int value;
};   //从设备读取到的设备结构 16字节包
//const int KeypadHandler::MaxSecond = 30000;
KeypadHandler::KeypadHandler(const QString &device)  //很据头文件中打开设备 /dev/input/event0
{
    start_time = new QTimer;
    ledFlag = true; //默认不锁屏
    videoFlag = false;//默认不开机录像
    maxSecond = 0;
    QObject::connect(start_time,SIGNAL(timeout()),this ,SLOT(closeLcd()));
    start_time->start(5000);

    setObjectName("Optimus Keypad Handler");
    this->kbdFd = ::open(device.toLocal8Bit().constData(), O_RDONLY, 0);

    if (kbdFd > 0){
        printf("%s opened as keyboard input.\n", device.toLocal8Bit().constData());
        this->m_notifier = new QSocketNotifier(kbdFd, QSocketNotifier::Read, this);
        connect(this->m_notifier, SIGNAL(activated(int)), this, SLOT(readKpdData()));  //信号与槽
    }else{
        printf("Cannot open %s for keyboard input. (%s)",device.toLocal8Bit().constData(), strerror(errno));
        return;
    }
}

KeypadHandler::~KeypadHandler()
{
    if (kbdFd >= 0)
        ::close(kbdFd);
    delete start_time;
}
// Key function 一次处理读取到的32字节, should be 16B???
void KeypadHandler::readKpdData()
{
    InputData event;
    Qt::KeyboardModifiers modifiers = Qt::NoModifier;
    int unicode = 0xffff;
    int key_code = 0;

    int n = read(kbdFd, &event, sizeof(InputData));
    if (n != sizeof(InputData)){
        printf("key pressed: n=%d\n", n);
        return;
    } 
    if(!ledFlag && event.code == 0 && event.value == 0){
        qDebug() << "####################open Led close keyboard";
        system("gpio 63 1");
        ledFlag = true;
        maxSecond = 0;
        return;
    }
    if(ledFlag){
        printf("key pressed: type=%d, code=0x%x, value=%d, %s\n",
               event.type, event.code, event.value, (event.value != 0)? "(Down)" : "(Up)");
        switch (event.code)
        {
        case 9:
            key_code = Qt::Key_Left;
            maxSecond = 0;
            break;
        case 1:
            key_code = Qt::Key_Right;
            maxSecond = 0;
            break;
        case 4:
            key_code = Qt::Key_Up;
            maxSecond = 0;
            break;
        case 6:
            key_code = Qt::Key_Down;
            maxSecond = 0;
            break;
        case 5:
            key_code = Qt::Key_5;
            maxSecond = 0;
            break;
        case 0:
            key_code = Qt::Key_Backspace;
            maxSecond = 0;
            break;
        case 2:
            key_code = Qt::Key_8;
            maxSecond = 0;
            break;
        case 8:
            key_code = Qt::Key_Space;
            maxSecond = 0;
            break;
        case 10:
            key_code = Qt::Key_A;
            maxSecond = 0;
            break;
        case 15:
            key_code = Qt::Key_F;//复用通话
            maxSecond = 0;
            break;
        case 3://群呼
            key_code = Qt::Key_D;
            maxSecond = 0;
            break;
        case 11://单呼
            key_code = Qt::Key_B;
            maxSecond = 0;
            break;
        case 7://组呼
            key_code = Qt::Key_C;
            maxSecond = 0;
            break;
        default:
            maxSecond = 0;
            qDebug("Unrecognised key code %d", event.code );
            return;
            break;
        }
        if(event.code == 2&&event.value == 0){
            //解除定时器，不再进行屏保操作；
            QObject::disconnect(start_time,SIGNAL(timeout()),this,SLOT(closeLcd()));
            maxSecond = 0;
            videoFlag = true;
        }
        if(videoFlag && event.code == 0 && event.value == 0){
            QObject::connect(start_time,SIGNAL(timeout()),this ,SLOT(closeLcd()));
            start_time->start(5000);
            maxSecond = 0;
            videoFlag = false;
        }
        QWSServer::processKeyEvent(unicode, key_code, modifiers, event.value != 0,false);
    }
}

void KeypadHandler::closeLcd()
{

    if(maxSecond < 30){
        maxSecond+=5;
    }
    qDebug() << (30 - maxSecond) <<"Second close LED";
    if(ledFlag && maxSecond == 30){
        system("gpio 63 0");
        ledFlag = false;
    }
    else return;
}
