/**
 * @file output_dock.h
 * @brief Output dock widget for displaying build/command/search results.
 *
 * Provides a read-only text area where background processes (builds,
 * search-in-files, linters) can append their output line by line.
 */

#ifndef NOVAPAD_UI_DOCK_OUTPUT_DOCK_H
#define NOVAPAD_UI_DOCK_OUTPUT_DOCK_H

#include <QDockWidget>
#include <QPlainTextEdit>

namespace NovaPad::UI::Dock {

/**
 * @class OutputDock
 * @brief Dock widget that accumulates text output from various sources.
 */
class OutputDock : public QDockWidget
{
    Q_OBJECT

public:
    explicit OutputDock(QWidget *parent = nullptr);
    ~OutputDock() override = default;

    /** @brief Appends a line of text (auto-scrolls to the bottom). */
    void appendOutput(const QString &text);

    /** @brief Clears all accumulated output. */
    void clearOutput();

private:
    void setupUi();
    QPlainTextEdit *m_outputView = nullptr;
};

} // namespace NovaPad::UI::Dock

#endif // NOVAPAD_UI_DOCK_OUTPUT_DOCK_H
