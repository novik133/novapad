/**
 * @file command_line_parser.h
 * @brief Factory for creating a pre-configured QCommandLineParser.
 *
 * Encapsulates all recognised command-line options in a single place so that
 * both the primary instance and forwarded-argument handlers share the same
 * definitions.
 */

#ifndef NOVAPAD_CORE_COMMAND_LINE_PARSER_H
#define NOVAPAD_CORE_COMMAND_LINE_PARSER_H

#include <QCommandLineParser>
#include <QSharedPointer>
#include <QStringList>

namespace NovaPad::Core {

/**
 * @class CommandLineParser
 * @brief Static factory that builds a fully-configured QCommandLineParser.
 */
class CommandLineParser final
{
    Q_DISABLE_COPY_MOVE(CommandLineParser)
    CommandLineParser() = delete;

public:
    /**
     * @brief Creates and processes a parser for the given argument list.
     *
     * The returned parser already contains the parsed results — callers
     * can immediately query `isSet()` / `positionalArguments()`.
     *
     * @param arguments  Typically QApplication::arguments().
     * @return Shared pointer owning the parser.
     */
    [[nodiscard]] static QSharedPointer<QCommandLineParser>
    create(const QStringList &arguments);
};

} // namespace NovaPad::Core

#endif // NOVAPAD_CORE_COMMAND_LINE_PARSER_H
