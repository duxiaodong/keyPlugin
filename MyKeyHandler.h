#ifndef MYKEYHANDLER_H
#define MYKEYHANDLER_H
#include <QString>
#include <QObject>
#include <QSocketNotifier>
#include <QtGui/QWSKeyboardHandler>
#include<QTimer>
class KeypadHandler : public QObject, public QWSKeyboardHandler
{
    Q_OBJECT

public:
    KeypadHandler(const QString &device = QString("/dev/input/event0")); //打开设备
    ~KeypadHandler();

private:
    QSocketNotifier *m_notifier; //通知设备上来数据
    int kbdFd;  //打开的设备描述符
    bool ledFlag;
    QTimer *start_time;
    int maxSecond;
protected:
//    void timerEvent(QTimerEvent *e);

private slots:
    void readKpdData();   //信号槽，m_notifier有通知说明设备上有数据
    void closeLcd();               //促发槽readKpdData读取设备上的数据

};


#endif // MYKEYHANDLER_H
