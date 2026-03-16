/**
 * @file go_to_line_dialog.cpp
 * @brief Implementation of the Go To Line dialog.
 */

#include "go_to_line_dialog.h"

#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLabel>
#include <QVBoxLayout>

namespace NovaPad::UI::Dialogs {

GoToLineDialog::GoToLineDialog(int maxLines, QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("Go to Line"));
    setFixedSize(280, 120);

    auto *layout = new QVBoxLayout(this);
    auto *form   = new QFormLayout();

    m_spnLine = new QSpinBox(this);
    m_spnLine->setRange(1, qMax(1, maxLines));
    m_spnLine->setValue(1);
    m_spnLine->selectAll();

    form->addRow(tr("Line number (1–%1):").arg(maxLines), m_spnLine);
    layout->addLayout(form);

    auto *buttons = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
    layout->addWidget(buttons);

    m_spnLine->setFocus();
}

int GoToLineDialog::selectedLine() const
{
    return m_spnLine->value();
}

} // namespace NovaPad::UI::Dialogs
