#ifndef QCUSTOMPLUGIN_H
#define QCUSTOMPLUGIN_H

#include <QDebug>
#include <QObject>
#include <QPointer>
#include <QEventLoop>

class QCustomPlugin : public QObject
{
    Q_OBJECT
private:
    QString tmpaskreply;
    QEventLoop askloop;
public:
    QObject *pluginmanager;
    virtual void process(QString command) = 0;
    QList<QString> *capabilities;
    QList<QString> getCapabilities()
    {
        return *capabilities;
    }
    QString ask(QString question, bool html)
    {
        QObject::connect(pluginmanager, SIGNAL(askReply(QString)), this, SLOT(askReply(QString)));
        emit pluginAsk(question, html);
        askloop.exec();
        return tmpaskreply;
        QObject::disconnect(this, SLOT(askReply(QString)));
    }
public slots:
    void askReply(QString askreply)
    {
        askloop.quit();
        tmpaskreply = askreply;
    }
signals:
    void pluginAsk(QString question, bool html);
    void reply(QString reply, bool html);
    void clearLine();
    void pluginFinished(bool finished);
};

#endif // QCUSTOMPLUGIN_H
