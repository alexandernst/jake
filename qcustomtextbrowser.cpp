#include "qcustomtabwidget.h"
#include "qcustomtextbrowser.h"

QCustomTextBrowser::QCustomTextBrowser(QWidget *parent) :
    QTextBrowser(parent)
{
    setParent(parent);
    setReadOnly(true);
    setOpenLinks(false);
    setAcceptRichText(true);
    setOpenExternalLinks(false);
    setObjectName("textBrowser");
    setFrameShape(QFrame::NoFrame);
    setContextMenuPolicy(Qt::CustomContextMenu);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //Open settings and go to main app group.
    settings = new QSettings("alexandernst", "jake++");
    settings->beginGroup("jake++");
    welcomemsg = settings->value("welcomemsg", tr("Welcome to Jake++!")).toString();
    prompt = settings->value("prompt", "console@jake: ").toString();
    settings->endGroup();
    settings->deleteLater();

    //Create plugin manager instance
    pluginManager = new QCustomPluginManager(this);
    capabilities = pluginManager->getCapabilities();

    //QFontMetrics
    fm = new QFontMetrics(font());

    //Create command history.
    commandhistory = new QStringList();
    commandhistory->append("");
    commandhistoryindex = 0;
    commandisreply = false;

    //Text cursor and downloader
    downloader = new SyncDown(this);
    textcursor = new QTextCursor(document());

    //Install event filter.
    installEventFilter(this);

    //Set welcome message and console prompt
    insert(welcomemsg, true);
    setPrompt(true);

    //Connect signal.
    QObject::connect(this, SIGNAL(textChanged()), this, SLOT(autoScroll()));
    QObject::connect(this, SIGNAL(anchorClicked(QUrl)), this, SLOT(anchorClicked(QUrl)));
    QObject::connect(this, SIGNAL(customContextMenuRequested(QPoint)), SLOT(rightClick()));
    QObject::connect(pluginManager, SIGNAL(pluginAsk(QString, bool)), this, SLOT(ask(QString, bool)));
    QObject::connect(pluginManager, SIGNAL(pluginReply(QString, bool)), this, SLOT(reply(QString, bool)));
    QObject::connect(pluginManager, SIGNAL(pluginClearLine()), this, SLOT(clearLine()));
    QObject::connect(pluginManager, SIGNAL(processFinished(bool)), this, SLOT(setPrompt(bool)));
}

QCustomTextBrowser::~QCustomTextBrowser()
{
    delete fm;
    delete downloader;
    delete textcursor;
}

void QCustomTextBrowser::autoScroll()
{
    verticalScrollBar()->setValue(verticalScrollBar()->maximum());
}

void QCustomTextBrowser::setPrompt(bool finished)
{
    if(finished){
        insert("<br>", true);
        insert(prompt, true);
    }
    cursorlimit = textcursor->position();
    insert(" ", false);
    textcursor->movePosition(QTextCursor::PreviousCharacter);
    underlineNextChar(true);
    processing = false;
}

bool QCustomTextBrowser::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::KeyPress){
        keyevent = static_cast<QKeyEvent *>(event);
        keyPressEvent(keyevent);
        return true;
    } else {
        return false;
    }
}

void QCustomTextBrowser::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter){
        process();
    } else if(event->key() == Qt::Key_Backspace) {
        deleteLast();
    } else if(event->key() == Qt::Key_Delete) {
        deleteNext();
    } else if(event->key() == Qt::Key_Left) {
        if(event->key() == Qt::Key_Left && event->modifiers() & Qt::ControlModifier) {
            moveLeftControl();
        } else if(event->key() == Qt::Key_Left && event->modifiers() & Qt::ShiftModifier) {
            moveLeftShift();
        } else {
            moveLeft();
        }
    } else if(event->key() == Qt::Key_Right) {
        if(event->key() == Qt::Key_Right && event->modifiers() & Qt::ControlModifier) {
            moveRightControl();
        } else if(event->key() == Qt::Key_Right && event->modifiers() & Qt::ShiftModifier) {
            moveRightShift();
        } else {
            moveRight();
        }
    } else if(event->key() == Qt::Key_Up) {
        upHistory();
    } else if(event->key() == Qt::Key_Down) {
        downHistory();
    } else if(event->key() == Qt::Key_Tab) {
        predict();
    } else {
        insert(event->text(), false);
    }
}

