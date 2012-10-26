#include "qconsole.h"

int main(int argc, char *argv[])
{
    QtSingleApplication app("jake++", argc, argv);

    if (app.sendMessage("Do I exist?"))
        return 0;
    app.initialize(true);
    app.setGraphicsSystem("raster");
    app.setApplicationName("jake++");
    app.setQuitOnLastWindowClosed(false);
    Q_INIT_RESOURCE(resources);
    QConsole console;
    console.toggleVisibility();
    QFile qss(":/skins/resources/skins/Black/style.qss");
    qss.open(QFile::ReadOnly);
    console.setStyleSheet(qss.readAll());
    qss.close();
    app.setActivationWindow(&console);
    QObject::connect(&app, SIGNAL(messageReceived(const QString&)), &console, SLOT(toggleVisibility()));
    QDir::setCurrent(QCoreApplication::applicationDirPath());
    return app.exec();
}
