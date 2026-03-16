/**
 * @file file_utils.h
 * @brief File I/O utilities — reading, writing, encoding detection, and monitoring.
 *
 * This is the C++23 equivalent of Notepadqq's DocEngine, providing a
 * higher-level API for loading and saving documents with proper encoding
 * handling, BOM detection, and filesystem change monitoring.
 */

#ifndef NOVAPAD_UTILS_FILE_UTILS_H
#define NOVAPAD_UTILS_FILE_UTILS_H

#include <QByteArray>
#include <QObject>
#include <QString>
#include <QTextCodec>
#include <QUrl>

#include <optional>

namespace NovaPad::Utils {

/**
 * @struct FileLoadResult
 * @brief Result of a file-load operation.
 */
struct FileLoadResult
{
    bool        success    = false;
    QString     content;
    QTextCodec *codec      = nullptr;
    bool        hasBom     = false;
    QString     eolSequence;   ///< Detected EOL: "\n", "\r\n", or "\r"
    QString     errorMessage;
};

/**
 * @class FileUtils
 * @brief Static utility functions for file I/O operations.
 */
class FileUtils final
{
    Q_DISABLE_COPY_MOVE(FileUtils)
    FileUtils() = delete;

public:
    /**
     * @brief Reads a file, auto-detecting encoding, BOM, and EOL style.
     * @param filePath  Absolute path to the file.
     * @param codec     Optional forced codec (nullptr → auto-detect).
     */
    [[nodiscard]] static FileLoadResult readFile(
        const QString &filePath, QTextCodec *codec = nullptr);

    /**
     * @brief Writes content to a file with the specified encoding.
     * @param filePath    Absolute path to the destination.
     * @param content     Text to write.
     * @param codec       Encoding to use (nullptr → UTF-8).
     * @param writeBom    Whether to prepend a BOM.
     * @param eolSequence EOL characters to normalise to before writing.
     * @return true on success.
     */
    [[nodiscard]] static bool writeFile(
        const QString &filePath,
        const QString &content,
        QTextCodec *codec       = nullptr,
        bool writeBom           = false,
        const QString &eolSequence = QStringLiteral("\n"));

    /**
     * @brief Detects the dominant end-of-line sequence in raw byte data.
     * @return "\r\n", "\r", or "\n" (default).
     */
    [[nodiscard]] static QString detectEol(const QByteArray &data);

    /**
     * @brief Returns true if the file at @p path appears to be binary.
     *
     * Uses a heuristic: if the first 8 KB contain a NUL byte, the file
     * is considered binary.
     */
    [[nodiscard]] static bool isBinaryFile(const QString &path);

    /**
     * @brief Returns a human-readable file size string (e.g. "12.3 KB").
     */
    [[nodiscard]] static QString humanReadableSize(qint64 bytes);
};

/**
 * @class DocEngine
 * @brief Document engine managing file I/O and filesystem monitoring.
 *
 * Wraps FileUtils with QFileSystemWatcher integration and tab-widget
 * awareness.  Used by MainWindow to coordinate file operations with
 * the editor tab widgets.
 */
class DocEngine : public QObject
{
    Q_OBJECT

public:
    explicit DocEngine(QObject *parent = nullptr);
    ~DocEngine() override = default;

    // Placeholder — a full implementation would provide methods like:
    //   loadDocument(), saveDocument(), reloadDocument(), monitorFile(), etc.

signals:
    void fileChanged(const QString &filePath);
    void fileRemoved(const QString &filePath);
};

} // namespace NovaPad::Utils

#endif // NOVAPAD_UTILS_FILE_UTILS_H