void QCustomTextBrowser::rightClick()
{
    emit showContextMenu();
}

void QCustomTextBrowser::process()
{
    if(!commandisreply && processing)
        return;
    command = getCommand();
    commandhistory->append(command);
    commandhistoryindex = commandhistory->length();
    setCommand(command, false);
    textcursor->movePosition(QTextCursor::End);
    textcursor->deletePreviousChar();
    if(!commandisreply){
        processing = true;
        pluginManager->process(command);
    } else {
        emit askReply(command);
        commandisreply = false;
    }
}

void QCustomTextBrowser::deleteLast()
{
    if(textcursor->position() > cursorlimit){
        textcursor->deletePreviousChar();
    }
}

void QCustomTextBrowser::deleteNext()
{
    textcursor->movePosition(QTextCursor::NextCharacter);
    if(textcursor->atEnd()){
        textcursor->movePosition(QTextCursor::PreviousCharacter);
    } else {
        textcursor->movePosition(QTextCursor::PreviousCharacter);
        textcursor->deleteChar();
        underlineNextChar(true);
    }
}

void QCustomTextBrowser::moveLeftControl()
{
    qDebug() << "moveLeftControl";
}

void QCustomTextBrowser::moveLeftShift()
{
    qDebug() << "moveLeftShift";
}

void QCustomTextBrowser::moveLeft()
{
    if(textcursor->position() > cursorlimit){
        underlineNextChar(false);
        textcursor->movePosition(QTextCursor::PreviousCharacter);
        underlineNextChar(true);
    }
}

void QCustomTextBrowser::moveRightControl()
{
    qDebug() << "moveRightControl";
}

void QCustomTextBrowser::moveRightShift()
{
    qDebug() << "moveRightShift";
}

void QCustomTextBrowser::moveRight()
{
    textcursor->movePosition(QTextCursor::NextCharacter);
    if(textcursor->atEnd()){
        textcursor->movePosition(QTextCursor::PreviousCharacter);
        underlineNextChar(true);
        underlinePrevChar(false);
    } else {
        underlinePrevChar(false);
        underlineNextChar(true);
    }
}

void QCustomTextBrowser::upHistory()
{
    if(commandhistoryindex == 0){
        commandhistoryindex = commandhistory->length()-1;
    } else {
        commandhistoryindex -= 1;
    }
    setCommand(commandhistory->at(commandhistoryindex), false);
}

void QCustomTextBrowser::downHistory()
{
    if(commandhistoryindex >= commandhistory->length()-1){
        commandhistoryindex = 0;
    } else {
        commandhistoryindex += 1;
    }
    setCommand(commandhistory->at(commandhistoryindex), false);
}

void QCustomTextBrowser::predict()
{
    if(commandisreply)
        return;
    QString currentcommand = getCommand();
    capabilitiescompletelist = new QList<QString>;
    foreach(QString capability, capabilities){
        if(capability.startsWith(currentcommand))
            capabilitiescompletelist->append(capability);
    }
    QString capability = getPrefixFromList(capabilitiescompletelist);
    if(currentcommand.length() < capability.length() && capability.length() > 0){
        setPrompt(true);
        setCommand(capability, false);
    } else if (currentcommand.length() > 0 && capabilitiescompletelist->size() > 1){
        showCompleteHTMLTable(currentcommand, capabilitiescompletelist);
    } else if (currentcommand.length() == 0) {
        showCompleteHTMLTable(currentcommand, capabilitiescompletelist);
    } else {
        if(capabilitiescompletelist->size() == 0){
            //do nothing
        } else if(capabilitiescompletelist->size() == 1){
            setCommand(capabilitiescompletelist->at(0), false);
        }
    }
    delete capabilitiescompletelist;
}

void QCustomTextBrowser::showCompleteHTMLTable(QString currentcommand, QList<QString> *capabilitiescompletelist)
{
    maxlenstr = "";
    foreach(QString capability, *capabilitiescompletelist){
        if(capability.length() > maxlenstr.length())
            maxlenstr = capability;
    }
    int maxlenpx = fm->width(maxlenstr) + 40; //40px
    int maxncols = width() / maxlenpx;
    int i = 0;
    QString completetable = "<table><tr>";
    foreach(QString capability, *capabilitiescompletelist){
        completetable += "<td width='" + QString("%1").arg(maxlenpx) + "'>" + capability + "</td>";
        i++;
        if(i == maxncols){
          i = 0;
          completetable += "</tr><tr>";
        }
    }
    completetable += "</tr></table>";
    insert(completetable, true);
    setPrompt(true);
    //So, we started typing something, we should auto-complete it!
    if(currentcommand.length() > 0)
        currentcommand = getPrefixFromList(capabilitiescompletelist);
    setCommand(currentcommand, false);
}

