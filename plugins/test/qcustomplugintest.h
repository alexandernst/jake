#ifndef QCUSTOMPLUGINTEST_H
#define QCUSTOMPLUGINTEST_H

#include "../../qcustomplugin.h"

class QCustomPluginTest : public QCustomPlugin
{
    Q_OBJECT
public:
    QCustomPluginTest(QObject *parent = 0);
    void process(QString command);
};

#endif // QCUSTOMPLUGINTEST_H
