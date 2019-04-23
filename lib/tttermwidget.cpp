/*  Copyright (C) 2008 e_k (e_k@users.sourceforge.net)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include <QLayout>
#include <QBoxLayout>
#include <QtDebug>
#include <QDir>
#include <QMessageBox>
#include <signal.h>

#include "ColorTables.h"
#include "Session.h"
#include "Screen.h"
#include "ScreenWindow.h"
#include "Emulation.h"
#include "TerminalDisplay.h"
#include "KeyboardTranslator.h"
#include "ColorScheme.h"
#include "SearchBar.h"
#include "tttermwidget.h"


#define STEP_ZOOM 1

using namespace Konsole;

void *createTermWidget(int startnow, void *parent)
{
    return (void*) new TTTermWidget(startnow, (QWidget*)parent);
}

struct TermWidgetImpl {
    TermWidgetImpl(bool connectPtyData, QWidget* parent = 0);

    TerminalDisplay *m_terminalDisplay;
    Session *m_session;

    Session* createSession(bool connectPtyData, QWidget* parent);
    TerminalDisplay* createTerminalDisplay(Session *session, QWidget* parent);
};

TermWidgetImpl::TermWidgetImpl(bool connectPtyData, QWidget* parent)
{
    this->m_session = createSession(connectPtyData, parent);
    this->m_terminalDisplay = createTerminalDisplay(this->m_session, parent);
}


Session *TermWidgetImpl::createSession(bool connectPtyData, QWidget* parent)
{
    Session *session = new Session(connectPtyData, parent);

    session->setTitle(Session::NameRole, "TTTermWidget");

    /* Thats a freaking bad idea!!!!
     * /bin/bash is not there on every system
     * better set it to the current $SHELL
     * Maybe you can also make a list available and then let the widget-owner decide what to use.
     * By setting it to $SHELL right away we actually make the first filecheck obsolete.
     * But as iam not sure if you want to do anything else ill just let both checks in and set this to $SHELL anyway.
     */
    //session->setProgram("/bin/bash");

    session->setProgram(getenv("SHELL"));



    QStringList args("");
    session->setArguments(args);
    session->setAutoClose(true);

    session->setCodec(QTextCodec::codecForName("UTF-8"));

    session->setFlowControlEnabled(true);
    session->setHistoryType(HistoryTypeBuffer(1000));

    session->setDarkBackground(true);

    session->setKeyBindings("");
    return session;
}

TerminalDisplay *TermWidgetImpl::createTerminalDisplay(Session *session, QWidget* parent)
{
//    TerminalDisplay* display = new TerminalDisplay(this);
    TerminalDisplay* display = new TerminalDisplay(parent);

    display->setBellMode(TerminalDisplay::NotifyBell);
    display->setTerminalSizeHint(true);
    display->setTripleClickMode(TerminalDisplay::SelectWholeLine);
    display->setTerminalSizeStartup(true);

    display->setRandomSeed(session->sessionId() * 31);

    return display;
}


TTTermWidget::TTTermWidget(int startnow, bool connectPtyData, QWidget *parent)
    : QWidget(parent)
{
    init(connectPtyData, startnow);
}

TTTermWidget::TTTermWidget(QWidget *parent)
    : QWidget(parent)
{
    init(true, 1);
}

void TTTermWidget::selectionChanged(bool textSelected)
{
    emit copyAvailable(textSelected);
}

void TTTermWidget::find()
{
    search(true, false);
}

void TTTermWidget::findNext()
{
    search(true, true);
}

void TTTermWidget::findPrevious()
{
    search(false, false);
}

void TTTermWidget::search(bool forwards, bool next)
{
    int startColumn, startLine;

    if (next) // search from just after current selection
    {
        m_impl->m_terminalDisplay->screenWindow()->screen()->getSelectionEnd(startColumn, startLine);
        startColumn++;
    }
    else // search from start of current selection
    {
        m_impl->m_terminalDisplay->screenWindow()->screen()->getSelectionStart(startColumn, startLine);
    }

    qDebug() << "current selection starts at: " << startColumn << startLine;
    qDebug() << "current cursor position: " << m_impl->m_terminalDisplay->screenWindow()->cursorPosition();

    QRegExp regExp(m_searchBar->searchText());
    regExp.setPatternSyntax(m_searchBar->useRegularExpression() ? QRegExp::RegExp : QRegExp::FixedString);
    regExp.setCaseSensitivity(m_searchBar->matchCase() ? Qt::CaseSensitive : Qt::CaseInsensitive);

    HistorySearch *historySearch =
            new HistorySearch(m_impl->m_session->emulation(), regExp, forwards, startColumn, startLine, this);
    connect(historySearch, SIGNAL(matchFound(int, int, int, int)), this, SLOT(matchFound(int, int, int, int)));
    connect(historySearch, SIGNAL(noMatchFound()), this, SLOT(noMatchFound()));
    connect(historySearch, SIGNAL(noMatchFound()), m_searchBar, SLOT(noMatchFound()));
    historySearch->search();
}


