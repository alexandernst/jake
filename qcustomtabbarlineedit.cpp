#include "qcustomtabbarlineedit.h"

QCustomTabBarLineEdit::QCustomTabBarLineEdit(QWidget *parent) :
    QLineEdit(parent)
{
    setupWidget();
}

QCustomTabBarLineEdit::QCustomTabBarLineEdit(const QString &contents, QWidget *parent) :
    QLineEdit(contents, parent)
{
    setupWidget();
}

void QCustomTabBarLineEdit::setupWidget()
{
    connect(this, SIGNAL(editingFinished()), this, SLOT(publishText()));
    setWindowFlags(Qt::CustomizeWindowHint | Qt::Popup);
}

void QCustomTabBarLineEdit::setText(const QString &text)
{
    QLineEdit::setText(text);
    setFocus();
    selectAll();
    show();
}

void QCustomTabBarLineEdit::keyPressEvent(QKeyEvent *event)
{
    QLineEdit::keyPressEvent(event);
    if(event->key() == Qt::Key_Escape){
        setText("");
        hide();
    }
}

void QCustomTabBarLineEdit::focusOutEvent(QFocusEvent *event)
{
    QLineEdit::focusOutEvent(event);
    emit editingFinished();
}

void QCustomTabBarLineEdit::publishText()
{
    if(text() != ""){
        emit newTextEntered(text());
    }
    hide();
}
