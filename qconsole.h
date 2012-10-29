#ifndef QCONSOLE_H
#define QCONSOLE_H

#include <QDir>
#include <QFile>
#include <QColor>
#include <QWidget>
#include <QShortcut>
#include <QSettings>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTextStream>
#include <QApplication>
#include <QDesktopWidget>
#include <QPropertyAnimation>
#include "qcustomtabwidget.h"
#include "libs/qtglobalshortcut/qtglobalshortcut.h"

class QConsole : public QWidget
{
    Q_OBJECT
public:
    QConsole(QWidget *parent = 0);
private:
    int appx;
    int appy;
    int screenwidth;
    int screenheight;
    int showonscreen;
    int defaultappwidth;
    int defaultappheight;
    int animationduration;
    bool stayontop;
    float appwidth;
    float appheight;
    QFile *stylefile;
    QString exithotkey;
    QString visibilityhotkey;
    QSettings *settings;
    QShortcut *widgetHook;
    QHBoxLayout *mainLayout;
    QTextStream *stylereader;
    QDesktopWidget *desktopWidget;
    QCustomTabWidget *tabWidget;
    QtGlobalShortcut *keyboardHook;
    QPropertyAnimation *windowHeightAnimation;
    QPropertyAnimation *windowOpacityAnimation;
public slots:
    void toggleVisibility();
    void windowAnimationFinished();
private slots:
    void appKeyPress();
};

#endif // QCONSOLE_H