void TTTermWidget::matchFound(int startColumn, int startLine, int endColumn, int endLine)
{
    ScreenWindow* sw = m_impl->m_terminalDisplay->screenWindow();
    qDebug() << "Scroll to" << startLine;
    sw->scrollTo(startLine);
    sw->setTrackOutput(false);
    sw->notifyOutputChanged();
    sw->setSelectionStart(startColumn, startLine - sw->currentLine(), false);
    sw->setSelectionEnd(endColumn, endLine - sw->currentLine());
}

void TTTermWidget::noMatchFound()
{
        m_impl->m_terminalDisplay->screenWindow()->clearSelection();
}

int TTTermWidget::getShellPID()
{
    return m_impl->m_session->processId();
}

void TTTermWidget::changeDir(const QString & dir)
{
    /*
       this is a very hackish way of trying to determine if the shell is in
       the foreground before attempting to change the directory.  It may not
       be portable to anything other than Linux.
    */
    QString strCmd;
    strCmd.setNum(getShellPID());
    strCmd.prepend("ps -j ");
    strCmd.append(" | tail -1 | awk '{ print $5 }' | grep -q \\+");
    int retval = system(strCmd.toStdString().c_str());

    if (!retval) {
        QString cmd = "cd " + dir + "\n";
        sendText(cmd);
    }
}

QProcess* TTTermWidget::getProcess() {
    return m_impl->m_session->getProcess();
}

QSize TTTermWidget::sizeHint() const
{
    QSize size = m_impl->m_terminalDisplay->sizeHint();
    size.rheight() = 150;
    return size;
}

void TTTermWidget::setTerminalSizeHint(bool on)
{
    if (!m_impl->m_terminalDisplay)
        return;
    m_impl->m_terminalDisplay->setTerminalSizeHint(on);
}

bool TTTermWidget::terminalSizeHint()
{
    if (!m_impl->m_terminalDisplay)
        return true;
    return m_impl->m_terminalDisplay->terminalSizeHint();
}

void TTTermWidget::startShellProgram()
{
    if ( m_impl->m_session->isRunning() ) {
        return;
    }

    m_impl->m_session->run();
}

void TTTermWidget::startTerminalTeletype()
{
    if ( m_impl->m_session->isRunning() ) {
        return;
    }

    m_impl->m_session->runEmptyPTY();
    // redirect data from TTY to external recipient
    connect( m_impl->m_session->emulation(), SIGNAL(sendData(const char *,int)),
             this, SIGNAL(sendData(const char *,int)) );
}

