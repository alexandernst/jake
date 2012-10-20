#ifndef SYNCDOWN_H
#define SYNCDOWN_H

#include <QUrl>
#include <QFile>
#include <QDebug>
#include <QObject>
#include <QIODevice>
#include <QEventLoop>
#include <QNetworkReply>
#include <QAuthenticator>
#include <QNetworkRequest>
#include <QNetworkCookieJar>
#include <QNetworkAccessManager>

class SyncDown : public QObject
{
    Q_OBJECT
public:
    SyncDown(QObject *parent = 0);
    ~SyncDown();
private:
    bool active;
    QUrl url;
    QFile *path;
    QString user;
    QString password;
    QString downloadmode;
    QByteArray useragent;
    QByteArray *returndata;
    QEventLoop *eventloop;
    QNetworkReply *reply;
    QNetworkRequest *request;
    QNetworkCookieJar *jar;
    QNetworkAccessManager *manager;
signals:
    void downloadProgress(qint64, qint64);
public slots:
    QByteArray download(QUrl purl, QByteArray puseragent = "Wget/1.12 (linux-gnu)", QString puser = "", QString ppassword = "");
    void downloadToFile(QUrl purl, QString ppath, QByteArray puseragent = "Wget/1.12 (linux-gnu)", QString puser = "", QString ppassword = "");
    void authenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator);
    void readyRead();
    void progressCallback(qint64 done,qint64 total);
    void downloadFinished(QNetworkReply *reply);
    bool isActive();
    void abort();
};

#endif // SYNCDOWN_H
