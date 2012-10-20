#include "qcustomtabbar.h"

QCustomTabBar::QCustomTabBar(QWidget *parent) :
    QTabBar(parent), tabbarlineedit("", 0)
{
    setObjectName("mainTabBarWidget");
    setContextMenuPolicy(Qt::CustomContextMenu);
    setSelectionBehaviorOnRemove(QTabBar::SelectPreviousTab);

    //Create context menu
    contextmenu =  new QMenu();
    contextmenu->addAction(tr("Close"), this, SLOT(closeTab()));
    contextmenu->addAction(tr("Rename"), this, SLOT(renameTab()));

    //Connect signals
    QObject::connect(this, SIGNAL(customContextMenuRequested(const QPoint)), SLOT(showContextMenu(const QPoint &)));
    QObject::connect(&tabbarlineedit, SIGNAL(newTextEntered(const QString &)), this, SLOT(setCurrentTabText(const QString &)));
}

void QCustomTabBar::showContextMenu(const QPoint &point)
{
    eventpoint = point;
    contextmenu->exec(QCursor::pos());
}

void QCustomTabBar::renameTab()
{
    tabbarlineedit.setText(tabText(tabAt(eventpoint)));
    tabbarlineedit.move(mapToGlobal(eventpoint));
}

void QCustomTabBar::closeTab()
{
    emit tabCloseRequested(tabAt(eventpoint));
}

void QCustomTabBar::mouseDoubleClickEvent(QMouseEvent *event)
{
    tabbarlineedit.setText(tabText(tabAt(event->pos())));
    tabbarlineedit.move(mapToGlobal(event->pos()));
}

void QCustomTabBar::setCurrentTabText(const QString &text)
{
    setTabText(tabAt(eventpoint), text);
}
