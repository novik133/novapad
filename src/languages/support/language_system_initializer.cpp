/**
 * @file language_system_initializer.cpp
 * @brief Implementation of language system initialization.
 */

#include "language_system_initializer.h"
#include "language_registry.h"

namespace NovaPad::Languages {

LanguageSystemInitializer::LanguageSystemInitializer()
{
    // Initialize the language registry with all built-in languages
    LanguageRegistry::instance().initializeBuiltinLanguages();
}

} // namespace NovaPad::Languages