void TTTermWidget::init(bool connectPtyData, int startnow)
{
    m_layout = new QVBoxLayout();
    m_layout->setMargin(0);
    m_layout->setSpacing(0);
    setLayout(m_layout);

    // translations
    // First check $XDG_DATA_DIRS. This follows the implementation in libqtxdg
    QString d = QFile::decodeName(qgetenv("XDG_DATA_DIRS"));
    QStringList dirs = d.split(QLatin1Char(':'), QString::SkipEmptyParts);
    if (dirs.isEmpty()) {
        dirs.append(QString::fromLatin1("/usr/local/share"));
        dirs.append(QString::fromLatin1("/usr/share"));
    }
    dirs.append(QFile::decodeName(TRANSLATIONS_DIR));

    m_translator = new QTranslator(this);

    for (const QString& dir : dirs) {
        qDebug() << "Trying to load translation file from dir" << dir;
        if (m_translator->load(QLocale::system(), "TTTermWidget", "_", dir)) {
            qApp->installTranslator(m_translator);
            qDebug() << "Translations found in" << dir;
            break;
        }
    }

    m_impl = new TermWidgetImpl(connectPtyData, this);
    m_layout->addWidget(m_impl->m_terminalDisplay);

    connect(m_impl->m_session, SIGNAL(bellRequest(QString)), m_impl->m_terminalDisplay, SLOT(bell(QString)));
    connect(m_impl->m_terminalDisplay, SIGNAL(notifyBell(QString)), this, SIGNAL(bell(QString)));
    connect(m_impl->m_terminalDisplay, SIGNAL(lineCountChanged(int)), this, SIGNAL(lineCountChanged(int)));

    connect(m_impl->m_session, SIGNAL(activity()), this, SIGNAL(activity()));
    connect(m_impl->m_session, SIGNAL(silence()), this, SIGNAL(silence()));
    connect(m_impl->m_session, &Session::profileChangeCommandReceived, this, &TTTermWidget::profileChanged);
    connect(m_impl->m_session, &Session::receivedData, this, &TTTermWidget::receivedData);
    connect(m_impl->m_session, &Session::flowControlEnabledChanged, this, &TTTermWidget::flowControlEnabledChanged);

    // That's OK, FilterChain's dtor takes care of UrlFilter.
    UrlFilter *urlFilter = new UrlFilter();
    connect(urlFilter, &UrlFilter::activated, this, &TTTermWidget::urlActivated);
    m_impl->m_terminalDisplay->filterChain()->addFilter(urlFilter);

    m_searchBar = new SearchBar(this);
    m_searchBar->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
    connect(m_searchBar, SIGNAL(searchCriteriaChanged()), this, SLOT(find()));
    connect(m_searchBar, SIGNAL(findNext()), this, SLOT(findNext()));
    connect(m_searchBar, SIGNAL(findPrevious()), this, SLOT(findPrevious()));
    m_layout->addWidget(m_searchBar);
    m_searchBar->hide();

    if (startnow && m_impl->m_session) {
        m_impl->m_session->run();
    }

    this->setFocus( Qt::OtherFocusReason );
    this->setFocusPolicy( Qt::WheelFocus );
    m_impl->m_terminalDisplay->resize(this->size());

    this->setFocusProxy(m_impl->m_terminalDisplay);
    connect(m_impl->m_terminalDisplay, SIGNAL(copyAvailable(bool)),
            this, SLOT(selectionChanged(bool)));
    connect(m_impl->m_terminalDisplay, SIGNAL(termGetFocus()),
            this, SIGNAL(termGetFocus()));
    connect(m_impl->m_terminalDisplay, SIGNAL(termLostFocus()),
            this, SIGNAL(termLostFocus()));
    connect(m_impl->m_terminalDisplay, SIGNAL(keyPressedSignal(QKeyEvent *)),
            this, SIGNAL(termKeyPressed(QKeyEvent *)));
//    m_impl->m_terminalDisplay->setSize(80, 40);

    QFont font = QApplication::font();
    font.setFamily("Monospace");
    font.setPointSize(10);
    font.setStyleHint(QFont::TypeWriter);
    setTerminalFont(font);
    m_searchBar->setFont(font);

    setScrollBarPosition(NoScrollBar);
    setKeyboardCursorShape(Emulation::KeyboardCursorShape::BlockCursor);

    m_impl->m_session->addView(m_impl->m_terminalDisplay);

    connect(m_impl->m_session, SIGNAL(resizeRequest(QSize)), this, SLOT(setSize(QSize)));
    connect(m_impl->m_session, SIGNAL(finished()), this, SLOT(sessionFinished()));
    connect(m_impl->m_session, SIGNAL(shellProcessDone(int)), this, SIGNAL(shellProgramFinished(int)));
    connect(m_impl->m_session, &Session::titleChanged, this, &TTTermWidget::titleChanged);
    connect(m_impl->m_session, &Session::cursorChanged, this, &TTTermWidget::cursorChanged);
}


TTTermWidget::~TTTermWidget()
{
    delete m_impl;
    emit destroyed();
}


void TTTermWidget::setTerminalFont(const QFont &font)
{
    if (!m_impl->m_terminalDisplay)
        return;
    m_impl->m_terminalDisplay->setVTFont(font);
}

QFont TTTermWidget::getTerminalFont()
{
    if (!m_impl->m_terminalDisplay)
        return QFont();
    return m_impl->m_terminalDisplay->getVTFont();
}