QString QCustomTextBrowser::getPrefixFromList(QList<QString> *capabilitiescompletelist)
{
    QString capability = "";
    if(capabilitiescompletelist->count() == 0)
      return capability;
    //Sort alphabetically and compare the first and the last word in the list to check that.
    qSort(capabilitiescompletelist->begin(), capabilitiescompletelist->end());
    QString firstword = capabilitiescompletelist->at(0);
    QString lastword = capabilitiescompletelist->at(capabilitiescompletelist->size()-1);
    int maxlen = firstword.length() > lastword.length() ? lastword.length() : firstword.length();
    for(int i = 0; i < maxlen; i++){
        if(firstword.at(i) == lastword.at(i))
            capability += firstword.at(i);
        else
            break;
    }
    return capability;
}

void QCustomTextBrowser::clearLine()
{
    textcursor->movePosition(QTextCursor::EndOfLine);
    textcursor->movePosition(QTextCursor::StartOfLine, QTextCursor::KeepAnchor);
    textcursor->removeSelectedText();
}

void QCustomTextBrowser::ask(QString ask, bool html)
{
    commandisreply = true;
    insert(ask, html);
    setPrompt(false);
}

void QCustomTextBrowser::reply(QString reply, bool html)
{
    insert(reply, html);
    cursorlimit = textcursor->position();
}

void QCustomTextBrowser::insert(QString str, bool html)
{
    if(html){
        textcursor->insertHtml(str);
    } else {
        textcursor->insertText(str);
    }
}

QString QCustomTextBrowser::getCommand()
{
    cursorposition = textcursor->position();
    textcursor->movePosition(QTextCursor::End);
    textcursor->movePosition(QTextCursor::PreviousCharacter);
    textcursor->setPosition(cursorlimit, QTextCursor::KeepAnchor);
    command = textcursor->selectedText();
    textcursor->setPosition(cursorposition);
    return command;
}

void QCustomTextBrowser::setCommand(QString command, bool html)
{
    textcursor->movePosition(QTextCursor::End);
    textcursor->movePosition(QTextCursor::PreviousCharacter);
    textcursor->setPosition(cursorlimit, QTextCursor::KeepAnchor);
    insert(command, html);
    textcursor->movePosition(QTextCursor::End);
    textcursor->movePosition(QTextCursor::PreviousCharacter);
    underlineNextChar(true);
}

void QCustomTextBrowser::underlinePrevChar(bool underline)
{
    textcursor->movePosition(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor);
    charformat = textcursor->charFormat();
    charformat.setFontUnderline(underline);
    textcursor->setCharFormat(charformat);
    textcursor->movePosition(QTextCursor::NextCharacter);
}

void QCustomTextBrowser::underlineNextChar(bool underline)
{
    textcursor->movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
    charformat = textcursor->charFormat();
    charformat.setFontUnderline(underline);
    textcursor->setCharFormat(charformat);
    textcursor->movePosition(QTextCursor::PreviousCharacter);
}

QVariant QCustomTextBrowser::loadResource(int type, QUrl name)
{
    QVariant ret = QVariant();
    if(name.toString().startsWith("http://")){
        QTemporaryFile *tmpfile = new QTemporaryFile();
        tmpfile->open();
        QString tmpfilepath = tmpfile->fileName();
        tmpfile->close();
        tmpfile->deleteLater();
        downloader->downloadToFile(name, tmpfilepath);
        ret = QTextBrowser::loadResource(type, QUrl(tmpfilepath));
    } else {
        ret = QTextBrowser::loadResource(type, name);
    }
    return ret;
}

void QCustomTextBrowser::anchorClicked(QUrl url)
{
    if(url.toString().startsWith("player://")){
        emit musicLinkClicked(QUrl("http" + url.toString().mid(6)));
    } else {
        QDesktopServices::openUrl(url);
    }
}
