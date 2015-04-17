#include <stdio.h>
#include <QDebug>
#include "keypadDriverPlugin.h"
#include "MyKeyHandler.h"

KeypadDriverPlugin::KeypadDriverPlugin(QObject *parent):QKbdDriverPlugin(parent)
{ }

KeypadDriverPlugin::~KeypadDriverPlugin()
{ }

QStringList KeypadDriverPlugin::keys() const
{
    return QStringList() << "optkeypad";
}

// The create() functions are responsible for returning an instance of
// the keypad driver. We do this only if the driver parameter matches our key.

QWSKeyboardHandler *KeypadDriverPlugin::create(const QString &driver, const QString &device)
{
    printf("KeypadDriverPlugin::create###################################: %s\n",driver.toLocal8Bit().constData());
    if (driver.toLower() == "optkeypad"){
        printf("Before creating KeypadHandler\n");
        return new KeypadHandler(device);
    }
    return 0;
}

QWSKeyboardHandler *KeypadDriverPlugin::create(const QString &driver)
{
    if (driver.toLower() == "optkeypad")
    {
        printf("Before creating KeypadHandler");
        return new KeypadHandler();
    }
    return 0;
}

Q_EXPORT_PLUGIN2(optkeypad,KeypadDriverPlugin)


