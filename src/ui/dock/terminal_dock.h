/**
 * @file terminal_dock.h
 * @brief Integrated terminal dock widget.
 *
 * Provides a simple embedded terminal using QProcess, allowing the user
 * to run shell commands without leaving NovaPad.
 */

#ifndef NOVAPAD_UI_DOCK_TERMINAL_DOCK_H
#define NOVAPAD_UI_DOCK_TERMINAL_DOCK_H

#include <QDockWidget>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QProcess>

namespace NovaPad::UI::Dock {

/**
 * @class TerminalDock
 * @brief Dock widget with a command input and scrollable output area.
 */
class TerminalDock : public QDockWidget
{
    Q_OBJECT

public:
    explicit TerminalDock(QWidget *parent = nullptr);
    ~TerminalDock() override;

    /** @brief Sets the working directory for subsequent commands. */
    void setWorkingDirectory(const QString &dir);

private slots:
    void onCommandEntered();
    void onProcessOutput();
    void onProcessFinished(int exitCode, QProcess::ExitStatus status);

private:
    void setupUi();

    QPlainTextEdit *m_outputView = nullptr;
    QLineEdit      *m_inputLine  = nullptr;
    QProcess       *m_process    = nullptr;
    QString         m_workingDir;
};

} // namespace NovaPad::UI::Dock

#endif // NOVAPAD_UI_DOCK_TERMINAL_DOCK_H
