/**
 * @file language_registry.h
 * @brief Central registry for all supported programming languages.
 *
 * Provides:
 * - Registration and lookup of language support instances
 * - Language detection from file extensions and content
 * - Factory for creating language-specific features
 * - Query for all available languages
 */

#ifndef NOVAPAD_LANGUAGES_SUPPORT_LANGUAGE_REGISTRY_H
#define NOVAPAD_LANGUAGES_SUPPORT_LANGUAGE_REGISTRY_H

#include "languages/base/language_support_base.h"

#include <QMap>
#include <QMutex>
#include <QMutexLocker>
#include <memory>
#include <map>

namespace NovaPad::Languages {

/**
 * @class LanguageRegistry
 * @brief Singleton registry managing all language support implementations.
 *
 * Thread-safe registry that maintains a map of language IDs to their
 * corresponding LanguageSupport implementations. Provides factory methods
 * for creating language-specific editor components.
 */
class LanguageRegistry final
{
public:
    /** @brief Returns the singleton instance. */
    [[nodiscard]] static LanguageRegistry &instance();
    
    // Non-copyable, non-movable
    LanguageRegistry(const LanguageRegistry &) = delete;
    LanguageRegistry &operator=(const LanguageRegistry &) = delete;
    
    /**
     * @brief Registers a language support implementation.
     * @param id Language identifier (e.g., "cpp", "python")
     * @param factory Factory function to create the language support
     */
    void registerLanguage(const QString &id, 
                          std::function<std::unique_ptr<LanguageSupport>()> factory);
    
    /**
     * @brief Creates a language support instance by ID.
     * @param id Language identifier
     * @return Language support instance, or nullptr if not found
     */
    [[nodiscard]] std::unique_ptr<LanguageSupport> createLanguage(const QString &id) const;
    
    /**
     * @brief Gets language support without taking ownership.
     * @param id Language identifier
     * @return Pointer to cached language support, or nullptr
     */
    [[nodiscard]] const LanguageSupport *getLanguage(const QString &id) const;
    
    /**
     * @brief Detects language from file path and optional content.
     * @param filePath Path to the file
     * @param content Optional file content for shebang/content detection
     * @return Detected language ID, or "text" if unknown
     */
    [[nodiscard]] QString detectLanguage(const QString &filePath,
                                         const QString &content = {}) const;
    
    /**
     * @brief Returns all registered language IDs.
     */
    [[nodiscard]] QStringList allLanguageIds() const;
    
    /**
     * @brief Returns all registered languages with their display names.
     */
    [[nodiscard]] QMap<QString, QString> allLanguages() const;
    
    /**
     * @brief Checks if a language is registered.
     */
    [[nodiscard]] bool hasLanguage(const QString &id) const;
    
    /**
     * @brief Initializes all built-in language support instances.
     * Called once at application startup.
     */
    void initializeBuiltinLanguages();
    
private:
    LanguageRegistry() = default;
    ~LanguageRegistry() = default;
    
    mutable QMutex m_mutex;
    QMap<QString, std::function<std::unique_ptr<LanguageSupport>()>> m_factories;
    mutable std::map<QString, std::unique_ptr<LanguageSupport>> m_cache;
    
    // Extension to language ID mapping
    QMap<QString, QString> m_extensionMap;
    // Filename to language ID mapping  
    QMap<QString, QString> m_filenameMap;
    
    void buildExtensionMap();
    void buildFilenameMap();
};

} // namespace NovaPad::Languages

#endif // NOVAPAD_LANGUAGES_SUPPORT_LANGUAGE_REGISTRY_H
