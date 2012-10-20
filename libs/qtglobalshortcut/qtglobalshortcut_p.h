#ifndef QTGLOBALSHORTCUT_P_H
#define QTGLOBALSHORTCUT_P_H

#include "qtglobalshortcut.h"

#ifdef Q_WS_WIN
#include <qt_windows.h>
#endif // Q_WS_WIN

#ifdef Q_WS_X11
#include <QX11Info>
#include <X11/Xlib.h>
#endif // Q_WS_X11

#ifdef Q_WS_MAC
#include <Carbon/Carbon.h>
#endif

class QtGlobalShortcutPrivate : public QxtPrivate<QtGlobalShortcut>
{
public:
    QXT_DECLARE_PUBLIC(QtGlobalShortcut)
    QtGlobalShortcutPrivate();
    ~QtGlobalShortcutPrivate();

    bool enabled;
    Qt::Key key;
    Qt::KeyboardModifiers mods;

    bool setShortcut(const QKeySequence& shortcut);
    bool unsetShortcut();

    static bool error;
    static int ref;
    static QAbstractEventDispatcher::EventFilter prevEventFilter;
    static bool eventFilter(void* message);

private:
    static quint32 nativeKeycode(Qt::Key keycode);
    static quint32 nativeModifiers(Qt::KeyboardModifiers modifiers);

    static bool registerShortcut(quint32 nativeKey, quint32 nativeMods);
    static bool unregisterShortcut(quint32 nativeKey, quint32 nativeMods);
    static void activateShortcut(quint32 nativeKey, quint32 nativeMods);

    static QHash<QPair<quint32, quint32>, QtGlobalShortcut*> shortcuts;
};

#endif // QTGLOBALSHORTCUT_P_H
