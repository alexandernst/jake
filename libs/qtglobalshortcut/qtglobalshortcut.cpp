#include "qtglobalshortcut.h"
#include "qtglobalshortcut_p.h"

bool QtGlobalShortcutPrivate::error = false;
int QtGlobalShortcutPrivate::ref = 0;
QAbstractEventDispatcher::EventFilter QtGlobalShortcutPrivate::prevEventFilter = 0;
QHash<QPair<quint32, quint32>, QtGlobalShortcut*> QtGlobalShortcutPrivate::shortcuts;

QtGlobalShortcutPrivate::QtGlobalShortcutPrivate() : enabled(true), key(Qt::Key(0)), mods(Qt::NoModifier)
{
    if (!ref++)
        prevEventFilter = QAbstractEventDispatcher::instance()->setEventFilter(eventFilter);
}

QtGlobalShortcutPrivate::~QtGlobalShortcutPrivate()
{
    if (!--ref)
        QAbstractEventDispatcher::instance()->setEventFilter(prevEventFilter);
}

bool QtGlobalShortcutPrivate::setShortcut(const QKeySequence& shortcut)
{
    Qt::KeyboardModifiers allMods = Qt::ShiftModifier | Qt::ControlModifier | Qt::AltModifier | Qt::MetaModifier;
    key = shortcut.isEmpty() ? Qt::Key(0) : Qt::Key((shortcut[0] ^ allMods) & shortcut[0]);
    mods = shortcut.isEmpty() ? Qt::KeyboardModifiers(0) : Qt::KeyboardModifiers(shortcut[0] & allMods);
    const quint32 nativeKey = nativeKeycode(key);
    const quint32 nativeMods = nativeModifiers(mods);
    const bool res = registerShortcut(nativeKey, nativeMods);
    shortcuts.insert(qMakePair(nativeKey, nativeMods), &qxt_p());
    if (!res)
        qWarning() << "QtGlobalShortcut failed to register:" << QKeySequence(key + mods).toString();
    return res;
}

bool QtGlobalShortcutPrivate::unsetShortcut()
{
    const quint32 nativeKey = nativeKeycode(key);
    const quint32 nativeMods = nativeModifiers(mods);
    const bool res = unregisterShortcut(nativeKey, nativeMods);
    shortcuts.remove(qMakePair(nativeKey, nativeMods));
    if (!res)
        qWarning() << "QtGlobalShortcut failed to unregister:" << QKeySequence(key + mods).toString();
    key = Qt::Key(0);
    mods = Qt::KeyboardModifiers(0);
    return res;
}

void QtGlobalShortcutPrivate::activateShortcut(quint32 nativeKey, quint32 nativeMods)
{
    QtGlobalShortcut* shortcut = shortcuts.value(qMakePair(nativeKey, nativeMods));
    if (shortcut && shortcut->isEnabled())
        emit shortcut->activated();
}

QtGlobalShortcut::QtGlobalShortcut(QObject* parent)
    : QObject(parent)
{
    QXT_INIT_PRIVATE(QtGlobalShortcut);
}

QtGlobalShortcut::QtGlobalShortcut(const QKeySequence& shortcut, QObject* parent)
    : QObject(parent)
{
    QXT_INIT_PRIVATE(QtGlobalShortcut);
    setShortcut(shortcut);
}

QtGlobalShortcut::~QtGlobalShortcut()
{
    if (qxt_d().key != 0)
        qxt_d().unsetShortcut();
}

QKeySequence QtGlobalShortcut::shortcut() const
{
    return QKeySequence(qxt_d().key | qxt_d().mods);
}

bool QtGlobalShortcut::setShortcut(const QKeySequence& shortcut)
{
    if (qxt_d().key != 0)
        qxt_d().unsetShortcut();
    return qxt_d().setShortcut(shortcut);
}

bool QtGlobalShortcut::isEnabled() const
{
    return qxt_d().enabled;
}

void QtGlobalShortcut::setEnabled(bool enabled)
{
    qxt_d().enabled = enabled;
}

void QtGlobalShortcut::setDisabled(bool disabled)
{
    qxt_d().enabled = !disabled;
}
