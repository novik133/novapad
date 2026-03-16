/**
 * @file novapad_settings.cpp
 * @brief Implementation of the NovaPadSettings singleton.
 *
 * The singleton is constructed with QSettings in INI format, which
 * stores settings in the platform-appropriate location:
 *   Linux:   ~/.config/NovaPad/NovaPad.ini
 *   macOS:   ~/Library/Preferences/NovaPad/NovaPad.ini
 *   Windows: %APPDATA%/NovaPad/NovaPad.ini
 */

#include "novapad_settings.h"

namespace NovaPad::Settings {

NovaPadSettings &NovaPadSettings::instance()
{
    static NovaPadSettings inst;
    return inst;
}

NovaPadSettings::NovaPadSettings()
    : m_settings(QSettings::IniFormat, QSettings::UserScope,
                 QStringLiteral("NovaPad"), QStringLiteral("NovaPad"))
    , m_general(m_settings)
    , m_appearance(m_settings)
    , m_search(m_settings)
    , m_languages(m_settings)
    , m_mainWindow(m_settings)
{
}

} // namespace NovaPad::Settings
