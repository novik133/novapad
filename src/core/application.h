/**
 * @file application.h
 * @brief NovaPad application class — extends QApplication with app-wide services.
 *
 * Owns the lifecycle of shared singletons (settings, theme manager, etc.)
 * and provides convenience accessors used throughout the codebase.
 */

#ifndef NOVAPAD_CORE_APPLICATION_H
#define NOVAPAD_CORE_APPLICATION_H

#include <QApplication>

namespace NovaPad::Core {

/**
 * @class Application
 * @brief Thin QApplication wrapper that bootstraps NovaPad-specific subsystems.
 *
 * Currently this is intentionally lightweight.  Heavier setup (session restore,
 * extension loading) happens in main() so that the order of operations remains
 * explicit and easy to reason about.
 */
class Application : public QApplication
{
    Q_OBJECT

public:
    /**
     * @brief Constructs the application.
     * @param argc  Argument count (passed through to QApplication).
     * @param argv  Argument vector (passed through to QApplication).
     */
    Application(int &argc, char **argv);
    ~Application() override = default;

    /**
     * @brief Returns the singleton Application instance.
     *
     * Equivalent to `qApp` but with the correct derived type.
     */
    [[nodiscard]] static Application *instance();
};

} // namespace NovaPad::Core

#endif // NOVAPAD_CORE_APPLICATION_H
