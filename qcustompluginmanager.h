#ifndef QCUSTOMPLUGINMANAGER_H
#define QCUSTOMPLUGINMANAGER_H

#include <QObject>
#include <QPointer>
#include "qcustomplugin.h"
#include "plugins/test/qcustomplugintest.h"

class QCustomPluginManager : public QObject
{
    Q_OBJECT
public:
    QList<QString> getCapabilities();
    QCustomPluginManager(QObject *parent = 0);
    ~QCustomPluginManager();
private:
    int replies;
    int finishedprocess;
    QCustomPlugin *test, *musicgoear;
    QList<QString> *capabilities;
    QList<QCustomPlugin*> plugins;
public slots:
    void process(QString command);
    void pluginFinished(bool finished);
signals:
    void askReply(QString askreply);
    void pluginAsk(QString ask, bool html);
    void pluginReply(QString reply, bool html);
    void pluginClearLine();
    void processFinished(bool finished);
};

#endif // QCUSTOMPLUGINMANAGER_H