void TTTermWidget::setTerminalOpacity(qreal level)
{
    if (!m_impl->m_terminalDisplay)
        return;

    m_impl->m_terminalDisplay->setOpacity(level);
}

void TTTermWidget::setTerminalBackgroundImage(QString backgroundImage)
{
    if (!m_impl->m_terminalDisplay)
        return;

    m_impl->m_terminalDisplay->setBackgroundImage(backgroundImage);
}

void TTTermWidget::setShellProgram(const QString &progname)
{
    if (!m_impl->m_session)
        return;
    m_impl->m_session->setProgram(progname);
}

void TTTermWidget::setWorkingDirectory(const QString& dir)
{
    if (!m_impl->m_session)
        return;
    m_impl->m_session->setInitialWorkingDirectory(dir);
}

QString TTTermWidget::workingDirectory()
{
    if (!m_impl->m_session)
        return QString();

#ifdef Q_OS_LINUX
    // Christian Surlykke: On linux we could look at /proc/<pid>/cwd which should be a link to current
    // working directory (<pid>: process id of the shell). I don't know about BSD.
    // Maybe we could just offer it when running linux, for a start.
    QDir d(QString("/proc/%1/cwd").arg(getShellPID()));
    if (!d.exists())
    {
        qDebug() << "Cannot find" << d.dirName();
        goto fallback;
    }
    return d.canonicalPath();
#endif

fallback:
    // fallback, initial WD
    return m_impl->m_session->initialWorkingDirectory();
}

void TTTermWidget::setArgs(const QStringList &args)
{
    if (!m_impl->m_session)
        return;
    m_impl->m_session->setArguments(args);
}

void TTTermWidget::setTextCodec(QTextCodec *codec)
{
    if (!m_impl->m_session)
        return;
    m_impl->m_session->setCodec(codec);
}

void TTTermWidget::setColorScheme(const QString& origName)
{
    const ColorScheme *cs = 0;

    const bool isFile = QFile::exists(origName);
    const QString& name = isFile ?
            QFileInfo(origName).baseName() :
            origName;

    // avoid legacy (int) solution
    if (!availableColorSchemes().contains(name))
    {
        if (isFile)
        {
            if (ColorSchemeManager::instance()->loadCustomColorScheme(origName))
                cs = ColorSchemeManager::instance()->findColorScheme(name);
            else
                qWarning () << Q_FUNC_INFO
                        << "cannot load color scheme from"
                        << origName;
        }

        if (!cs)
            cs = ColorSchemeManager::instance()->defaultColorScheme();
    }
    else
        cs = ColorSchemeManager::instance()->findColorScheme(name);

    if (! cs)
    {
        QMessageBox::information(this,
                                 tr("Color Scheme Error"),
                                 tr("Cannot load color scheme: %1").arg(name));
        return;
    }
    ColorEntry table[TABLE_COLORS];
    cs->getColorTable(table);
    m_impl->m_terminalDisplay->setColorTable(table);

    qDebug() << cs->backgroundColor();

    QPalette searchPal = m_searchBar->palette();
    searchPal.setColor(QPalette::Window, cs->backgroundColor());
    searchPal.setColor(QPalette::WindowText, cs->foregroundColor());
    searchPal.setColor(QPalette::Base, cs->backgroundColor());
    searchPal.setColor(QPalette::Text, cs->foregroundColor());
    m_searchBar->setPalette(searchPal);
}

QStringList TTTermWidget::availableColorSchemes()
{
    QStringList ret;
    const auto allColorSchemes = ColorSchemeManager::instance()->allColorSchemes();
    for (const ColorScheme* cs : allColorSchemes)
        ret.append(cs->name());
    return ret;
}

void TTTermWidget::addCustomColorSchemeDir(const QString& custom_dir)
{
    ColorSchemeManager::instance()->addCustomColorSchemeDir(custom_dir);
}

void TTTermWidget::setSize(const QSize &size)
{
    if (!m_impl->m_terminalDisplay)
        return;
    m_impl->m_terminalDisplay->setSize(size.width(), size.height());
}

void TTTermWidget::setHistorySize(int lines)
{
    if (lines < 0)
        m_impl->m_session->setHistoryType(HistoryTypeFile());
    else
        m_impl->m_session->setHistoryType(HistoryTypeBuffer(lines));
}

