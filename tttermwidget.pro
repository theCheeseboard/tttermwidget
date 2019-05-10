#-------------------------------------------------
#
# Project created by QtCreator 2019-05-10T15:04:03
#
#-------------------------------------------------

QT       += widgets multimedia
CONFIG   += c++14

TARGET = tttermwidget
TEMPLATE = lib

DEFINES += TTTERMWIDGET_LIBRARY

system("pkg-config --version") {
    CONFIG += link_pkgconfig
    system("pkg-config --exists libutf8proc") {
        message("Building with Utf8Proc support");
        PKGCONFIG += libutf8proc
        DEFINES += HAVE_UTF8PROC
    } else {
        message("Utf8Proc not found on this system.");
    }
}

macx {
    DEFINES += HAVE_UTMPX UTMPX_COMPAT
}

KB_LAYOUT_DIR = /usr/share/tttermwidget/kb-layouts
COLORSCHEMES_DIR = /usr/share/tttermwidget/color-schemes
TRANSLATIONS_DIR = /usr/share/tttermwidget/translations

DEFINES += KB_LAYOUT_DIR="\\\"$$KB_LAYOUT_DIR\\\""
DEFINES += COLORSCHEMES_DIR="\\\"$$COLORSCHEMES_DIR\\\""
DEFINES += TRANSLATIONS_DIR="\\\"$$TRANSLATIONS_DIR\\\""
DEFINES += HAVE_POSIX_OPENPT HAVE_SYS_TIME_H QT_NO_FOREACH

FORMS += \
    lib/SearchBar.ui

HEADERS += \
    lib/Emulation.h \
    lib/Filter.h \
    lib/HistorySearch.h \
    lib/Pty.h \
    lib/ScreenWindow.h \
    lib/SearchBar.h \
    lib/Session.h \
    lib/TerminalDisplay.h \
    lib/Vt102Emulation.h \
    lib/kprocess.h \
    lib/kptydevice.h \
    lib/kptyprocess.h \
    lib/tttermwidget.h

SOURCES += \
    lib/BlockArray.cpp \
    lib/ColorScheme.cpp \
    lib/Emulation.cpp \
    lib/Filter.cpp \
    lib/History.cpp \
    lib/HistorySearch.cpp \
    lib/KeyboardTranslator.cpp \
    lib/Pty.cpp \
    lib/Screen.cpp \
    lib/ScreenWindow.cpp \
    lib/SearchBar.cpp \
    lib/Session.cpp \
    lib/ShellCommand.cpp \
    lib/TerminalCharacterDecoder.cpp \
    lib/TerminalDisplay.cpp \
    lib/Vt102Emulation.cpp \
    lib/konsole_wcwidth.cpp \
    lib/kprocess.cpp \
    lib/kpty.cpp \
    lib/kptydevice.cpp \
    lib/kptyprocess.cpp \
    lib/tools.cpp \
    lib/tttermwidget.cpp

unix:!macx {
    target.path = /usr/lib

    translations.path = $$TRANSLATIONS_DIR
    translations.files = translations/*

    colorschemes.path = $$COLORSCHEMES_DIR
    colorschemes.files = lib/color-schemes/*

    kblayouts.path = $$KB_LAYOUT_DIR
    kblayouts.files = lib/kb-layouts/*

    header.path = /usr/include/tttermwidget
    header.files = lib/tttermwidget.h lib/Emulation.h lib/Filter.h

    module.files = qt_thelib.pri
    module.path = $$[QMAKE_MKSPECS]/modules

    INSTALLS = target translations colorschemes kblayouts header module
}

DISTFILES += \
    qt_tttermwidget.pri
