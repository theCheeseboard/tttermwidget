#include "tools.h"

#include <QCoreApplication>
#include <QDir>
#include <QtDebug>
#include <tapplication.h>

#ifdef Q_OS_MAC
QString getBundleLocation(QString macDir) {
    return tApplication::macOSBundlePath() + macDir;
}
#endif

Q_LOGGING_CATEGORY(qtermwidgetLogger, "qtermwidget", QtWarningMsg)

/*! Helper function to get possible location of layout files.
By default the KB_LAYOUT_DIR is used (linux/BSD/macports).
But in some cases (apple bundle) there can be more locations).
*/
QString get_kb_layout_dir() {
    //    qDebug() << __FILE__ << __FUNCTION__;

    QString rval = QString();
    QString k;
#ifdef Q_OS_MAC
    // Find in the .app bundle
    k = getBundleLocation(QLatin1String(KB_LAYOUT_DIR));
#else
    k = KB_LAYOUT_DIR;
#endif
    QDir d(k);

    // qDebug() << "default KB_LAYOUT_DIR: " << k;

    if (d.exists()) {
        rval = k.append(QLatin1Char('/'));
        return rval;
    }

    // Install location
    d.setPath("/usr/share/tttermwidget/kb-layouts/");
    if (d.exists()) return d.path() + "/";
    // AppDir
    d.setPath(QDir::cleanPath(QApplication::applicationDirPath() + "/../share/tttermwidget/kb-layouts/"));
    if (d.exists()) return d.path() + "/";
    // subdir in the app location
    d.setPath(QCoreApplication::applicationDirPath() + "/kb-layouts/");
    if (d.exists()) return d.path() + "/";
#ifdef Q_WS_MAC
    d.setPath(QCoreApplication::applicationDirPath() + "/../Resources/kb-layouts/");
    if (d.exists())
        return QCoreApplication::applicationDirPath() + QLatin1String("/../Resources/kb-layouts/");
#endif
    // qDebug() << "Cannot find KB_LAYOUT_DIR. Default:" << k;
    return QString();
}

/*! Helper function to add custom location of color schemes.
 */
namespace {
    QStringList custom_color_schemes_dirs;
}
void add_custom_color_scheme_dir(const QString& custom_dir) {
    if (!custom_color_schemes_dirs.contains(custom_dir))
        custom_color_schemes_dirs << custom_dir;
}

/*! Helper function to get possible locations of color schemes.
By default the COLORSCHEMES_DIR is used (linux/BSD/macports).
But in some cases (apple bundle) there can be more locations).
*/
const QStringList get_color_schemes_dirs() {
    //    qDebug() << __FILE__ << __FUNCTION__;

    QStringList rval;
    QString k;
#ifdef Q_OS_MAC
    // Find in the .app bundle
    k = getBundleLocation(QLatin1String(COLORSCHEMES_DIR));
#else
    k = COLORSCHEMES_DIR;
#endif
    QDir d(k);

    //    qDebug() << "default COLORSCHEMES_DIR: " << k;

    if (d.exists())
        rval << k.append(QLatin1Char('/'));

    // Install location
    d.setPath("/usr/share/tttermwidget/color-schemes/");
    if (d.exists()) rval << d.path() + "/";
    // AppDir
    d.setPath(QDir::cleanPath(QApplication::applicationDirPath() + "/../share/tttermwidget/color-schemes/"));
    if (d.exists()) rval << d.path() + "/";
    // subdir in the app location
    d.setPath(QCoreApplication::applicationDirPath() + "/color-schemes/");
    if (d.exists()) rval << d.path() + "/";

    rval.removeDuplicates();

#ifdef Q_WS_MAC
    d.setPath(QCoreApplication::applicationDirPath() + "/../Resources/color-schemes/");
    if (d.exists()) {
        if (!rval.isEmpty())
            rval.clear();
        rval << (QCoreApplication::applicationDirPath() + QLatin1String("/../Resources/color-schemes/"));
    }
#endif

    for (const QString& custom_dir : qAsConst(custom_color_schemes_dirs)) {
        d.setPath(custom_dir);
        if (d.exists())
            rval << custom_dir;
    }
#ifdef QT_DEBUG
    if (!rval.isEmpty()) {
        qDebug() << "Using color-schemes: " << rval;
    } else {
        qDebug() << "Cannot find color-schemes in any location!";
    }
#endif
    return rval;
}
