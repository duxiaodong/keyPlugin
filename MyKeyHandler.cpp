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
    //  unsigned int dummy3;
    //  unsigned int dummy4;
    //  unsigned int dummy5;
    //  unsigned int dummy6;
};   //从设备读取到的设备结构 16字节包
//const int KeypadHandler::MaxSecond = 30000;
KeypadHandler::KeypadHandler(const QString &device)  //很据头文件中打开设备 /dev/input/event0
{
    start_time = new QTimer;
    ledFlag = true; //默认不锁屏
    maxSecond = 0;
    QObject::connect(start_time,SIGNAL(timeout()),this ,SLOT(closeLcd()));
    start_time->start(5000);

    setObjectName("Optimus Keypad Handler");
    this->kbdFd = ::open(device.toLocal8Bit().constData(), O_RDONLY, 0);

    if (kbdFd > 0)
    {
        printf("%s opened as keyboard input.\n", device.toLocal8Bit().constData());
        this->m_notifier = new QSocketNotifier(kbdFd, QSocketNotifier::Read, this);
        connect(this->m_notifier, SIGNAL(activated(int)), this, SLOT(readKpdData()));  //信号与槽
    }
    else
    {
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

    int n = read(kbdFd, &event, sizeof(InputData));
    if (n != sizeof(InputData)){
        printf("key pressed: n=%d\n", n);
        return;
    }

    Qt::KeyboardModifiers modifiers = Qt::NoModifier;
    int unicode = 0xffff;
    int key_code = 0;

    if(!ledFlag && event.code == 0 &&event.value == 0){
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
            unicode = 0xffff;
            maxSecond = 0;
            break;
        case 1:
            key_code = Qt::Key_Right;
            unicode = 0xffff;
            maxSecond = 0;
            break;
        case 4:
            key_code = Qt::Key_Up;
            unicode = 0xffff;
            maxSecond = 0;
            break;
        case 6:
            key_code = Qt::Key_Down;
            unicode = 0xffff;
            maxSecond = 0;
            break;
        case 5:
            key_code = Qt::Key_5;
            unicode = 0xffff;
            maxSecond = 0;
            break;
        case 0:
            key_code = Qt::Key_Backspace;
            unicode = 0xffff;
            maxSecond = 0;
            break;
        case 2:
            key_code = Qt::Key_8;
            unicode = 0xffff;
            maxSecond = 0;
            break;
        case 8:
            key_code = Qt::Key_Space;
            unicode = 0xffff;
            maxSecond = 0;
            break;
        case 10:
            key_code = Qt::Key_A;
            unicode = 0xffff;
            maxSecond = 0;
            break;
        case 15:
            key_code = Qt::Key_F;//复用通话
            unicode = 0xffff;
            maxSecond = 0;
            break;
        case 3://群呼
            key_code = Qt::Key_D;
            unicode = 0xffff;
            maxSecond = 0;
            break;
        case 11://单呼
            key_code = Qt::Key_B;
            unicode = 0xffff;
            maxSecond = 0;
            break;
        case 7://组呼
            key_code = Qt::Key_C;
            unicode = 0xffff;
            maxSecond = 0;
            break;
        default:
            qDebug("Unrecognised key code %d", event.code );
            break;
        }
        QWSServer::processKeyEvent(unicode, key_code, modifiers, event.value != 0,false);
    }

}

void KeypadHandler::closeLcd()
{
    if(maxSecond < 30){
        maxSecond+=5;
    }
    qDebug() << maxSecond <<"Second close LED";
    if(ledFlag && maxSecond == 30){
        system("gpio 63 0");
        ledFlag = false;
    }
}
