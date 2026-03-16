/**
 * @file language_registry.cpp
 * @brief Implementation of the language registry.
 */

#include "language_registry.h"

// Include all language definitions
#include "languages/definitions/c_language.h"
#include "languages/definitions/cpp_language.h"
#include "languages/definitions/cs_language.h"
#include "languages/definitions/rust_language.h"
#include "languages/definitions/go_language.h"
#include "languages/definitions/python_language.h"
#include "languages/definitions/javascript_language.h"
#include "languages/definitions/kotlin_language.h"
#include "languages/definitions/swift_language.h"
#include "languages/definitions/php_language.h"
#include "languages/definitions/perl_language.h"
#include "languages/definitions/html_css_language.h"
#include "languages/definitions/markdown_language.h"
#include "languages/definitions/sql_language.h"

#include <QFileInfo>

namespace NovaPad::Languages {

LanguageRegistry &LanguageRegistry::instance()
{
    static LanguageRegistry instance;
    return instance;
}

void LanguageRegistry::registerLanguage(const QString &id,
    std::function<std::unique_ptr<LanguageSupport>()> factory)
{
    QMutexLocker lock(&m_mutex);
    m_factories[id] = std::move(factory);
}

std::unique_ptr<LanguageSupport> LanguageRegistry::createLanguage(const QString &id) const
{
    QMutexLocker lock(&m_mutex);
    auto it = m_factories.find(id);
    if (it != m_factories.end()) {
        return it.value()();
    }
    return nullptr;
}

const LanguageSupport *LanguageRegistry::getLanguage(const QString &id) const
{
    QMutexLocker lock(&m_mutex);
    
    // Check cache first
    auto cacheIt = m_cache.find(id);
    if (cacheIt != m_cache.end()) {
        return cacheIt->second.get();
    }
    
    // Create and cache
    auto lang = createLanguage(id);
    if (lang) {
        const auto *ptr = lang.get();
        m_cache[id] = std::move(lang);
        return ptr;
    }
    
    return nullptr;
}

QString LanguageRegistry::detectLanguage(const QString &filePath,
                                          const QString &content) const
{
    QFileInfo info(filePath);
    QString ext = info.suffix().toLower();
    QString fileName = info.fileName().toLower();
    
    // Check filename matches first
    auto fnIt = m_filenameMap.find(fileName);
    if (fnIt != m_filenameMap.end()) {
        return fnIt.value();
    }
    
    // Check extension
    auto extIt = m_extensionMap.find(ext);
    if (extIt != m_extensionMap.end()) {
        return extIt.value();
    }
    
    // Check shebang in content
    if (!content.isEmpty()) {
        QString firstLine = content.split('\n').first();
        if (firstLine.startsWith("#!/")) {
            if (firstLine.contains("python")) return "python";
            if (firstLine.contains("perl")) return "perl";
            if (firstLine.contains("ruby")) return "ruby";
            if (firstLine.contains("node")) return "javascript";
            if (firstLine.contains("bash") || firstLine.contains("sh")) return "shell";
            if (firstLine.contains("php")) return "php";
        }
    }
    
    return "text";
}

QStringList LanguageRegistry::allLanguageIds() const
{
    QMutexLocker lock(&m_mutex);
    return m_factories.keys();
}

QMap<QString, QString> LanguageRegistry::allLanguages() const
{
    QMap<QString, QString> result;
    QMutexLocker lock(&m_mutex);
    
    for (auto it = m_factories.begin(); it != m_factories.end(); ++it) {
        auto lang = it.value()();
        if (lang) {
            result[it.key()] = lang->displayName();
        }
    }
    
    return result;
}

bool LanguageRegistry::hasLanguage(const QString &id) const
{
    QMutexLocker lock(&m_mutex);
    return m_factories.contains(id);
}

void LanguageRegistry::initializeBuiltinLanguages()
{
    // Register C family
    registerLanguage("c", []() { return std::make_unique<CLanguageSupport>(); });
    registerLanguage("cpp", []() { return std::make_unique<CppLanguageSupport>(); });
    registerLanguage("csharp", []() { return std::make_unique<CsLanguageSupport>(); });
    
    // Register modern systems languages
    registerLanguage("rust", []() { return std::make_unique<RustLanguageSupport>(); });
    registerLanguage("go", []() { return std::make_unique<GoLanguageSupport>(); });
    
    // Register scripting languages
    registerLanguage("python", []() { return std::make_unique<PythonLanguageSupport>(); });
    registerLanguage("javascript", []() { return std::make_unique<JavaScriptLanguageSupport>(); });
    registerLanguage("php", []() { return std::make_unique<PhpLanguageSupport>(); });
    registerLanguage("perl", []() { return std::make_unique<PerlLanguageSupport>(); });
    
    // Register JVM/mobile languages
    registerLanguage("kotlin", []() { return std::make_unique<KotlinLanguageSupport>(); });
    registerLanguage("swift", []() { return std::make_unique<SwiftLanguageSupport>(); });
    
    // Register markup/data languages
    registerLanguage("html", []() { return std::make_unique<HtmlLanguageSupport>(); });
    registerLanguage("css", []() { return std::make_unique<CssLanguageSupport>(); });
    registerLanguage("json", []() { return std::make_unique<JsonLanguageSupport>(); });
    registerLanguage("xml", []() { return std::make_unique<XmlLanguageSupport>(); });
    registerLanguage("markdown", []() { return std::make_unique<MarkdownLanguageSupport>(); });
    
    // Register database languages
    registerLanguage("sql", []() { return std::make_unique<SqlLanguageSupport>(); });
    
    buildExtensionMap();
    buildFilenameMap();
}

void LanguageRegistry::buildExtensionMap()
{
    // C family
    m_extensionMap["c"] = "c";
    m_extensionMap["h"] = "c";
    m_extensionMap["cpp"] = "cpp";
    m_extensionMap["cc"] = "cpp";
    m_extensionMap["cxx"] = "cpp";
    m_extensionMap["hpp"] = "cpp";
    m_extensionMap["hh"] = "cpp";
    m_extensionMap["cs"] = "csharp";
    m_extensionMap["csx"] = "csharp";
    
    // Systems languages
    m_extensionMap["rs"] = "rust";
    m_extensionMap["go"] = "go";
    
    // Scripting
    m_extensionMap["py"] = "python";
    m_extensionMap["pyw"] = "python";
    m_extensionMap["pyi"] = "python";
    m_extensionMap["js"] = "javascript";
    m_extensionMap["jsx"] = "javascript";
    m_extensionMap["ts"] = "javascript";
    m_extensionMap["tsx"] = "javascript";
    m_extensionMap["mjs"] = "javascript";
    m_extensionMap["php"] = "php";
    m_extensionMap["pl"] = "perl";
    m_extensionMap["pm"] = "perl";
    
    // JVM/Mobile
    m_extensionMap["kt"] = "kotlin";
    m_extensionMap["kts"] = "kotlin";
    m_extensionMap["swift"] = "swift";
    
    // Markup/Data
    m_extensionMap["html"] = "html";
    m_extensionMap["htm"] = "html";
    m_extensionMap["css"] = "css";
    m_extensionMap["scss"] = "css";
    m_extensionMap["json"] = "json";
    m_extensionMap["xml"] = "xml";
    m_extensionMap["md"] = "markdown";
    m_extensionMap["markdown"] = "markdown";
    
    // Database
    m_extensionMap["sql"] = "sql";
    m_extensionMap["ddl"] = "sql";
    m_extensionMap["dml"] = "sql";
}

void LanguageRegistry::buildFilenameMap()
{
    m_filenameMap["cargo.toml"] = "rust";
    m_filenameMap["go.mod"] = "go";
    m_filenameMap["package.json"] = "javascript";
    m_filenameMap["composer.json"] = "php";
    m_filenameMap["requirements.txt"] = "python";
    m_filenameMap["cpanfile"] = "perl";
    m_filenameMap["makefile"] = "makefile";
    m_filenameMap["cmakefile"] = "cmake";
    m_filenameMap["dockerfile"] = "dockerfile";
    m_filenameMap["readme.md"] = "markdown";
    m_filenameMap["tsconfig.json"] = "javascript";
    m_filenameMap["build.gradle.kts"] = "kotlin";
    m_filenameMap["package.swift"] = "swift";
}

} // namespace NovaPad::Languages
