#include "qcustompluginmanager.h"

QCustomPluginManager::QCustomPluginManager(QObject *parent) :
    QObject(parent)
{
    //Don't touch those!
    capabilities = new QList<QString>;
    replies = 0;
    finishedprocess = 0;

    /*************************************************************\
    * C++ doesn't support reflections like C# or Java, so this is *
    * the only way of doing this. Oh yeah, there is the factory   *
    * pattern, but that's just the same thing. In the end I'll    *
    * have to add a case in a switch (if-else if-else if you'd    *
    * like) to keep a list of all "plugins" that are available.   *
    * Also, I didn't used QtPlugins because I wanted to learn how *
    * does a plugin system works inside. Nothing too big, nor too *
    * complicated.                                                *
    * Last but not less important reason of doing it this way is  *
    * that I'm coding just for fun.                               *
    \*************************************************************/

    test = new QCustomPluginTest(static_cast<QObject*>(this));
    if(!plugins.contains(test)){
        plugins.append(test);
    }

    /*************************************************************\
    * Yes, I know. That just sucks...                             *
    \*************************************************************/

    foreach(QCustomPlugin *plugin, plugins){
        QList<QString> plugincapabilities = plugin->getCapabilities();
        foreach(const QString capability, plugincapabilities){
            capabilities->append(capability);
        }
        QObject::connect(plugin, SIGNAL(pluginFinished(bool)), this, SLOT(pluginFinished(bool)));
        QObject::connect(plugin, SIGNAL(pluginAsk(QString, bool)), this, SIGNAL(pluginAsk(QString, bool)));
        QObject::connect(plugin, SIGNAL(reply(QString, bool)), this, SIGNAL(pluginReply(QString, bool)));
        QObject::connect(plugin, SIGNAL(clearLine()), this, SIGNAL(pluginClearLine()));
    }
    QObject::connect(parent, SIGNAL(askReply(QString)), this, SIGNAL(askReply(QString)));
}

QCustomPluginManager::~QCustomPluginManager()
{
    delete capabilities;

    /*************************************************************\
    * Yessssssssss, same here...                                  *
    \*************************************************************/

    test->deleteLater();
}

void QCustomPluginManager::process(QString command)
{
    foreach(QCustomPlugin *plugin, plugins){
        plugin->process(command);
    }
}

QList<QString> QCustomPluginManager::getCapabilities()
{
    return *capabilities;
}

void QCustomPluginManager::pluginFinished(bool finished)
{
    replies++;
    if(finished){
        finishedprocess++;
    }
    if(replies == plugins.size()){
        emit processFinished(finishedprocess == plugins.size());
        replies = 0;
        finishedprocess = 0;
    }
}
