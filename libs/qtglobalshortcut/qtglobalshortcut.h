#ifndef QTGLOBALSHORTCUT_H
#define QTGLOBALSHORTCUT_H

#include "qtglobal.h"

class QtGlobalShortcutPrivate;

class QtGlobalShortcut : public QObject
{
    Q_OBJECT
    QXT_DECLARE_PRIVATE(QtGlobalShortcut)
    Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled)
    Q_PROPERTY(QKeySequence shortcut READ shortcut WRITE setShortcut)

public:
    explicit QtGlobalShortcut(QObject* parent = 0);
    explicit QtGlobalShortcut(const QKeySequence& shortcut, QObject* parent = 0);
    virtual ~QtGlobalShortcut();

    QKeySequence shortcut() const;
    bool setShortcut(const QKeySequence& shortcut);

    bool isEnabled() const;

public Q_SLOTS:
    void setEnabled(bool enabled = true);
    void setDisabled(bool disabled = true);

Q_SIGNALS:
    void activated();
};

#endif // QTGLOBALSHORTCUT_H
