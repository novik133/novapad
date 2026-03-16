/**
 * @file preferences_dialog.cpp
 * @brief Implementation of the application preferences dialog.
 *
 * The dialog is organised into tabbed pages mirroring the NovaPadSettings
 * categories: General, Appearance, Languages, Search, Shortcuts, Extensions.
 * Each page reads its initial values from NovaPadSettings and writes them
 * back when the user clicks OK.
 */

#include "preferences_dialog.h"
#include "settings/novapad_settings.h"

#include <QDialogButtonBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QVBoxLayout>

namespace NovaPad::UI::Dialogs {

// ---------------------------------------------------------------------------
// Construction
// ---------------------------------------------------------------------------

PreferencesDialog::PreferencesDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("Preferences"));
    setMinimumSize(640, 480);
    setupUi();
    loadSettings();
}

// ---------------------------------------------------------------------------
// UI construction
// ---------------------------------------------------------------------------

void PreferencesDialog::setupUi()
{
    auto *mainLayout = new QVBoxLayout(this);

    m_tabs = new QTabWidget(this);
    m_tabs->addTab(createGeneralPage(),     tr("General"));
    m_tabs->addTab(createAppearancePage(),  tr("Appearance"));
    m_tabs->addTab(createLanguagesPage(),   tr("Languages"));
    m_tabs->addTab(createSearchPage(),      tr("Search"));
    m_tabs->addTab(createShortcutsPage(),   tr("Shortcuts"));
    m_tabs->addTab(createExtensionsPage(),  tr("Extensions"));
    mainLayout->addWidget(m_tabs);

    auto *buttons = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttons, &QDialogButtonBox::accepted, this, &PreferencesDialog::onAccepted);
    connect(buttons, &QDialogButtonBox::rejected, this, &PreferencesDialog::onRejected);
    mainLayout->addWidget(buttons);
}

QWidget *PreferencesDialog::createGeneralPage()
{
    auto *page   = new QWidget(this);
    auto *layout = new QFormLayout(page);

    m_chkWarnIndent    = new QCheckBox(tr("Warn for different indentation"), page);
    m_chkExitOnLastTab = new QCheckBox(tr("Exit on last tab close"), page);
    m_chkRememberTabs  = new QCheckBox(tr("Remember tabs on exit"), page);
    m_chkSmartIndent   = new QCheckBox(tr("Smart indentation"), page);
    m_chkMathRendering = new QCheckBox(tr("Math rendering"), page);

    m_spnAutosave = new QSpinBox(page);
    m_spnAutosave->setRange(0, 300);
    m_spnAutosave->setSuffix(tr(" sec"));
    m_spnAutosave->setSpecialValueText(tr("Disabled"));

    m_spnWarnFileSize = new QSpinBox(page);
    m_spnWarnFileSize->setRange(0, 100);
    m_spnWarnFileSize->setSuffix(tr(" MB"));
    m_spnWarnFileSize->setSpecialValueText(tr("No warning"));

    m_cmbLocale = new QComboBox(page);
    m_cmbLocale->addItem(tr("English"), "en");
    m_cmbLocale->addItem(tr("Polish"),  "pl");
    m_cmbLocale->addItem(tr("German"),  "de");
    m_cmbLocale->addItem(tr("French"),  "fr");
    m_cmbLocale->addItem(tr("Spanish"), "es");

    layout->addRow(m_chkWarnIndent);
    layout->addRow(m_chkExitOnLastTab);
    layout->addRow(m_chkRememberTabs);
    layout->addRow(m_chkSmartIndent);
    layout->addRow(m_chkMathRendering);
    layout->addRow(tr("Autosave interval:"), m_spnAutosave);
    layout->addRow(tr("Warn if file larger than:"), m_spnWarnFileSize);
    layout->addRow(tr("Language:"), m_cmbLocale);

    return page;
}

QWidget *PreferencesDialog::createAppearancePage()
{
    auto *page   = new QWidget(this);
    auto *layout = new QFormLayout(page);

    m_cmbColorScheme = new QComboBox(page);
    m_cmbColorScheme->addItem(tr("Default"));
    m_cmbColorScheme->addItem(tr("Dark"));
    m_cmbColorScheme->addItem(tr("Monokai"));
    m_cmbColorScheme->addItem(tr("Solarized Light"));
    m_cmbColorScheme->addItem(tr("Solarized Dark"));

    m_cmbFontFamily = new QFontComboBox(page);
    m_cmbFontFamily->setFontFilters(QFontComboBox::MonospacedFonts);

    m_spnFontSize = new QSpinBox(page);
    m_spnFontSize->setRange(6, 72);
    m_spnFontSize->setValue(12);

    m_chkShowLineNumbers = new QCheckBox(tr("Show line numbers"), page);

    layout->addRow(tr("Color scheme:"), m_cmbColorScheme);
    layout->addRow(tr("Font family:"),  m_cmbFontFamily);
    layout->addRow(tr("Font size:"),    m_spnFontSize);
    layout->addRow(m_chkShowLineNumbers);

    return page;
}