void TTTermWidget::setScrollBarPosition(ScrollBarPosition pos)
{
    if (!m_impl->m_terminalDisplay)
        return;
    m_impl->m_terminalDisplay->setScrollBarPosition(pos);
}

void TTTermWidget::scrollToEnd()
{
    if (!m_impl->m_terminalDisplay)
        return;
    m_impl->m_terminalDisplay->scrollToEnd();
}

void TTTermWidget::setScrollOnKeypress(bool scrollOnKeypress) {
    if (!m_impl->m_terminalDisplay)
        return;
    m_impl->m_terminalDisplay->setScrollOnKeypress(scrollOnKeypress);
}

void TTTermWidget::sendText(const QString &text)
{
    m_impl->m_session->sendText(text);
}

void TTTermWidget::resizeEvent(QResizeEvent*)
{
//qDebug("global window resizing...with %d %d", this->size().width(), this->size().height());
    m_impl->m_terminalDisplay->resize(this->size());

    QStringList env = m_impl->m_session->environment();
    for (QString str : env) {
        if (str.startsWith("LINES") || str.startsWith("COLUMNS")) {
            env.removeOne(str);
        }
    }

    env.append("LINES=" + QString::number(screenLinesCount()));
    env.append("COLUMNS=" + QString::number(screenColumnsCount()));
    m_impl->m_session->setEnvironment(env);
}


void TTTermWidget::sessionFinished()
{
    emit finished();
}

void TTTermWidget::bracketText(QString& text)
{
    m_impl->m_terminalDisplay->bracketText(text);
}

void TTTermWidget::copyClipboard()
{
    m_impl->m_terminalDisplay->copyClipboard();
}

void TTTermWidget::pasteClipboard()
{
    m_impl->m_terminalDisplay->pasteClipboard();
}

void TTTermWidget::pasteSelection()
{
    m_impl->m_terminalDisplay->pasteSelection();
}

void TTTermWidget::setZoom(int step)
{
    if (!m_impl->m_terminalDisplay)
        return;

    QFont font = m_impl->m_terminalDisplay->getVTFont();

    font.setPointSize(font.pointSize() + step);
    setTerminalFont(font);
}

void TTTermWidget::zoomIn()
{
    setZoom(STEP_ZOOM);
}

void TTTermWidget::zoomOut()
{
    setZoom(-STEP_ZOOM);
}

void TTTermWidget::setKeyBindings(const QString & kb)
{
    m_impl->m_session->setKeyBindings(kb);
}

void TTTermWidget::clear()
{
    m_impl->m_session->emulation()->reset();
    m_impl->m_session->refresh();
    m_impl->m_session->clearHistory();
}

void TTTermWidget::setFlowControlEnabled(bool enabled)
{
    m_impl->m_session->setFlowControlEnabled(enabled);
}

QStringList TTTermWidget::availableKeyBindings()
{
    return KeyboardTranslatorManager::instance()->allTranslators();
}

QString TTTermWidget::keyBindings()
{
    return m_impl->m_session->keyBindings();
}

void TTTermWidget::toggleShowSearchBar()
{
    m_searchBar->isHidden() ? m_searchBar->show() : m_searchBar->hide();
}

bool TTTermWidget::flowControlEnabled(void)
{
    return m_impl->m_session->flowControlEnabled();
}

void TTTermWidget::setFlowControlWarningEnabled(bool enabled)
{
    if (flowControlEnabled()) {
        // Do not show warning label if flow control is disabled
        m_impl->m_terminalDisplay->setFlowControlWarningEnabled(enabled);
    }
}

void TTTermWidget::setEnvironment(const QStringList& environment)
{
    m_impl->m_session->setEnvironment(environment);
}

void TTTermWidget::setMotionAfterPasting(int action)
{
    m_impl->m_terminalDisplay->setMotionAfterPasting((Konsole::MotionAfterPasting) action);
}

int TTTermWidget::historyLinesCount()
{
    return m_impl->m_terminalDisplay->screenWindow()->screen()->getHistLines();
}

int TTTermWidget::screenColumnsCount()
{
    return m_impl->m_terminalDisplay->screenWindow()->screen()->getColumns();
}

int TTTermWidget::screenLinesCount()
{
    return m_impl->m_terminalDisplay->screenWindow()->screen()->getLines();
}

