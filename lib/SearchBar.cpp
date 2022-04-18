/*
    Copyright 2013 Christian Surlykke

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
    02110-1301  USA.
*/
#include <QMenu>
#include <QAction>
#include <QRegExp>
#include <QDebug>
#include <QRegularExpression>
#include <terrorflash.h>

#include "SearchBar.h"

SearchBar::SearchBar(QWidget *parent) : QWidget(parent)
{
    widget.setupUi(this);
    setAutoFillBackground(true); // make it always opaque, especially inside translucent windows

    widget.regexErrorLabel->setVisible(false);

    connect(widget.closeButton, SIGNAL(clicked()), this, SLOT(hide()));
    connect(widget.closeButton, &QAbstractButton::clicked, this, &SearchBar::hide);
    connect(widget.searchTextEdit, SIGNAL(textChanged(QString)), this, SIGNAL(searchCriteriaChanged()));
    connect(widget.findPreviousButton, SIGNAL(clicked()), this, SIGNAL(findPrevious()));
    connect(widget.findNextButton, SIGNAL(clicked()), this, SIGNAL(findNext()));

    connect(this, &SearchBar::searchCriteriaChanged, [=] {
        if (m_useRegularExpressionMenuEntry->isChecked()) {
            QRegularExpression searchCriteria(widget.searchTextEdit->text());
            if (searchCriteria.isValid()) {
                widget.regexErrorLabel->setVisible(false);
            } else {
                widget.regexErrorLabel->setText(searchCriteria.errorString());
                widget.regexErrorLabel->setVisible(true);
            }
        } else {
            widget.regexErrorLabel->setVisible(false);
        }
    });

    QMenu *optionsMenu = new QMenu(widget.optionsButton);
    widget.optionsButton->setMenu(optionsMenu);

    m_matchCaseMenuEntry = optionsMenu->addAction(tr("Match case"));
    m_matchCaseMenuEntry->setCheckable(true);
    m_matchCaseMenuEntry->setChecked(true);
    connect(m_matchCaseMenuEntry, SIGNAL(toggled(bool)), this, SIGNAL(searchCriteriaChanged()));


    m_useRegularExpressionMenuEntry = optionsMenu->addAction(tr("Regular expression"));
    m_useRegularExpressionMenuEntry->setCheckable(true);
    connect(m_useRegularExpressionMenuEntry, SIGNAL(toggled(bool)), this, SIGNAL(searchCriteriaChanged()));

    m_highlightMatchesMenuEntry = optionsMenu->addAction(tr("Highlight all matches"));
    m_highlightMatchesMenuEntry->setCheckable(true);
    m_highlightMatchesMenuEntry->setChecked(true);
    connect(m_highlightMatchesMenuEntry, SIGNAL(toggled(bool)), this, SIGNAL(highlightMatchesChanged(bool)));
}

SearchBar::~SearchBar() {
}

QString SearchBar::searchText()
{
    return widget.searchTextEdit->text();
}


bool SearchBar::useRegularExpression()
{
    return m_useRegularExpressionMenuEntry->isChecked();
}

bool SearchBar::matchCase()
{
    return m_matchCaseMenuEntry->isChecked();
}

bool SearchBar::highlightAllMatches()
{
    return m_highlightMatchesMenuEntry->isChecked();
}

void SearchBar::show()
{
    QWidget::show();
    widget.searchTextEdit->setFocus();
    widget.searchTextEdit->selectAll();
}

void SearchBar::hide()
{
    QWidget::hide();
    if (QWidget *p = parentWidget())
    {
        p->setFocus(Qt::OtherFocusReason); // give the focus to the parent widget on hiding
    }
}

void SearchBar::noMatchFound()
{
    tErrorFlash::flashError(widget.searchTextEdit);
}


void SearchBar::keyReleaseEvent(QKeyEvent* keyEvent)
{
    if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter)
    {
        if (keyEvent->modifiers() == Qt::ShiftModifier)
        {
            Q_EMIT findPrevious();
        }
        else
        {
            Q_EMIT findNext();
        }
    }
    else if (keyEvent->key() == Qt::Key_Escape)
    {
        hide();
    }
}
