TEMPLATE = app

CONFIG += qtestlib

QT += network

SOURCES += \
    main.cpp \
    qconsole.cpp \
    qcustomplugin.cpp \
    qcustomtabbar.cpp \
    qcustomtabwidget.cpp \
    qcustomtextbrowser.cpp \
    libs/qtnam/syncdown.cpp \
    libs/qtnam/asyncdown.cpp \
    qcustompluginmanager.cpp \
    qcustomtabbarlineedit.cpp \
    libs/qtglobalshortcut/qtglobal.cpp \
    plugins/test/qcustomplugintest.cpp \
    libs/qtglobalshortcut/qtglobalshortcut.cpp \
    libs/qtsingleapplication/qtsingleapplication.cpp \
    libs/qtsingleapplication/qtlocalpeer.cpp \
    libs/qtsingleapplication/qtlockedfile.cpp

macx{
  SOURCES += libs/qtglobalshortcut/qtglobalshortcut_mac.cpp
  LIBS += -framework CoreFoundation

}

unix{
  SOURCES += libs/qtsingleapplication/qtlockedfile_unix.cpp
}

unix:!mac{
  SOURCES += libs/qtglobalshortcut/qtglobalshortcut_x11.cpp
  CONFIG  += x11
}

win32{
  SOURCES += \
    libs/qtglobalshortcut/qtglobalshortcut_win.cpp \
    libs/qtsingleapplication/qtlockedfile_win.cpp
  contains(TEMPLATE, lib):contains(CONFIG, shared):DEFINES += QT_QTSINGLEAPPLICATION_EXPORT
  else:qtsingleapplication-uselib:DEFINES += QT_QTSINGLEAPPLICATION_IMPORT
}

HEADERS += \
    qconsole.h \
    qcustomtabbar.h \
    qcustomplugin.h \
    qcustomtabwidget.h \
    qcustomtextbrowser.h \
    libs/qtnam/syncdown.h \
    libs/qtnam/asyncdown.h \
    qcustompluginmanager.h \
    qcustomtabbarlineedit.h \
    plugins/test/qcustomplugintest.h \
    libs/qtglobalshortcut/qtglobal.h \
    libs/qtglobalshortcut/qtglobalshortcut.h \
    libs/qtglobalshortcut/qtglobalshortcut_p.h \
    libs/qtsingleapplication/qtsingleapplication.h \
    libs/qtsingleapplication/qtlocalpeer.h \
    libs/qtsingleapplication/qtlockedfile.h

RESOURCES += \
    resources.qrc
