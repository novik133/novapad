/**
 * @file about_dialog.cpp
 * @brief Implementation of the About dialog.
 */

#include "about_dialog.h"
#include "core/globals.h"

#include <novapad_version.h>

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

namespace NovaPad::UI::Dialogs {

AboutDialog::AboutDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("About NovaPad"));
    setFixedSize(500, 420);
    setupUi();
}

void AboutDialog::setupUi()
{
    auto *mainLayout = new QVBoxLayout(this);

    // Application icon + name
    auto *headerLayout = new QHBoxLayout();
    auto *iconLabel = new QLabel(this);
    iconLabel->setPixmap(QIcon(":/icons/novapad/scalable/notepadqq.svg")
                             .pixmap(64, 64));
    headerLayout->addWidget(iconLabel);

    auto *titleLabel = new QLabel(this);
    titleLabel->setText(QStringLiteral(
        "<h2>NovaPad %1</h2>"
        "<p>A modern programmer's text editor</p>")
        .arg(NOVAPAD_VERSION_STRING));
    titleLabel->setTextFormat(Qt::RichText);
    headerLayout->addWidget(titleLabel, 1);
    mainLayout->addLayout(headerLayout);

    // Description and credits
    auto *descLabel = new QLabel(this);
    descLabel->setWordWrap(true);
    descLabel->setTextFormat(Qt::RichText);
    descLabel->setOpenExternalLinks(true);
    descLabel->setText(QStringLiteral(
        "<p><b>Copyright © 2025-2026 Kamil 'Novik' Nowicki</b></p>"
        "<p>Website: <a href=\"https://noviktech.com\">https://noviktech.com</a><br>"
        "GitHub: <a href=\"https://github.com/novik133/novapad\">https://github.com/novik133/novapad</a></p>"
        "<p>Built with C++23 and Qt %1.</p>"
        "<p>Licensed under the GNU General Public License v2.0</p>"
        "<hr>"
        "<p><i>Based on Notepadqq, created by Daniele Di Sarli and contributors (2015-2019).</i><br>"
        "Notepadqq GitHub: <a href=\"https://github.com/notepadqq/notepadqq\">https://github.com/notepadqq/notepadqq</a></p>")
        .arg(QStringLiteral(QT_VERSION_STR)));

    mainLayout->addWidget(descLabel);
    mainLayout->addStretch();

    // Close button
    auto *closeBtn = new QPushButton(tr("Close"), this);
    connect(closeBtn, &QPushButton::clicked, this, &QDialog::accept);
    auto *btnLayout = new QHBoxLayout();
    btnLayout->addStretch();
    btnLayout->addWidget(closeBtn);
    mainLayout->addLayout(btnLayout);
}

} // namespace NovaPad::UI::Dialogs
