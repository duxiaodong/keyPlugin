#ifndef KEYPADDRIVERPLUGIN_H
#define KEYPADDRIVERPLUGIN_H

//#include <QString>
//#include <QObject>
//#include <QKbdDriverPlugin>
#include <QtGui/QWSKeyboardHandlerFactoryInterface>
//插件类，使用插件类才能访问处理器类，处理器类是QWSKeyboardHandle
class KeypadDriverPlugin : public QKbdDriverPlugin
{
    Q_OBJECT
public:
    KeypadDriverPlugin(QObject *parent = 0);
    ~KeypadDriverPlugin();

    QWSKeyboardHandler *create(const QString &driver, const QString &device);
    QWSKeyboardHandler *create(const QString &driver);
    QStringList keys() const;
    /*The keys function returns the list of drivers that this plug-in provides.
    Our plug-in only provides the "Keypadhandler" driver.*/
};
#endif // KEYPADDRIVERPLUGIN_H
