/**
 * @file command_line_parser.cpp
 * @brief Implementation of the command-line parser factory.
 *
 * Every flag recognised by NovaPad is defined here.  Adding a new flag
 * is a single-step operation: add the QCommandLineOption and document it.
 */

#include "command_line_parser.h"

#include <QCoreApplication>

namespace NovaPad::Core {

QSharedPointer<QCommandLineParser>
CommandLineParser::create(const QStringList &arguments)
{
    auto parser = QSharedPointer<QCommandLineParser>::create();

    parser->setApplicationDescription(
        QCoreApplication::tr("NovaPad — a modern programmer's notebook"));
    parser->addHelpOption();
    parser->addVersionOption();

    // -- Custom options ------------------------------------------------------

    parser->addOption({
        QStringLiteral("new-window"),
        QCoreApplication::tr("Open files in a new window instead of reusing an existing one.")
    });

    parser->addOption({
        QStringLiteral("allow-root"),
        QCoreApplication::tr("Allow running NovaPad as the root user (not recommended).")
    });

    parser->addOption({
        QStringLiteral("print-debug-info"),
        QCoreApplication::tr("Print environment diagnostic information and exit.")
    });

    parser->addOption({
        QStringLiteral("line"),
        QCoreApplication::tr("Move the cursor to line <number> after opening the file."),
        QCoreApplication::tr("number")
    });

    parser->addOption({
        QStringLiteral("column"),
        QCoreApplication::tr("Move the cursor to column <number> after opening the file."),
        QCoreApplication::tr("number")
    });

    parser->addOption({
        QStringLiteral("lang"),
        QCoreApplication::tr("Force a specific syntax language (e.g. \"python\", \"cpp\")."),
        QCoreApplication::tr("language")
    });

    parser->addOption({
        QStringLiteral("encoding"),
        QCoreApplication::tr("Open the file using the specified encoding (e.g. \"UTF-8\")."),
        QCoreApplication::tr("encoding")
    });

    // Positional arguments: file paths to open
    parser->addPositionalArgument(
        QStringLiteral("files"),
        QCoreApplication::tr("Files to open."),
        QStringLiteral("[files...]"));

    // Process the arguments (does not call exit() on error in library mode)
    parser->parse(arguments);

    return parser;
}

} // namespace NovaPad::Core
