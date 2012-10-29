#include "qconsole.h"
#include <QSharedMemory>

bool isRunning(){
    QSharedMemory sm("jake++");

    if(sm.create(1)){
        return false; //Wrong on Windows
        //http://stackoverflow.com/questions/13120374/qsharedmemory-inconsistent-behaviour-in-windows
        //damn trolls coded shitty code...
    }else{
        switch(sm.error()){
            case QSharedMemory::NoError:
                //No other instances are running
                return false;
                break;
            case QSharedMemory::AlreadyExists:
                //There is a shared memory with the same key
                //Check if that memory is left after a crash
                sm.attach();
                sm.detach();
                return isRunning();
                break;
            default:
                return true;
                break;
        }
    }
    return true;
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    if(isRunning()){
        return 0;
    }
    app.setGraphicsSystem("raster");
    app.setApplicationName("jake++");
    app.setQuitOnLastWindowClosed(false);
    Q_INIT_RESOURCE(resources);
    QConsole console;
    QFile qss(":/skins/resources/skins/Black/style.qss");
    qss.open(QFile::ReadOnly);
    console.setStyleSheet(qss.readAll());
    qss.close();
    console.toggleVisibility();
    QDir::setCurrent(QCoreApplication::applicationDirPath());
    return app.exec();
}
