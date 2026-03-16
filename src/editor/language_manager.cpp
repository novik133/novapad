/**
 * @file language_manager.cpp
 * @brief Implementation of the language registry and lookup service.
 *
 * The catalogue is hard-coded for performance and determinism — no external
 * JSON/XML files need to be parsed at startup.  Adding a new language is a
 * single-step operation: append a Language struct inside registerLanguages().
 */

#include "language_manager.h"

#include <QFileInfo>
#include <QRegularExpression>

namespace NovaPad::Editor {

// ---------------------------------------------------------------------------
// Singleton access
// ---------------------------------------------------------------------------

LanguageManager &LanguageManager::instance()
{
    static LanguageManager inst;
    return inst;
}

// ---------------------------------------------------------------------------
// Construction — populates the catalogue once
// ---------------------------------------------------------------------------

LanguageManager::LanguageManager()
{
    registerLanguages();
}

// ---------------------------------------------------------------------------
// Lookup methods
// ---------------------------------------------------------------------------

const Language *LanguageManager::lookupById(const QString &id) const
{
    for (const auto &lang : m_languages) {
        if (lang.id == id) return &lang;
    }
    return nullptr;
}

const Language *LanguageManager::lookupByFileName(const QString &fileName) const
{
    const QString baseName = QFileInfo(fileName).fileName();
    for (const auto &lang : m_languages) {
        if (lang.fileNames.contains(baseName, Qt::CaseInsensitive)) {
            return &lang;
        }
    }
    return nullptr;
}

const Language *LanguageManager::lookupByExtension(const QString &filePath) const
{
    const QString suffix = QFileInfo(filePath).suffix().toLower();
    if (suffix.isEmpty()) return nullptr;

    for (const auto &lang : m_languages) {
        if (lang.fileExtensions.contains(suffix, Qt::CaseInsensitive)) {
            return &lang;
        }
    }
    return nullptr;
}

const Language *LanguageManager::lookupByContent(const QString &content) const
{
    // Extract the first non-blank line for shebang detection
    const QStringList lines = content.left(512).split(QLatin1Char('\n'));
    QString firstLine;
    for (const QString &line : lines) {
        if (!line.trimmed().isEmpty()) {
            firstLine = line.trimmed();
            break;
        }
    }
    if (firstLine.isEmpty()) return nullptr;

    for (const auto &lang : m_languages) {
        for (const QString &pattern : lang.firstNonBlankLine) {
            QRegularExpression re(pattern);
            if (re.match(firstLine).hasMatch()) {
                return &lang;
            }
        }
    }
    return nullptr;
}

// ---------------------------------------------------------------------------
// Language catalogue — all supported languages
// ---------------------------------------------------------------------------

void LanguageManager::registerLanguages()
{
    m_languages = {
        // -- C / C++ ---------------------------------------------------------
        { "c",          "C",              "text/x-csrc",         "clike",
          {},           {"c", "h"},       {} },
        { "cpp",        "C++",            "text/x-c++src",       "clike",
          {},           {"cpp", "cxx", "cc", "hpp", "hxx", "hh", "h++", "ipp", "tpp", "cppm", "ixx"},
          {} },

        // -- C# / Objective-C ------------------------------------------------
        { "csharp",     "C#",             "text/x-csharp",       "clike",
          {},           {"cs", "csx"},    {} },
        { "objective-c","Objective-C",    "text/x-objectivec",   "clike",
          {},           {"m", "mm"},      {} },

        // -- Java / Kotlin / Scala / Groovy ----------------------------------
        { "java",       "Java",           "text/x-java",         "clike",
          {},           {"java"},         {} },
        { "kotlin",     "Kotlin",         "text/x-kotlin",       "clike",
          {},           {"kt", "kts"},    {} },
        { "scala",      "Scala",          "text/x-scala",        "clike",
          {},           {"scala", "sc"},  {} },
        { "groovy",     "Groovy",         "text/x-groovy",       "clike",
          {},           {"groovy", "gradle"}, {} },

        // -- JavaScript / TypeScript -----------------------------------------
        { "javascript", "JavaScript",     "text/javascript",     "clike",
          {},           {"js", "mjs", "cjs", "jsx"}, {} },
        { "typescript", "TypeScript",     "text/typescript",     "clike",
          {},           {"ts", "tsx", "mts", "cts"}, {} },

        // -- Python ----------------------------------------------------------
        { "python",     "Python",         "text/x-python",       "python",
          {},           {"py", "pyw", "pyi", "pyx", "pxd"},
          { R"(^#!.*python)" } },

        // -- Rust ------------------------------------------------------------
        { "rust",       "Rust",           "text/x-rustsrc",      "rust",
          {},           {"rs"},           {} },

        // -- Go --------------------------------------------------------------
        { "go",         "Go",             "text/x-go",           "go",
          {},           {"go"},           {} },

        // -- Swift -----------------------------------------------------------
        { "swift",      "Swift",          "text/x-swift",        "clike",
          {},           {"swift"},        {} },

        // -- Dart ------------------------------------------------------------
        { "dart",       "Dart",           "text/x-dart",         "clike",
          {},           {"dart"},         {} },

        // -- PHP -------------------------------------------------------------
        { "php",        "PHP",            "application/x-httpd-php", "php",
          {},           {"php", "phtml", "php3", "php4", "php5", "php7", "phps"},
          { R"(^<\?php)" } },

        // -- Ruby ------------------------------------------------------------
        { "ruby",       "Ruby",           "text/x-ruby",         "ruby",
          {"Gemfile", "Rakefile", "Vagrantfile"},
          {"rb", "rbw", "rake", "gemspec"},
          { R"(^#!.*ruby)" } },

        // -- Perl ------------------------------------------------------------
        { "perl",       "Perl",           "text/x-perl",         "perl",
          {},           {"pl", "pm", "pod", "t"},
          { R"(^#!.*perl)" } },

        // -- Lua -------------------------------------------------------------
        { "lua",        "Lua",            "text/x-lua",          "lua",
          {},           {"lua"},
          { R"(^#!.*lua)" } },

        // -- Shell / Bash ----------------------------------------------------
        { "shell",      "Shell",          "text/x-sh",           "shell",
          {},           {"sh", "bash", "zsh", "fish", "ksh", "csh", "tcsh"},
          { R"(^#!.*/(?:ba)?sh)", R"(^#!.*/zsh)", R"(^#!.*/fish)" } },

        // -- PowerShell ------------------------------------------------------
        { "powershell", "PowerShell",     "text/x-powershell",   "powershell",
          {},           {"ps1", "psm1", "psd1"}, {} },

        // -- SQL -------------------------------------------------------------
        { "sql",        "SQL",            "text/x-sql",          "sql",
          {},           {"sql", "ddl", "dml"}, {} },

        // -- HTML / XML / SVG ------------------------------------------------
        { "html",       "HTML",           "text/html",           "htmlmixed",
          {},           {"html", "htm", "xhtml"}, {} },
        { "xml",        "XML",            "application/xml",     "xml",
          {},           {"xml", "xsl", "xslt", "xsd", "svg", "rss", "atom", "plist", "wsdl"},
          { R"(^<\?xml)" } },

        // -- CSS / SCSS / LESS -----------------------------------------------
        { "css",        "CSS",            "text/css",            "css",
          {},           {"css"},          {} },
        { "scss",       "SCSS",           "text/x-scss",         "css",
          {},           {"scss"},         {} },
        { "less",       "LESS",           "text/x-less",         "css",
          {},           {"less"},         {} },

        // -- JSON / YAML / TOML / INI ---------------------------------------
        { "json",       "JSON",           "application/json",    "javascript",
          {},           {"json", "jsonc", "json5", "geojson"}, {} },
        { "yaml",       "YAML",           "text/x-yaml",         "yaml",
          {},           {"yml", "yaml"},  {} },
        { "toml",       "TOML",           "text/x-toml",         "toml",
          {},           {"toml"},         {} },
        { "ini",        "INI",            "text/x-ini",          "properties",
          {},           {"ini", "cfg", "conf", "properties"}, {} },

        // -- Markdown / reStructuredText ------------------------------------
        { "markdown",   "Markdown",       "text/x-markdown",     "markdown",
          {},           {"md", "markdown", "mkd", "mkdn"}, {} },
        { "rst",        "reStructuredText","text/x-rst",         "rst",
          {},           {"rst", "rest"},  {} },

        // -- LaTeX -----------------------------------------------------------
        { "latex",      "LaTeX",          "text/x-latex",        "stex",
          {},           {"tex", "latex", "sty", "cls", "bib"}, {} },

        // -- Dockerfile / Docker Compose ------------------------------------
        { "dockerfile", "Dockerfile",     "text/x-dockerfile",   "dockerfile",
          {"Dockerfile"},
          {"dockerfile"}, {} },

        // -- Makefile --------------------------------------------------------
        { "makefile",   "Makefile",       "text/x-makefile",     "cmake",
          {"Makefile", "makefile", "GNUmakefile"},
          {"mk", "mak"}, {} },

        // -- CMake -----------------------------------------------------------
        { "cmake",      "CMake",          "text/x-cmake",        "cmake",
          {"CMakeLists.txt"},
          {"cmake"},    {} },

        // -- Protobuf --------------------------------------------------------
        { "protobuf",   "Protocol Buffers","text/x-protobuf",    "protobuf",
          {},           {"proto"},        {} },

        // -- GraphQL ---------------------------------------------------------
        { "graphql",    "GraphQL",        "application/graphql", "graphql",
          {},           {"graphql", "gql"}, {} },

        // -- Haskell ---------------------------------------------------------
        { "haskell",    "Haskell",        "text/x-haskell",      "haskell",
          {},           {"hs", "lhs"},    {} },

        // -- Elixir / Erlang -------------------------------------------------
        { "elixir",     "Elixir",         "text/x-elixir",       "elixir",
          {},           {"ex", "exs"},    {} },
        { "erlang",     "Erlang",         "text/x-erlang",       "erlang",
          {},           {"erl", "hrl"},   {} },

        // -- Clojure ---------------------------------------------------------
        { "clojure",    "Clojure",        "text/x-clojure",      "clojure",
          {},           {"clj", "cljs", "cljc", "edn"}, {} },

        // -- R ---------------------------------------------------------------
        { "r",          "R",              "text/x-rsrc",         "r",
          {},           {"r", "R", "rmd", "Rmd"}, {} },

        // -- Julia -----------------------------------------------------------
        { "julia",      "Julia",          "text/x-julia",        "julia",
          {},           {"jl"},           {} },

        // -- Zig -------------------------------------------------------------
        { "zig",        "Zig",            "text/x-zig",          "zig",
          {},           {"zig"},          {} },

        // -- Nim -------------------------------------------------------------
        { "nim",        "Nim",            "text/x-nim",          "nim",
          {},           {"nim", "nims", "nimble"}, {} },

        // -- V ---------------------------------------------------------------
        { "vlang",      "V",              "text/x-v",            "vlang",
          {},           {"v"},            {} },

        // -- Assembly --------------------------------------------------------
        { "asm",        "Assembly",       "text/x-asm",          "gas",
          {},           {"asm", "s", "S"}, {} },

        // -- Plain text (fallback) -------------------------------------------
        { "plaintext",  "Plain Text",     "text/plain",          "",
          {},           {"txt", "text", "log"}, {} },
    };
}

} // namespace NovaPad::Editor
