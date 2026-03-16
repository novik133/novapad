/**
 * @file application.cpp
 * @brief Implementation of the NovaPad Application class.
 */

#include "application.h"

namespace NovaPad::Core {

Application::Application(int &argc, char **argv)
    : QApplication(argc, argv)
{
    // High-DPI scaling is automatic in Qt 6 — no explicit AA_ flags required.
    // Additional app-wide initialisation can be placed here as the project grows.
}

Application *Application::instance()
{
    return qobject_cast<Application *>(QCoreApplication::instance());
}

} // namespace NovaPad::Core
