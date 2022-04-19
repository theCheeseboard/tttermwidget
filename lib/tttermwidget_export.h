
#ifndef TTTERMWIDGET_EXPORT_H
#define TTTERMWIDGET_EXPORT_H

#ifdef TTTERMWIDGET_STATIC_DEFINE
#  define TTTERMWIDGET_EXPORT
#  define TTTERMWIDGET_NO_EXPORT
#else
#  ifndef TTTERMWIDGET_EXPORT
#    ifdef tttermwidget_EXPORTS
        /* We are building this library */
#      define TTTERMWIDGET_EXPORT __attribute__((visibility("default")))
#    else
        /* We are using this library */
#      define TTTERMWIDGET_EXPORT __attribute__((visibility("default")))
#    endif
#  endif

#  ifndef TTTERMWIDGET_NO_EXPORT
#    define TTTERMWIDGET_NO_EXPORT __attribute__((visibility("hidden")))
#  endif
#endif

#ifndef TTTERMWIDGET_DEPRECATED
#  define TTTERMWIDGET_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef TTTERMWIDGET_DEPRECATED_EXPORT
#  define TTTERMWIDGET_DEPRECATED_EXPORT TTTERMWIDGET_EXPORT TTTERMWIDGET_DEPRECATED
#endif

#ifndef TTTERMWIDGET_DEPRECATED_NO_EXPORT
#  define TTTERMWIDGET_DEPRECATED_NO_EXPORT TTTERMWIDGET_NO_EXPORT TTTERMWIDGET_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef TTTERMWIDGET_NO_DEPRECATED
#    define TTTERMWIDGET_NO_DEPRECATED
#  endif
#endif

#endif /* TTTERMWIDGET_EXPORT_H */
