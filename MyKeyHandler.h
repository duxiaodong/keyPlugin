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
    KeypadHandler(const QString &device = QString("/dev/input/event0")); //���豸
    ~KeypadHandler();

private:
    QSocketNotifier *m_notifier; //֪ͨ�豸��������
    int kbdFd;  //�򿪵��豸������
    bool ledFlag;
    QTimer *start_time;
    int maxSecond;
protected:
//    void timerEvent(QTimerEvent *e);

private slots:
    void readKpdData();   //�źŲۣ�m_notifier��֪ͨ˵���豸��������
    void closeLcd();               //�ٷ���readKpdData��ȡ�豸�ϵ�����

};


#endif // MYKEYHANDLER_H
