#include "qcustomtabwidget.h"
#include "qcustomtextbrowser.h"

QCustomTabWidget::QCustomTabWidget(QWidget *parent) :
    QTabWidget(parent)
{
    //Set object parent and name.
    setParent(parent);
    setObjectName("mainTabWidget");
    setContextMenuPolicy(Qt::CustomContextMenu);
    setGeometry(0, 0, parent->width(), parent->height());

    //Open settings and go to main app group.
    settings = new QSettings("alexandernst", "jake++");
    settings->beginGroup("jake++");
    tabnameprefix = settings->value("tabnameprefix", "jake++").toString();
    settings->endGroup();
    settings->deleteLater();

    //Create add and remove tabs buttons.
    addTabButton = new QPushButton("+");
    addTabButton->setObjectName("addTabButton");
    setCornerWidget(addTabButton, Qt::TopRightCorner);
    removeTabButton = new QPushButton("-");
    removeTabButton->setObjectName("removeTabButton");
    setCornerWidget(removeTabButton, Qt::TopLeftCorner);

    //Clipboard
    clipboard = QApplication::clipboard();

    //Create context menu.
    contextmenu =  new QMenu(this);
    contextmenu->addAction(tr("Copy"), this, SLOT(copy()));
    contextmenu->addAction(tr("Paste"), this, SLOT(paste()));

    //Create a custom tabbar.
    tabBar = new QCustomTabBar();
    setTabBar(tabBar);
    setMovable(true);
    setTabsClosable(true);

    //Add one text area at startup
    addTab();

    //Connect signals
    QObject::connect(addTabButton, SIGNAL(clicked()), this, SLOT(addTab()));
    QObject::connect(removeTabButton, SIGNAL(clicked()), this, SLOT(removeTab()));
    QObject::connect(tabBar, SIGNAL(tabCloseRequested(int)), this, SLOT(removeTab(int)));
}

QCustomTabWidget::~QCustomTabWidget()
{
    addTabButton->deleteLater();
    removeTabButton->deleteLater();
    tabBar->deleteLater();
    contextmenu->deleteLater();
}

void QCustomTabWidget::addTab()
{
    textBrowser = new QCustomTextBrowser(this);
    insertTab(count(), textBrowser, tabnameprefix);
    QObject::connect(textBrowser, SIGNAL(showContextMenu()), this, SLOT(showContextMenu()));
}

void QCustomTabWidget::removeTab()
{
    if(count()>1){
        delete widget(currentIndex());
    }
}

void QCustomTabWidget::removeTab(int i)
{
    if(count()>1){
        delete widget(i);
    }
}

void QCustomTabWidget::showContextMenu()
{
    contextmenu->exec(QCursor::pos());
}

void QCustomTabWidget::copy()
{
    clipboard->setText(static_cast<QCustomTextBrowser*>(currentWidget())->textCursor().selectedText());
}

void QCustomTabWidget::paste()
{
    static_cast<QCustomTextBrowser*>(currentWidget())->insert(clipboard->text(), false);
}