void TTTermWidget::setSelectionStart(int row, int column)
{
    m_impl->m_terminalDisplay->screenWindow()->screen()->setSelectionStart(column, row, true);
}

void TTTermWidget::setSelectionEnd(int row, int column)
{
    m_impl->m_terminalDisplay->screenWindow()->screen()->setSelectionEnd(column, row);
}

void TTTermWidget::getSelectionStart(int& row, int& column)
{
    m_impl->m_terminalDisplay->screenWindow()->screen()->getSelectionStart(column, row);
}

void TTTermWidget::getSelectionEnd(int& row, int& column)
{
    m_impl->m_terminalDisplay->screenWindow()->screen()->getSelectionEnd(column, row);
}

QString TTTermWidget::selectedText(bool preserveLineBreaks)
{
    return m_impl->m_terminalDisplay->screenWindow()->screen()->selectedText(preserveLineBreaks);
}

void TTTermWidget::setMonitorActivity(bool monitor)
{
    m_impl->m_session->setMonitorActivity(monitor);
}

void TTTermWidget::setMonitorSilence(bool monitor)
{
    m_impl->m_session->setMonitorSilence(monitor);
}

void TTTermWidget::setSilenceTimeout(int seconds)
{
    m_impl->m_session->setMonitorSilenceSeconds(seconds);
}

Filter::HotSpot* TTTermWidget::getHotSpotAt(const QPoint &pos) const
{
    int row = 0, column = 0;
    m_impl->m_terminalDisplay->getCharacterPosition(pos, row, column);
    return getHotSpotAt(row, column);
}

Filter::HotSpot* TTTermWidget::getHotSpotAt(int row, int column) const
{
    return m_impl->m_terminalDisplay->filterChain()->hotSpotAt(row, column);
}

QList<QAction*> TTTermWidget::filterActions(const QPoint& position)
{
    return m_impl->m_terminalDisplay->filterActions(position);
}

int TTTermWidget::getPtySlaveFd() const
{
    return m_impl->m_session->getPtySlaveFd();
}

void TTTermWidget::setKeyboardCursorShape(KeyboardCursorShape shape)
{
    if (!m_impl->m_terminalDisplay)
        return;
    m_impl->m_terminalDisplay->setKeyboardCursorShape(shape);
}

void TTTermWidget::setBlinkingCursor(bool blink)
{
    if (!m_impl->m_terminalDisplay)
        return;
    m_impl->m_terminalDisplay->setBlinkingCursor(blink);
}

void TTTermWidget::setBidiEnabled(bool enabled)
{
    if (!m_impl->m_terminalDisplay)
        return;
    m_impl->m_terminalDisplay->setBidiEnabled(enabled);
}

bool TTTermWidget::isBidiEnabled()
{
    if (!m_impl->m_terminalDisplay)
        return false; // Default value
    return m_impl->m_terminalDisplay->isBidiEnabled();
}

QString TTTermWidget::title() const
{
    QString title = m_impl->m_session->userTitle();
    if (title.isEmpty())
        title = m_impl->m_session->title(Konsole::Session::NameRole);
    return title;
}

QString TTTermWidget::icon() const
{
    QString icon = m_impl->m_session->iconText();
    if (icon.isEmpty())
        icon = m_impl->m_session->iconName();
    return icon;
}

bool TTTermWidget::isTitleChanged() const
{
    return m_impl->m_session->isTitleChanged();
}

void TTTermWidget::setAutoClose(bool autoClose)
{
    m_impl->m_session->setAutoClose(autoClose);
}

void TTTermWidget::cursorChanged(Konsole::Emulation::KeyboardCursorShape cursorShape, bool blinkingCursorEnabled)
{
    // TODO: A switch to enable/disable DECSCUSR?
    setKeyboardCursorShape(cursorShape);
    setBlinkingCursor(blinkingCursorEnabled);
}

int TTTermWidget::fontHeight() {
    return m_impl->m_terminalDisplay->fontHeight();
}

void TTTermWidget::setFixedHeight(int h) {
    QSize oldSize = this->size();
    QWidget::setFixedHeight(h);
    QResizeEvent e(this->size(), oldSize);
    this->resizeEvent(&e);

    if (m_impl->m_session->isRunning()) {
        kill(getShellPID(), SIGWINCH);
    }
}

bool TTTermWidget::isBusy() {
    return m_impl->m_session->isBusy();
}
