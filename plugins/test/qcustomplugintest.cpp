#include "qcustomplugintest.h"
#include <QtTest/QTest>

QCustomPluginTest::QCustomPluginTest(QObject *parent)
{
    pluginmanager = parent;
    capabilities = new QList<QString>;
    capabilities->append("!test");
}

void QCustomPluginTest::process(QString command)
{
    if(command == tr("!test")){
        emit reply("<br>", true);
        for(int i = 0; i < 100; i++){
            emit reply(QString("%1 %").arg(i), false);
            QTest::qWait(10);
            emit clearLine();
        }
        emit reply("100 %<br>", true);
        QString str = ask(QString("Reply with some answer: "), false);
        emit reply("<br>Got reply: " + str, true);
    }
    emit pluginFinished(true);
}
