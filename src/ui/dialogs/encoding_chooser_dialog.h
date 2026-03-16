/**
 * @file encoding_chooser_dialog.h
 * @brief Dialog for selecting a text encoding from a filterable list.
 */

#ifndef NOVAPAD_UI_DIALOGS_ENCODING_CHOOSER_DIALOG_H
#define NOVAPAD_UI_DIALOGS_ENCODING_CHOOSER_DIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QListWidget>

namespace NovaPad::UI::Dialogs {

/**
 * @class EncodingChooserDialog
 * @brief Presents a searchable list of all available text encodings.
 */
class EncodingChooserDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EncodingChooserDialog(QWidget *parent = nullptr);
    ~EncodingChooserDialog() override = default;

    /** @brief Returns the encoding name selected by the user. */
    [[nodiscard]] QString selectedEncoding() const;

private slots:
    void onFilterChanged(const QString &text);

private:
    void setupUi();
    void populateEncodings();

    QLineEdit   *m_filterEdit  = nullptr;
    QListWidget *m_listWidget  = nullptr;
    QString      m_selected;
};

} // namespace NovaPad::UI::Dialogs

#endif // NOVAPAD_UI_DIALOGS_ENCODING_CHOOSER_DIALOG_H
