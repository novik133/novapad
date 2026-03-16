/**
 * @file encoding_chooser_dialog.cpp
 * @brief Implementation of the encoding chooser dialog.
 */

#include "encoding_chooser_dialog.h"

#include <QDialogButtonBox>
#include <QTextCodec>
#include <QVBoxLayout>

namespace NovaPad::UI::Dialogs {

EncodingChooserDialog::EncodingChooserDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("Choose Encoding"));
    setMinimumSize(360, 400);
    setupUi();
    populateEncodings();
}

void EncodingChooserDialog::setupUi()
{
    auto *layout = new QVBoxLayout(this);

    m_filterEdit = new QLineEdit(this);
    m_filterEdit->setPlaceholderText(tr("Filter encodings..."));
    connect(m_filterEdit, &QLineEdit::textChanged,
            this, &EncodingChooserDialog::onFilterChanged);
    layout->addWidget(m_filterEdit);

    m_listWidget = new QListWidget(this);
    layout->addWidget(m_listWidget);

    auto *buttons = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttons, &QDialogButtonBox::accepted, this, [this]() {
        auto *item = m_listWidget->currentItem();
        if (item) m_selected = item->text();
        accept();
    });
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
    layout->addWidget(buttons);
}

void EncodingChooserDialog::populateEncodings()
{
    // Enumerate all available codecs from Qt's registry
    QList<QByteArray> codecs = QTextCodec::availableCodecs();
    std::sort(codecs.begin(), codecs.end());

    for (const QByteArray &name : codecs) {
        m_listWidget->addItem(QString::fromLatin1(name));
    }

    // Select UTF-8 by default
    auto items = m_listWidget->findItems("UTF-8", Qt::MatchExactly);
    if (!items.isEmpty()) {
        m_listWidget->setCurrentItem(items.first());
    }
}

void EncodingChooserDialog::onFilterChanged(const QString &text)
{
    for (int i = 0; i < m_listWidget->count(); ++i) {
        auto *item = m_listWidget->item(i);
        item->setHidden(!item->text().contains(text, Qt::CaseInsensitive));
    }
}

QString EncodingChooserDialog::selectedEncoding() const
{
    return m_selected;
}

} // namespace NovaPad::UI::Dialogs
