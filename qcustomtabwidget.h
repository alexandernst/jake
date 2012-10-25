#ifndef QCUSTOMTABWIDGET_H
#define QCUSTOMTABWIDGET_H

#include <QMenu>
#include <QSettings>
#include <QTabWidget>
#include <QClipboard>
#include <QPushButton>
#include <QScrollArea>
#include "qcustomtabbar.h"
#include "qcustomtextbrowser.h"

class QCustomTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    QClipboard *clipboard;
    QCustomTabWidget(QWidget *parent = 0);
    ~QCustomTabWidget();
private:
    QMenu *contextmenu;
    QString tabnameprefix;
    QSettings *settings;
    QPushButton *addTabButton;
    QPushButton *removeTabButton;
    QCustomTabBar *tabBar;
    QCustomTextBrowser *textBrowser;
public slots:
    void addTab();
    void removeTab();
    void removeTab(int i);
private slots:
    void copy();
    void paste();
    void showContextMenu();
};

#endif // QCUSTOMTABWIDGET_H
