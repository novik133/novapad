/**
 * @file code_folding_manager.h
 * @brief Code folding engine that detects and manages collapsible regions.
 *
 * Identifies foldable regions based on indentation levels and explicit
 * delimiters (braces, begin/end markers).  Each foldable region is
 * represented as a FoldRegion that maps a start block to an end block.
 *
 * The gutter renders fold indicators (▶ / ▼) which the user can click
 * to toggle visibility of the enclosed lines.
 */

#ifndef NOVAPAD_EDITOR_CODE_FOLDING_MANAGER_H
#define NOVAPAD_EDITOR_CODE_FOLDING_MANAGER_H

#include <QObject>
#include <QSet>
#include <QTextBlock>
#include <vector>

namespace NovaPad::Editor {

class CodeEditorWidget; // forward declaration

/**
 * @struct FoldRegion
 * @brief Describes a contiguous range of blocks that can be collapsed.
 */
struct FoldRegion
{
    int startBlock = -1;  ///< First block of the region (the "fold header")
    int endBlock   = -1;  ///< Last block of the region (inclusive)
    bool collapsed = false;
};

/**
 * @class CodeFoldingManager
 * @brief Detects foldable regions and manages their collapsed/expanded state.
 *
 * ## Detection strategy
 * For brace-delimited languages (C, C++, Java, JS, …), regions are bounded
 * by matching `{` / `}` pairs.  For indentation-based languages (Python,
 * YAML), regions are determined by indentation changes.  A fallback
 * heuristic uses indentation for all other languages.
 */
class CodeFoldingManager : public QObject
{
    Q_OBJECT

public:
    explicit CodeFoldingManager(CodeEditorWidget *editor, QObject *parent = nullptr);
    ~CodeFoldingManager() override = default;

    /** @brief Re-scans the document and rebuilds the fold region list. */
    void updateFoldRegions();

    /** @brief Returns all detected fold regions. */
    [[nodiscard]] const std::vector<FoldRegion> &regions() const;

    /** @brief Toggles the fold state of the region starting at @p blockNumber. */
    void toggleFold(int blockNumber);

    /** @brief Collapses all foldable regions. */
    void foldAll();

    /** @brief Expands all foldable regions. */
    void unfoldAll();

    /** @brief Returns true if the block at @p blockNumber is a fold header. */
    [[nodiscard]] bool isFoldHeader(int blockNumber) const;

    /** @brief Returns true if the region starting at @p blockNumber is collapsed. */
    [[nodiscard]] bool isFolded(int blockNumber) const;

private:
    /** @brief Detects fold regions using brace matching ({…}). */
    void detectBraceRegions();

    /** @brief Detects fold regions using indentation levels. */
    void detectIndentRegions();

    /** @brief Hides or shows the blocks belonging to a region. */
    void applyFoldState(const FoldRegion &region);

    CodeEditorWidget          *m_editor = nullptr;
    std::vector<FoldRegion>    m_regions;
    QSet<int>                  m_foldHeaders; ///< Quick lookup for fold header blocks
};

} // namespace NovaPad::Editor

#endif // NOVAPAD_EDITOR_CODE_FOLDING_MANAGER_H