QWidget *PreferencesDialog::createLanguagesPage()
{
    auto *page   = new QWidget(this);
    auto *layout = new QVBoxLayout(page);
    layout->addWidget(new QLabel(
        tr("Language-specific indentation settings can be configured here.\n"
           "Select a language from the list to override its default tab size and style."),
        page));
    // A full implementation would list all languages with per-language overrides
    layout->addStretch();
    return page;
}

QWidget *PreferencesDialog::createSearchPage()
{
    auto *page   = new QWidget(this);
    auto *layout = new QFormLayout(page);

    m_chkSearchAsIType = new QCheckBox(tr("Search as I type"), page);
    m_chkSaveHistory   = new QCheckBox(tr("Save search history"), page);

    layout->addRow(m_chkSearchAsIType);
    layout->addRow(m_chkSaveHistory);

    return page;
}

QWidget *PreferencesDialog::createShortcutsPage()
{
    auto *page   = new QWidget(this);
    auto *layout = new QVBoxLayout(page);
    layout->addWidget(new QLabel(
        tr("Keyboard shortcuts can be customised here.\n"
           "Double-click a shortcut cell to record a new key combination."),
        page));
    // A full implementation would show a QTableView of all actions + shortcuts
    layout->addStretch();
    return page;
}

QWidget *PreferencesDialog::createExtensionsPage()
{
    auto *page   = new QWidget(this);
    auto *layout = new QVBoxLayout(page);
    layout->addWidget(new QLabel(
        tr("Extension runtime paths and configuration.\n"
           "Extensions add new features to NovaPad via a plugin API."),
        page));
    layout->addStretch();
    return page;
}

// ---------------------------------------------------------------------------
// Load / save settings
// ---------------------------------------------------------------------------

void PreferencesDialog::loadSettings()
{
    auto &s = NovaPad::Settings::NovaPadSettings::instance();

    m_chkWarnIndent->setChecked(s.general().warnForDifferentIndentation());
    m_chkExitOnLastTab->setChecked(s.general().exitOnLastTabClose());
    m_chkRememberTabs->setChecked(s.general().rememberTabsOnExit());
    m_chkSmartIndent->setChecked(s.general().smartIndentation());
    m_chkMathRendering->setChecked(s.general().mathRendering());
    m_spnAutosave->setValue(s.general().autosaveInterval());
    m_spnWarnFileSize->setValue(s.general().warnIfFileLargerThan());

    m_chkShowLineNumbers->setChecked(s.appearance().showLineNumbers());

    m_chkSearchAsIType->setChecked(s.search().searchAsIType());
    m_chkSaveHistory->setChecked(s.search().saveHistory());
}

void PreferencesDialog::saveSettings()
{
    auto &s = NovaPad::Settings::NovaPadSettings::instance();

    s.general().setWarnForDifferentIndentation(m_chkWarnIndent->isChecked());
    s.general().setExitOnLastTabClose(m_chkExitOnLastTab->isChecked());
    s.general().setRememberTabsOnExit(m_chkRememberTabs->isChecked());
    s.general().setSmartIndentation(m_chkSmartIndent->isChecked());
    s.general().setMathRendering(m_chkMathRendering->isChecked());
    s.general().setAutosaveInterval(m_spnAutosave->value());
    s.general().setWarnIfFileLargerThan(m_spnWarnFileSize->value());

    s.appearance().setShowLineNumbers(m_chkShowLineNumbers->isChecked());

    s.search().setSearchAsIType(m_chkSearchAsIType->isChecked());
    s.search().setSaveHistory(m_chkSaveHistory->isChecked());
}

// ---------------------------------------------------------------------------
// Slots
// ---------------------------------------------------------------------------

void PreferencesDialog::onAccepted()
{
    saveSettings();
    accept();
}

void PreferencesDialog::onRejected()
{
    reject();
}

} // namespace NovaPad::UI::Dialogs
