# tttermwidget
Terminal Emulator used in theTerminal

tttermwidget is derived from the work of [QTermWidget](https://github.com/lxqt/qtermwidget).

## Dependencies
- [libcontemporary](https://github.com/theCheeseboard/libcontemporary)
This project is licensed under the terms of the [GPLv2](https://www.gnu.org/licenses/gpl-2.0.en.html) or any later version. See the LICENSE file for the full text of the license. Some files are published under compatible licenses:

## Build
```
cmake -B build -S .
cmake --build build
```

## Install
```
cmake --install build
```

## Bugs/Feature Requests
- Report any bugs using the "Issues" tab up there. Alternatively, click [here](https://github.com/theCheeseboard/tttermwidget/issues) to jump straight there. (Thanks!)
- If you want to request a feature, you can also use the "Issues" tab.

## Credits
```
Files: example/main.cpp
       lib/TerminalCharacterDecoder.cpp
       lib/TerminalCharacterDecoder.h
       lib/kprocess.cpp
       lib/kprocess.h
       lib/kpty.cpp
       lib/kpty.h
       lib/kpty_p.h
       lib/kptydevice.cpp
       lib/kptydevice.h
       lib/kptyprocess.cpp
       lib/kptyprocess.h
       lib/qtermwidget.cpp
       lib/qtermwidget.h
Copyright: Author Adriaan de Groot <groot@kde.org>
           2010, KDE e.V <kde-ev-board@kde.org>
           2002-2007, Oswald Buddenhagen <ossi@kde.org>
           2006-2008, Robert Knight <robertknight@gmail.com>
           2002, Waldo Bastian <bastian@kde.org>
           2008, e_k <e_k@users.sourceforge.net>
License: LGPL-2+

Files: cmake/FindUtf8Proc.cmake
Copyright: 2009-2011, Kitware, Inc
           2009-2011, Philip Lowman <philip@yhbt.com>
License: BSD-3-clause
```
