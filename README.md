## keyPlugin##
1.在QT/E中实现自定义键盘设备，利用了QT/E的C/S架构，运行在服务端，具体参见QT中插件的写法。插件由插件封装类和处理器类两个构成，需要开发者自己去实现处理器类。

2.键盘主要是为QWSKeyboardHandler的一个实例，键盘插件只要收到底层操作系统键盘设备的按键后调用processKeyEvent函数，然后将键盘信息传到QT系统，QWSserver负责分发给客户端。

3.为实现自定义键盘，首先继承QWSKeyboardHandler，然后在这个类中实现设备初始化，等待用户输入，然后通procesKeyEvent函数传递QT系统

4.底层设备有驱动负责，打开设备，监听设备描述符。

`notifier = new QSocketNotifier(kbdFD, QSocketNotifier::Read, this);
connect(notifier, SIGNAL(activated(int)),this,SLOT(readKeyboardData()));`

5.等待用户输入，根据键值判定

`void QWSServer::processKeyEvent ( int unicode, int keycode, Qt::KeyboardModifiers modifiers, bool isPress, bool autoRepeat )   [static]`