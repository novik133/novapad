/**
 * @file run_command_dialog.cpp
 * @brief Implementation of the Run Command dialog.
 */

#include "run_command_dialog.h"

#include <QDialogButtonBox>
#include <QDir>
#include <QFormLayout>
#include <QVBoxLayout>

namespace NovaPad::UI::Dialogs {

RunCommandDialog::RunCommandDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("Run Command"));
    setMinimumWidth(400);
    setupUi();
}

void RunCommandDialog::setupUi()
{
    auto *layout = new QVBoxLayout(this);
    auto *form   = new QFormLayout();

    m_cmdEdit = new QLineEdit(this);
    m_cmdEdit->setPlaceholderText(tr("Enter command..."));

    m_cwdEdit = new QLineEdit(this);
    m_cwdEdit->setText(QDir::currentPath());
    m_cwdEdit->setPlaceholderText(tr("Working directory"));

    form->addRow(tr("Command:"), m_cmdEdit);
    form->addRow(tr("Working directory:"), m_cwdEdit);
    layout->addLayout(form);

    auto *buttons = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
    layout->addWidget(buttons);

    m_cmdEdit->setFocus();
}

QString RunCommandDialog::command() const          { return m_cmdEdit->text(); }
QString RunCommandDialog::workingDirectory() const { return m_cwdEdit->text(); }

} // namespace NovaPad::UI::Dialogs
