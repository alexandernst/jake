#include "qcustomplugin.h"

QList<QString> QCustomPlugin::getCapabilities()
{
    return *capabilities;
}

QString QCustomPlugin::ask(QString question, bool html)
{
    QObject::connect(pluginmanager, SIGNAL(askReply(QString)), this, SLOT(askReply(QString)));
    emit pluginAsk(question, html);
    askloop.exec();
    return tmpaskreply;
    QObject::disconnect(this, SLOT(askReply(QString)));
}

void QCustomPlugin::askReply(QString askreply)
{
    askloop.quit();
    tmpaskreply = askreply;
}
