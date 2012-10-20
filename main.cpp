#include "qconsole.h"

#if defined(Q_WS_WIN)

#define _WIN32_WINNT 0x0500
#include <windows.h>
#include <stdio.h>
#include <shellapi.h>

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    #if !defined(QT_DEBUG)
    ShowWindow(GetConsoleWindow(), SW_HIDE);
    #endif
    QtSingleApplication app("jake++", __argc, __argv);

#else

int main(int argc, char *argv[])
{
    QtSingleApplication app("jake++", argc, argv);

#endif

    if (app.sendMessage("Do I exist?"))
        return 0;
    app.initialize(true);
    app.setGraphicsSystem("raster");
    app.setApplicationName("jake++");
    app.setQuitOnLastWindowClosed(false);
    Q_INIT_RESOURCE(resources);
    QConsole console;
    console.show();
    QFile qss(":/skins/resources/skins/Black/style.qss");
    qss.open(QFile::ReadOnly);
    console.setStyleSheet(qss.readAll());
    qss.close();
    app.setActivationWindow(&console);
    QObject::connect(&app, SIGNAL(messageReceived(const QString&)), &console, SLOT(globalKeyPress()));
    QDir::setCurrent(QCoreApplication::applicationDirPath());
    return app.exec();
}
