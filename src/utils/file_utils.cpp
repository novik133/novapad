/**
 * @file file_utils.cpp
 * @brief Implementation of file I/O utilities and the DocEngine.
 *
 * Encoding detection uses a multi-step approach:
 *   1. Check for a BOM (UTF-8, UTF-16 BE/LE, UTF-32 BE/LE).
 *   2. Try QTextCodec::codecForUtfText() for a statistical guess.
 *   3. Fall back to UTF-8 (the most common encoding for source code).
 */

#include "file_utils.h"

#include "utils/encoding_detector.h"

#include <QFile>
#include <QFileInfo>

namespace NovaPad::Utils {

// ---------------------------------------------------------------------------
// readFile
// ---------------------------------------------------------------------------

FileLoadResult FileUtils::readFile(const QString &filePath, QTextCodec *codec)
{
    FileLoadResult result;

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        result.errorMessage = file.errorString();
        return result;
    }

    const QByteArray rawData = file.readAll();
    file.close();

    // Detect encoding and BOM
    if (!codec) {
        auto detected = EncodingDetector::detect(rawData);
        codec         = detected.codec;
        result.hasBom = detected.hasBom;
    }

    if (!codec) {
        codec = QTextCodec::codecForName("UTF-8");
    }

    result.codec = codec;

    // Decode the content
    QTextDecoder decoder(codec);
    result.content = decoder.toUnicode(rawData);

    // Detect EOL style
    result.eolSequence = detectEol(rawData);

    // Normalise line endings to \n for internal representation
    result.content.replace("\r\n", "\n");
    result.content.replace('\r', '\n');

    result.success = true;
    return result;
}

// ---------------------------------------------------------------------------
// writeFile
// ---------------------------------------------------------------------------

bool FileUtils::writeFile(const QString &filePath,
                          const QString &content,
                          QTextCodec *codec,
                          bool writeBom,
                          const QString &eolSequence)
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        return false;
    }

    if (!codec) {
        codec = QTextCodec::codecForName("UTF-8");
    }

    // Normalise line endings to the requested style
    QString normalised = content;
    normalised.replace("\r\n", "\n");
    normalised.replace('\r', '\n');

    if (eolSequence != "\n") {
        normalised.replace('\n', eolSequence);
    }

    // Encode
    QByteArray encoded = codec->fromUnicode(normalised);

    // Prepend BOM if requested
    if (writeBom) {
        const QByteArray bom = EncodingDetector::bomForCodec(codec);
        file.write(bom);
    }

    file.write(encoded);
    file.close();

    return true;
}

// ---------------------------------------------------------------------------
// detectEol
// ---------------------------------------------------------------------------

QString FileUtils::detectEol(const QByteArray &data)
{
    int crlfCount = 0;
    int lfCount   = 0;
    int crCount   = 0;

    for (int i = 0; i < data.size(); ++i) {
        if (data[i] == '\r') {
            if (i + 1 < data.size() && data[i + 1] == '\n') {
                ++crlfCount;
                ++i; // Skip the \n
            } else {
                ++crCount;
            }
        } else if (data[i] == '\n') {
            ++lfCount;
        }
    }

    if (crlfCount >= lfCount && crlfCount >= crCount) {
        return crlfCount > 0 ? QStringLiteral("\r\n") : QStringLiteral("\n");
    }
    if (crCount >= lfCount) {
        return QStringLiteral("\r");
    }
    return QStringLiteral("\n");
}

// ---------------------------------------------------------------------------
// isBinaryFile
// ---------------------------------------------------------------------------

bool FileUtils::isBinaryFile(const QString &path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) return false;

    const QByteArray sample = file.read(8192);
    file.close();

    return sample.contains('\0');
}

// ---------------------------------------------------------------------------
// humanReadableSize
// ---------------------------------------------------------------------------

QString FileUtils::humanReadableSize(qint64 bytes)
{
    if (bytes < 1024)               return QStringLiteral("%1 B").arg(bytes);
    if (bytes < 1024 * 1024)        return QStringLiteral("%1 KB").arg(bytes / 1024.0, 0, 'f', 1);
    if (bytes < 1024 * 1024 * 1024) return QStringLiteral("%1 MB").arg(bytes / (1024.0 * 1024.0), 0, 'f', 1);
    return QStringLiteral("%1 GB").arg(bytes / (1024.0 * 1024.0 * 1024.0), 0, 'f', 2);
}

// ---------------------------------------------------------------------------
// DocEngine
// ---------------------------------------------------------------------------

DocEngine::DocEngine(QObject *parent)
    : QObject(parent)
{
    // A full implementation would create a QFileSystemWatcher here and
    // connect its signals to fileChanged/fileRemoved.
}

} // namespace NovaPad::Utils
