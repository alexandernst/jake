#ifndef ASYNCDOWN_H
#define ASYNCDOWN_H

#include <QUrl>
#include <QFile>
#include <QDebug>
#include <QThread>
#include <QIODevice>
#include <QNetworkReply>
#include <QAuthenticator>
#include <QNetworkRequest>
#include <QNetworkCookieJar>
#include <QNetworkAccessManager>

class AsyncDown : public QThread
{
    Q_OBJECT
public:
    AsyncDown(QObject *parent = 0);
    ~AsyncDown();
private:
    bool active;
    QUrl url;
    QFile *path;
    QString user;
    QString password;
    QString downloadmode;
    QByteArray useragent;
    QByteArray *returndata;
    QNetworkReply *reply;
    QNetworkRequest *request;
    QNetworkCookieJar *jar;
    QNetworkAccessManager *manager;
protected:
    void run();
signals:
    void downloadProgress(qint64, qint64);
    void downloadFinished();
    void downloadFinished(QByteArray);
public slots:
    void download(QUrl purl, QByteArray puseragent = "Wget/1.12 (linux-gnu)", QString puser = "", QString ppassword = "");
    void downloadToFile(QUrl purl, QString ppath, QByteArray puseragent = "Wget/1.12 (linux-gnu)", QString puser = "", QString ppassword = "");
    void authenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator);
    void readyRead();
    void progressCallback(qint64 done,qint64 total);
    void downloadFinished(QNetworkReply *reply);
    bool isActive();
    void abort();
};

#endif // ASYNCDOWN_H
