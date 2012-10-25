#include "qconsole.h"

QConsole::QConsole(QWidget *parent) :
    QWidget(parent)
{
    //Set object name.
    setObjectName("mainWidget");

    //Open settings and go to main app group.
    settings = new QSettings("alexandernst", "jake++");
    settings->beginGroup("jake++");
    defaultappwidth = settings->value("appwidth", 100).toInt();
    defaultappheight = settings->value("appheight", 50).toInt();
    showonscreen = settings->value("showonscreen", 1).toInt();
    animationduration = settings->value("animationduration", 500).toInt();
    stayontop = settings->value("stayontop", true).toBool();
    visibilityhotkey = settings->value("visibilityhotkey", "F8").toString();
    exithotkey = settings->value("exithotkey", "Esc").toString();
    settings->endGroup();
    settings->deleteLater();

    //Set flags and attributes to make app look better.
    if(stayontop){
        setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::Tool);
    } else {
        setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    }
    setAttribute(Qt::WA_TranslucentBackground);

    //Get screen size.
    desktopWidget = new QDesktopWidget();
    screenwidth = desktopWidget->screenGeometry().width();
    screenheight = desktopWidget->screenGeometry().height();
    desktopWidget->deleteLater();

    //Calc and set correct app size and move to the right place.
    appwidth = screenwidth * (defaultappwidth / 100.0);
    appheight = screenheight * (defaultappheight / 100.0);
    appx = ((screenwidth - appwidth) / 2) + (showonscreen * screenwidth);
    appy = 0;
    resize(appwidth, appheight);
    move(appx, appy);

    //Create animations and connect signals.
    windowOpacityAnimation  = new QPropertyAnimation(this, "windowOpacity");
    windowOpacityAnimation->setDuration(animationduration);
    windowHeightAnimation  = new QPropertyAnimation(this, "geometry");
    windowHeightAnimation->setDuration(animationduration);
    QObject::connect(windowHeightAnimation, SIGNAL(finished()), SLOT(windowAnimationFinished()));
    QObject::connect(windowOpacityAnimation, SIGNAL(finished()), SLOT(windowAnimationFinished()));

    //Create a custom tabwidget
    tabWidget = new QCustomTabWidget(this);

    //Create layout and set it up.
    mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName("mainLayout");
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(tabWidget);

    //Create global keyboard hook to control hotkeys.
    widgetHook = new QShortcut(exithotkey, this);
    keyboardHook = new QtGlobalShortcut();
    keyboardHook->setShortcut(QKeySequence(visibilityhotkey));
    QObject::connect(widgetHook, SIGNAL(activated()), this, SLOT(appKeyPress()));
    QObject::connect(keyboardHook, SIGNAL(activated()), this, SLOT(globalKeyPress()));
}

void QConsole::appKeyPress()
{
    windowOpacityAnimation->deleteLater();
    windowHeightAnimation->deleteLater();
    delete tabWidget;
    mainLayout->deleteLater();
    widgetHook->deleteLater();
    keyboardHook->deleteLater();
    exit(0);
}

void QConsole::globalKeyPress()
{
    if(isVisible()){
        //Fade animation.
        windowOpacityAnimation->setStartValue(1.0);
        windowOpacityAnimation->setEndValue(0.0);
        //Size animation.
        windowHeightAnimation->setStartValue(QRect(appx, appy, appwidth, appheight));
        windowHeightAnimation->setEndValue(QRect(appx, appy, appwidth, 0));
    } else {
        //Fade animation.
        setVisible(true);
        windowOpacityAnimation->setStartValue(0.0);
        windowOpacityAnimation->setEndValue(1.0);
        //Size animation.
        windowHeightAnimation->setStartValue(QRect(appx, appy, appwidth, 0));
        windowHeightAnimation->setEndValue(QRect(appx, appy, appwidth, appheight));
    }
    //Start animations.
    windowOpacityAnimation->start();
    windowHeightAnimation->start();
}

void QConsole::windowAnimationFinished()
{
    if(windowOpacityAnimation->endValue() == 1.0){
        //We don't need to do anything here as we already set visibility to true.
    } else {
        setVisible(false);
    }
}
