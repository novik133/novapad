/**
 * @file code_folding_manager.cpp
 * @brief Implementation of the code folding engine.
 *
 * Two detection strategies are supported:
 *   1. Brace-based — scans for matching `{` / `}` pairs (C-family languages).
 *   2. Indent-based — uses indentation depth changes (Python, YAML, etc.).
 *
 * The manager caches the fold regions and only rescans when the document
 * content actually changes, avoiding redundant work on every keystroke.
 */

#include "code_folding_manager.h"
#include "code_editor_widget.h"

#include <QTextBlock>
#include <QTextDocument>
#include <stack>

namespace NovaPad::Editor {

// ---------------------------------------------------------------------------
// Construction
// ---------------------------------------------------------------------------

CodeFoldingManager::CodeFoldingManager(CodeEditorWidget *editor, QObject *parent)
    : QObject(parent ? parent : editor)
    , m_editor(editor)
{
    // Rescan fold regions whenever the document structure changes
    connect(m_editor->document(), &QTextDocument::contentsChanged,
            this, &CodeFoldingManager::updateFoldRegions);
}

// ---------------------------------------------------------------------------
// Public API
// ---------------------------------------------------------------------------

const std::vector<FoldRegion> &CodeFoldingManager::regions() const
{
    return m_regions;
}

void CodeFoldingManager::updateFoldRegions()
{
    m_regions.clear();
    m_foldHeaders.clear();

    if (!m_editor || !m_editor->document()) return;

    // Use brace-based detection by default; fall back to indent-based
    // for languages that don't use braces (Python, YAML, etc.)
    const Language *lang = m_editor->currentLanguage();
    const bool useBraces = !lang
                           || lang->id != "python"
                           || lang->id != "yaml"
                           || lang->id != "nim";

    if (useBraces) {
        detectBraceRegions();
    } else {
        detectIndentRegions();
    }

    // Build the quick-lookup set of fold header block numbers
    for (const auto &region : m_regions) {
        m_foldHeaders.insert(region.startBlock);
    }
}

void CodeFoldingManager::toggleFold(int blockNumber)
{
    for (auto &region : m_regions) {
        if (region.startBlock == blockNumber) {
            region.collapsed = !region.collapsed;
            applyFoldState(region);
            return;
        }
    }
}

void CodeFoldingManager::foldAll()
{
    for (auto &region : m_regions) {
        region.collapsed = true;
        applyFoldState(region);
    }
}

void CodeFoldingManager::unfoldAll()
{
    for (auto &region : m_regions) {
        region.collapsed = false;
        applyFoldState(region);
    }
}

bool CodeFoldingManager::isFoldHeader(int blockNumber) const
{
    return m_foldHeaders.contains(blockNumber);
}

bool CodeFoldingManager::isFolded(int blockNumber) const
{
    for (const auto &region : m_regions) {
        if (region.startBlock == blockNumber) {
            return region.collapsed;
        }
    }
    return false;
}

// ---------------------------------------------------------------------------
// Brace-based detection
// ---------------------------------------------------------------------------

void CodeFoldingManager::detectBraceRegions()
{
    const QTextDocument *doc = m_editor->document();
    std::stack<int> braceStack; // stores block numbers of opening braces

    QTextBlock block = doc->begin();
    while (block.isValid()) {
        const QString text = block.text();

        for (const QChar &ch : text) {
            if (ch == QLatin1Char('{')) {
                braceStack.push(block.blockNumber());
            } else if (ch == QLatin1Char('}') && !braceStack.empty()) {
                const int startBlock = braceStack.top();
                braceStack.pop();

                // Only create a fold region if it spans at least 2 lines
                const int endBlock = block.blockNumber();
                if (endBlock - startBlock >= 1) {
                    FoldRegion region;
                    region.startBlock = startBlock;
                    region.endBlock   = endBlock;
                    region.collapsed  = false;
                    m_regions.push_back(region);
                }
            }
        }

        block = block.next();
    }
}

// ---------------------------------------------------------------------------
// Indent-based detection
// ---------------------------------------------------------------------------

void CodeFoldingManager::detectIndentRegions()
{
    const QTextDocument *doc = m_editor->document();
    const int blockCount = doc->blockCount();

    // Helper: compute the indentation level of a block (number of leading spaces/tabs)
    auto indentLevel = [](const QTextBlock &blk) -> int {
        const QString text = blk.text();
        int level = 0;
        for (const QChar &ch : text) {
            if (ch == QLatin1Char(' '))       ++level;
            else if (ch == QLatin1Char('\t'))  level += 4;
            else break;
        }
        return level;
    };

    // Scan through blocks looking for indent increases
    struct PendingRegion {
        int startBlock;
        int indentLvl;
    };
    std::stack<PendingRegion> stack;

    QTextBlock block = doc->begin();
    while (block.isValid()) {
        const int blockNum = block.blockNumber();
        const QString text = block.text().trimmed();

        if (text.isEmpty()) {
            block = block.next();
            continue;
        }

        const int level = indentLevel(block);

        // Close any pending regions whose indent is >= current level
        while (!stack.empty() && stack.top().indentLvl >= level) {
            const auto &top = stack.top();
            if (blockNum - top.startBlock >= 2) {
                FoldRegion region;
                region.startBlock = top.startBlock;
                region.endBlock   = blockNum - 1;
                region.collapsed  = false;
                m_regions.push_back(region);
            }
            stack.pop();
        }

        // Check if the next non-empty block has greater indentation
        QTextBlock nextBlock = block.next();
        while (nextBlock.isValid() && nextBlock.text().trimmed().isEmpty()) {
            nextBlock = nextBlock.next();
        }

        if (nextBlock.isValid() && indentLevel(nextBlock) > level) {
            stack.push({ blockNum, level });
        }

        block = block.next();
    }

    // Close any remaining open regions at end-of-document
    while (!stack.empty()) {
        const auto &top = stack.top();
        if (blockCount - 1 - top.startBlock >= 1) {
            FoldRegion region;
            region.startBlock = top.startBlock;
            region.endBlock   = blockCount - 1;
            region.collapsed  = false;
            m_regions.push_back(region);
        }
        stack.pop();
    }
}

// ---------------------------------------------------------------------------
// Apply fold state — show/hide blocks
// ---------------------------------------------------------------------------

void CodeFoldingManager::applyFoldState(const FoldRegion &region)
{
    QTextDocument *doc = m_editor->document();

    // Hide or show each block within the fold region (excluding the header)
    for (int i = region.startBlock + 1; i <= region.endBlock; ++i) {
        QTextBlock block = doc->findBlockByNumber(i);
        if (block.isValid()) {
            block.setVisible(!region.collapsed);
        }
    }

    // Force the document layout to recalculate
    doc->markContentsDirty(
        doc->findBlockByNumber(region.startBlock).position(),
        doc->findBlockByNumber(region.endBlock).position()
        + doc->findBlockByNumber(region.endBlock).length());

    m_editor->viewport()->update();
}

} // namespace NovaPad::Editor
