#include "asyncdown.h"

AsyncDown::AsyncDown(QObject *parent) :
    QThread(parent)
{
    active = false;
}

AsyncDown::~AsyncDown()
{
    if(isActive()){
        abort();
    }
}

void AsyncDown::run()
{
    active = true;
    //Create QNAM.
    jar = new QNetworkCookieJar(this);
    manager = new QNetworkAccessManager(this);
    manager->setCookieJar(jar);
    request = new QNetworkRequest(url);
    request->setRawHeader("User-Agent", useragent);
    reply = manager->get(*request);
    if(downloadmode == "async"){
        QObject::connect(reply, SIGNAL(readyRead()), SLOT(readyRead()));
    }

    //Connect signal.
    QObject::connect(manager, SIGNAL(finished(QNetworkReply *)), SLOT(downloadFinished(QNetworkReply *)));
    QObject::connect(reply, SIGNAL(downloadProgress(qint64,qint64)), SLOT(progressCallback(qint64,qint64)));
    QObject::connect(manager, SIGNAL(authenticationRequired(QNetworkReply *,QAuthenticator *)), SLOT(authenticationRequired(QNetworkReply *,QAuthenticator *)));

    //Run the thread.
    exec();
}

void AsyncDown::download(QUrl purl, QByteArray puseragent, QString puser, QString ppassword)
{
    //Create request and reply.
    moveToThread(this);
    downloadmode = "sync";
    url = purl;
    useragent = puseragent;
    user = puser;
    password = ppassword;
    returndata = new QByteArray();
    start();
}

void AsyncDown::downloadToFile(QUrl purl, QString ppath, QByteArray puseragent, QString puser, QString ppassword)
{
    //Create request and reply.
    moveToThread(this);
    downloadmode = "async";
    url = purl;
    useragent = puseragent;
    user = puser;
    password = ppassword;
    path = new QFile(ppath);
    path->open(QIODevice::WriteOnly);
    start();
}

void AsyncDown::authenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator)
{
    //Set user and passwork if any.
    authenticator->setUser(user);
    authenticator->setPassword(password);
}

void AsyncDown::readyRead()
{
    //Write to disk.
    path->write(reply->readAll());
}

void AsyncDown::progressCallback(qint64 done,qint64 total)
{
    //Send signal.
    emit downloadProgress(done, total);
}

void AsyncDown::downloadFinished(QNetworkReply *oldreply)
{
    //Check if we have a redirect.
    QUrl redirect = oldreply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
    if(!redirect.isEmpty()){
        //Yes, we have a redirect. Go for it.
        oldreply->deleteLater();
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
            QObject::connect(reply, SIGNAL(readyRead()), SLOT(readyRead()));
        }
        QObject::connect(reply, SIGNAL(downloadProgress(qint64,qint64)), SLOT(progressCallback(qint64,qint64)));
    } else {
        //No redirect. Check if we are in sync or async mode and return.
        if(downloadmode == "sync"){
            emit downloadFinished(oldreply->readAll());
        } else {
            path->close();
            path->deleteLater();
            emit downloadFinished();
        }
        //Quit the event loop.
        oldreply->deleteLater();
        active = false;
        exit();
    }
}

bool AsyncDown::isActive()
{
    return active;
}

void AsyncDown::abort()
{
    reply->abort();
    if(path->isOpen()){
        path->close();
        path->deleteLater();
    }
    reply->deleteLater();
    active = false;
    exit();
    wait();
}
