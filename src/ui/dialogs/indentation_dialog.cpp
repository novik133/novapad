/**
 * @file indentation_dialog.cpp
 * @brief Implementation of the indentation configuration dialog.
 */

#include "indentation_dialog.h"

#include <QDialogButtonBox>
#include <QFormLayout>
#include <QVBoxLayout>

namespace NovaPad::UI::Dialogs {

IndentationDialog::IndentationDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("Indentation Settings"));
    setupUi();
}

void IndentationDialog::setupUi()
{
    auto *layout = new QVBoxLayout(this);
    auto *form   = new QFormLayout();

    m_chkUseTabs = new QCheckBox(tr("Use tabs instead of spaces"), this);
    m_spnTabSize = new QSpinBox(this);
    m_spnTabSize->setRange(1, 16);
    m_spnTabSize->setValue(4);

    form->addRow(m_chkUseTabs);
    form->addRow(tr("Tab size:"), m_spnTabSize);
    layout->addLayout(form);

    auto *buttons = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
    layout->addWidget(buttons);
}

bool IndentationDialog::useTabs() const { return m_chkUseTabs->isChecked(); }
int  IndentationDialog::tabSize() const { return m_spnTabSize->value(); }

} // namespace NovaPad::UI::Dialogs
