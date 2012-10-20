#include "syncdown.h"

SyncDown::SyncDown(QObject *parent) :
    QObject(parent)
{
    active = false;
    //Create QNAM.
    jar = new QNetworkCookieJar(this);
    manager = new QNetworkAccessManager(this);
    manager->setCookieJar(jar);
    eventloop = new QEventLoop(this);

    //Connect signals.
    QObject::connect(manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(downloadFinished(QNetworkReply *)));
    QObject::connect(manager, SIGNAL(authenticationRequired(QNetworkReply *,QAuthenticator *)), this, SLOT(authenticationRequired(QNetworkReply *,QAuthenticator *)));
}

SyncDown::~SyncDown()
{
    if(isActive()){
        abort();
    }
}

QByteArray SyncDown::download(QUrl purl, QByteArray puseragent, QString puser, QString ppassword)
{
    active = true;
    //Create request and reply.
    downloadmode = "sync";
    url = purl;
    useragent = puseragent;
    user = puser;
    password = ppassword;
    returndata = new QByteArray();
    request = new QNetworkRequest(url);
    request->setRawHeader("User-Agent", useragent);
    reply = manager->get(*request);

    //Connect signal.
    QObject::connect(reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(progressCallback(qint64,qint64)));

    //Run the event loop and when done return what we got.
    eventloop->exec();
    active = false;
    return *returndata;
}

void SyncDown::downloadToFile(QUrl purl, QString ppath, QByteArray puseragent, QString puser, QString ppassword)
{
    active = true;
    //Create request and reply.
    downloadmode = "async";
    url = purl;
    useragent = puseragent;
    user = puser;
    password = ppassword;
    path = new QFile(ppath);
    path->open(QIODevice::WriteOnly);
    request = new QNetworkRequest(url);
    request->setRawHeader("User-Agent", useragent);
    reply = manager->get(*request);

    //Connect signals
    QObject::connect(reply, SIGNAL(readyRead()), this, SLOT(readyRead()));
    QObject::connect(reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(progressCallback(qint64,qint64)));

    //Run the event loop.
    eventloop->exec();
    active = false;
}

void SyncDown::authenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator)
{
    //Set user and passwork if any.
    authenticator->setUser(user);
    authenticator->setPassword(password);
}

void SyncDown::readyRead()
{
    //Write to disk.
    path->write(reply->readAll());
}

void SyncDown::progressCallback(qint64 done,qint64 total)
{
    //Send signal.
    emit downloadProgress(done, total);
}

void SyncDown::downloadFinished(QNetworkReply *reply)
{
    //Check if we have a redirect.
    QUrl redirect = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
    if(!redirect.isEmpty()){
        //Yes, we have a redirect. Go for it.
        reply->deleteLater();
        request = new QNetworkRequest(redirect);
        request->setRawHeader("User-Agent", useragent);
        reply = manager->get(*request);
        //If we are in async mode we wrote the response (redirect) to disk.
        //Close the file, and open it again.
        if(downloadmode == "async"){
            if(path->isOpen()){
                path->close();
                path->open(QIODevice::WriteOnly);
            }
        }
        QObject::connect(reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(progressCallback(qint64,qint64)));
    } else {
        //No redirect. Check if we are in sync or async mode and return.
        if(downloadmode == "sync"){
            *returndata = reply->readAll();
        } else {
            path->close();
            path->deleteLater();
        }
        //Quit the event loop.
        reply->deleteLater();
        eventloop->quit();
    }
}

bool SyncDown::isActive()
{
    return active;
}

void SyncDown::abort()
{
    reply->abort();
    if(path->isOpen()){
        path->close();
        path->deleteLater();
    }
    reply->deleteLater();
    active = false;
    eventloop->quit();
}
