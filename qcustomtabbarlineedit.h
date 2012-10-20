#ifndef QCUSTOMTABBARLINEEDIT_H
#define QCUSTOMTABBARLINEEDIT_H

#include <QLineEdit>
#include <QKeyEvent>
#include <QFocusEvent>

class QCustomTabBarLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    QCustomTabBarLineEdit(QWidget *parent = 0);
    QCustomTabBarLineEdit(const QString &contents, QWidget *parent = 0 );
public slots:
    void setText(const QString &text);
signals:
    void newTextEntered(const QString &text);
protected:
    void keyPressEvent(QKeyEvent *event);
    void focusOutEvent(QFocusEvent *event);
private slots:
    void publishText();
    void setupWidget();
};

#endif // QCUSTOMTABBARLINEEDIT_H
