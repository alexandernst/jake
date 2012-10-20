#ifndef QCUSTOMTABBAR_H
#define QCUSTOMTABBAR_H

#include <QMenu>
#include <QDebug>
#include <QTabBar>
#include <QMouseEvent>
#include <QColorDialog>
#include "qcustomtabbarlineedit.h"

class QCustomTabBar : public QTabBar
{
    Q_OBJECT
public:
    QCustomTabBar(QWidget *parent = 0);
protected:
    void mouseDoubleClickEvent(QMouseEvent *event);
private:
    QMenu *contextmenu;
    QPoint eventpoint;
    QCustomTabBarLineEdit tabbarlineedit;
public slots:
    void closeTab();
    void renameTab();
    void showContextMenu(const QPoint &point);
private slots:
    void setCurrentTabText(const QString &text);
};

#endif // QCUSTOMTABBAR_H
