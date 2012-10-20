#ifndef QCUSTOMTEXTBROWSER_H
#define QCUSTOMTEXTBROWSER_H

#include <QDebug>
#include <QKeyEvent>
#include <QSettings>
#include <QScrollBar>
#include <QClipboard>
#include <QTextBrowser>
#include <QApplication>
#include <QTemporaryFile>
#include <QDesktopServices>
#include "qcustompluginmanager.h"
#include "libs/qtnam/qtdownloader.h"

class QCustomTextBrowser : public QTextBrowser
{
    Q_OBJECT
public:
    QCustomTextBrowser(QWidget *parent = 0);
    ~QCustomTextBrowser();
protected:
    bool eventFilter(QObject *obj, QEvent *event);
    void keyPressEvent(QKeyEvent *event);
    QVariant loadResource(int type, QUrl name);
private:
    int cursorlimit;
    int cursorposition;
    int commandhistoryindex;
    bool processing;
    bool commandisreply;
    QString prompt;
    QString command;
    QString welcomemsg;
    SyncDown *downloader;
    QKeyEvent *keyevent;
    QSettings *settings;
    QString maxlenstr;
    QClipboard *clipboard;
    QTextCursor *textcursor;
    QStringList *commandhistory;
    QFontMetrics *fm;
    QList<QString> capabilities;
    QList<QString> *capabilitiescompletelist;
    QTextCharFormat charformat;
    QCustomPluginManager *pluginManager;
signals:
    void showContextMenu();
    void musicLinkClicked(QUrl url);
    void askReply(QString askreply);
public slots:
    void ask(QString ask, bool html);
    void insert(QString str, bool html);
    void reply(QString reply, bool html);
    void clearLine();
private slots:
    void process();
    void predict();
    void moveLeft();
    void moveRight();
    void upHistory();
    void rightClick();
    void deleteLast();
    void deleteNext();
    void autoScroll();
    void downHistory();
    void moveLeftShift();
    void moveRightShift();
    void moveLeftControl();
    void moveRightControl();
    void anchorClicked(QUrl url);
    void setPrompt(bool finished);
    void underlinePrevChar(bool underline);
    void underlineNextChar(bool underline);
    void setCommand(QString command, bool html);
    void showCompleteHTMLTable(QString currentcommand, QList<QString> *capabilitiescompletelist);
    QString getCommand();
    QString getPrefixFromList(QList<QString> *capabilitiescompletelist);
};

#endif // QCUSTOMTEXTBROWSER_H
