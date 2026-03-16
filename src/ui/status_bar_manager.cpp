/**
 * @file status_bar_manager.cpp
 * @brief Implementation of the status bar widget manager.
 */

#include "status_bar_manager.h"
#include "editor/code_editor_widget.h"

namespace NovaPad::UI {

StatusBarManager::StatusBarManager(QStatusBar *statusBar, QObject *parent)
    : QObject(parent)
    , m_statusBar(statusBar)
{
    m_cursorLabel  = new QLabel(m_statusBar);
    m_encodingBtn  = new QPushButton(m_statusBar);
    m_eolBtn       = new QPushButton(m_statusBar);
    m_languageBtn  = new QPushButton(m_statusBar);
    m_overtypeBtn  = new QPushButton(m_statusBar);

    // Flat buttons blend into the status bar
    for (auto *btn : { m_encodingBtn, m_eolBtn, m_languageBtn, m_overtypeBtn }) {
        btn->setFlat(true);
        btn->setFocusPolicy(Qt::NoFocus);
    }

    m_overtypeBtn->setText(QStringLiteral("INS"));

    m_statusBar->addPermanentWidget(m_cursorLabel);
    m_statusBar->addPermanentWidget(m_languageBtn);
    m_statusBar->addPermanentWidget(m_eolBtn);
    m_statusBar->addPermanentWidget(m_encodingBtn);
    m_statusBar->addPermanentWidget(m_overtypeBtn);
}

void StatusBarManager::refresh(NovaPad::Editor::CodeEditorWidget *editor)
{
    if (!editor) return;

    // Cursor position (1-indexed for display)
    const auto pos = editor->cursorPos();
    m_cursorLabel->setText(
        QStringLiteral("Ln %1, Col %2").arg(pos.line + 1).arg(pos.column + 1));

    // Encoding
    if (editor->codec()) {
        m_encodingBtn->setText(QString::fromLatin1(editor->codec()->name()));
    }

    // EOL format
    const QString &eol = editor->eolSequence();
    if (eol == QLatin1String("\r\n"))       m_eolBtn->setText(QStringLiteral("Windows (CRLF)"));
    else if (eol == QLatin1String("\r"))     m_eolBtn->setText(QStringLiteral("Mac (CR)"));
    else                                     m_eolBtn->setText(QStringLiteral("Unix (LF)"));

    // Language
    const auto *lang = editor->currentLanguage();
    m_languageBtn->setText(lang ? lang->name : QStringLiteral("Plain Text"));
}

void StatusBarManager::setOverwriteMode(bool overwrite)
{
    m_overtypeBtn->setText(overwrite ? QStringLiteral("OVR") : QStringLiteral("INS"));
}

} // namespace NovaPad::UI
