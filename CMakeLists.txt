cmake_minimum_required(VERSION 3.4.0)

project(tttermwidget VERSION 1.0.0 LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Widgets DBus)
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

set(PUBLIC_HEADERS
    lib/tttermwidget.h
    lib/Emulation.h
    lib/Filter.h
    lib/KeyboardTranslator.h)

add_library(tttermwidget SHARED ${SOURCES})
cntp_init(tttermwidget 17)
cntp_target_public_headers(tttermwidget
    DESTINATION_DIRECTORY tttermwidget
    HEADERS ${PUBLIC_HEADERS})
set_target_properties(tttermwidget PROPERTIES
        OUTPUT_NAME thefrisbee
        FRAMEWORK TRUE
        MACOSX_FRAMEWORK_IDENTIFIER com.vicr123.tttermwidget
        VERSION 1.0.0)

target_link_libraries(tttermwidget Qt6::Widgets Qt6::DBus libcontemporary)

IF(${Utf8Proc_FOUND})
    target_link_libraries(tttermwidget PkgConfig::Utf8Proc)
    target_compile_definitions(tttermwidget PRIVATE HAVE_UTF8PROC)
ENDIF()

target_compile_definitions(tttermwidget PRIVATE HAVE_POSIX_OPENPT HAVE_SYS_TIME_H QT_NO_FOREACH)

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

install(FILES com.vicr123.trigger-uevent.policy
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/polkit-1/actions)
install(FILES trigger-uevent.sh
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/tttermwidget)

install(TARGETS tttermwidget
        EXPORT tttermwidgetTargets
        INCLUDES DESTINATION ${tttermwidget_INCLUDE_DIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR})
