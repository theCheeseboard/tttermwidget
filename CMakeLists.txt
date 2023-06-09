cmake_minimum_required(VERSION 3.4.0)

project(tttermwidget VERSION 1.0.0 LANGUAGES CXX)

find_package(QT REQUIRED NAMES Qt6)

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets DBus Core5Compat)
find_package(libcontemporary)
cntp_find_pkgconfig(Utf8Proc libutf8proc)

set(SOURCES
    lib/BlockArray.cpp
    lib/ColorScheme.cpp
    lib/Emulation.cpp
    lib/Filter.cpp
    lib/History.cpp
    lib/HistorySearch.cpp
    lib/KeyboardTranslator.cpp
    lib/Pty.cpp
    lib/Screen.cpp
    lib/ScreenWindow.cpp
    lib/SearchBar.cpp
    lib/Session.cpp
    lib/ShellCommand.cpp
    lib/TerminalCharacterDecoder.cpp
    lib/TerminalDisplay.cpp
    lib/Vt102Emulation.cpp
    lib/konsole_wcwidth.cpp
    lib/kprocess.cpp
    lib/kpty.cpp
    lib/kptydevice.cpp
    lib/kptyprocess.cpp
    lib/tools.cpp
    lib/tttermwidget.cpp

    lib/SearchBar.ui)


set(HEADERS
    lib/Emulation.h
    lib/Filter.h
    lib/HistorySearch.h
    lib/kprocess.h
    lib/kptydevice.h
    lib/kptyprocess.h
    lib/Pty.h
    lib/ScreenWindow.h
    lib/SearchBar.h
    lib/Session.h
    lib/TerminalDisplay.h
    lib/Vt102Emulation.h)

set(PUBLIC_HEADERS
    lib/tttermwidget.h
    lib/Emulation.h
    lib/Filter.h
    lib/KeyboardTranslator.h
    lib/tttermwidget_export.h)

file(GLOB KBLAYOUTS lib/kb-layouts/*)
file(GLOB COLORSCHEMES lib/color-schemes/*)

add_library(tttermwidget SHARED ${SOURCES} ${HEADERS} ${PUBLIC_HEADERS} ${KBLAYOUTS} ${COLORSCHEMES})

cntp_init(tttermwidget 20)
cntp_target_public_headers(tttermwidget
    DESTINATION_DIRECTORY tttermwidget
    HEADERS ${PUBLIC_HEADERS})
set_target_properties(tttermwidget PROPERTIES
        OUTPUT_NAME tttermwidget
        FRAMEWORK TRUE
        MACOSX_FRAMEWORK_IDENTIFIER com.vicr123.tttermwidget
        VERSION 1.0.0)

target_link_libraries(tttermwidget Qt::Widgets Qt::DBus Qt::Core5Compat libcontemporary)

IF(${Utf8Proc_FOUND})
    target_link_libraries(tttermwidget PkgConfig::Utf8Proc)
    target_compile_definitions(tttermwidget PRIVATE HAVE_UTF8PROC)
ENDIF()

target_compile_definitions(tttermwidget PRIVATE
    HAVE_POSIX_OPENPT
    HAVE_SYS_TIME_H
    QT_NO_FOREACH
    tttermwidget_EXPORTS
)

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(KB_LAYOUT_DIR ${CMAKE_INSTALL_FULL_DATADIR}/tttermwidget/kb-layouts)
    set(COLORSCHEMES_DIR ${CMAKE_INSTALL_FULL_DATADIR}/tttermwidget/color-schemes)

    target_compile_definitions(tttermwidget PRIVATE
        KB_LAYOUT_DIR="${KB_LAYOUT_DIR}"
        COLORSCHEMES_DIR="${COLORSCHEMES_DIR}"
    )

    install(DIRECTORY lib/kb-layouts/
        DESTINATION ${KB_LAYOUT_DIR})
    install(DIRECTORY lib/color-schemes/
        DESTINATION ${COLORSCHEMES_DIR})
endif()

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    target_compile_definitions(tttermwidget PRIVATE HAVE_UTMPX UTMPX_COMPAT)

    foreach(file ${KBLAYOUTS})
        set_source_files_properties(${file} PROPERTIES
            MACOSX_PACKAGE_LOCATION Resources/kb-layouts)
    endforeach()
    foreach(file ${COLORSCHEMES})
        set_source_files_properties(${file} PROPERTIES
            MACOSX_PACKAGE_LOCATION Resources/color-schemes)
    endforeach()
endif()

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(CMAKECONFIG_INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/tttermwidget.framework/Resources/CMake)
ELSE()
    set(CMAKECONFIG_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR}/cmake/tttermwidget)
ENDIF()


configure_package_config_file(tttermwidgetConfig.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/tttermwidgetConfig.cmake
        INSTALL_DESTINATION ${CMAKECONFIG_INSTALL_DIR})

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/tttermwidgetConfig.cmake
        DESTINATION ${CMAKECONFIG_INSTALL_DIR})
install(EXPORT tttermwidgetTargets
        FILE tttermwidgetTargets.cmake
        DESTINATION ${CMAKECONFIG_INSTALL_DIR})

install(TARGETS tttermwidget
        EXPORT tttermwidgetTargets
        INCLUDES DESTINATION ${tttermwidget_INCLUDE_DIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR})
