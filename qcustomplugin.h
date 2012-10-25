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
    QList<QString> getCapabilities();
    QString ask(QString question, bool html);
public slots:
    void askReply(QString askreply);
signals:
    void pluginAsk(QString question, bool html);
    void reply(QString reply, bool html);
    void clearLine();
    void pluginFinished(bool finished);
};

#endif // QCUSTOMPLUGIN_H
