/**
 * @file language_system_initializer.h
 * @brief Initialization helper for the language support system.
 */

#ifndef NOVAPAD_LANGUAGES_SUPPORT_LANGUAGE_SYSTEM_INITIALIZER_H
#define NOVAPAD_LANGUAGES_SUPPORT_LANGUAGE_SYSTEM_INITIALIZER_H

namespace NovaPad::Languages {

/**
 * @class LanguageSystemInitializer
 * @brief RAII helper for initializing the language support system.
 *
 * Automatically registers all built-in languages on construction.
 * Call this once at application startup before creating any editors.
 */
class LanguageSystemInitializer
{
public:
    LanguageSystemInitializer();
    ~LanguageSystemInitializer() = default;
    
    // Non-copyable, non-movable
    LanguageSystemInitializer(const LanguageSystemInitializer &) = delete;
    LanguageSystemInitializer &operator=(const LanguageSystemInitializer &) = delete;
};

} // namespace NovaPad::Languages

#endif // NOVAPAD_LANGUAGES_SUPPORT_LANGUAGE_SYSTEM_INITIALIZER_H
