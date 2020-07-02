#include <iostream>
#include "Debug.h"
namespace Callback{
    void glDebugOutput(
            [[maybe_unused]] GLenum source,
            [[maybe_unused]] GLenum type,
            [[maybe_unused]] unsigned int id,
            [[maybe_unused]] GLenum severity,
            [[maybe_unused]] GLsizei length,
            [[maybe_unused]] const char *message,
            [[maybe_unused]] const void *userParam
    ) {
        // ignore non-significant error/warning codes
//            if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

        std::cout << "---------------" << newline;
        std::cout << "Debug message (" << id << "): " << message << "\n";

        switch (source) {
            case GL_DEBUG_SOURCE_API:
                std::cout << "Source: API\n";
                break;
            case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
                std::cout << "Source: Window System\n";
                break;
            case GL_DEBUG_SOURCE_SHADER_COMPILER:
                std::cout << "Source: Shader Compiler\n";
                break;
            case GL_DEBUG_SOURCE_THIRD_PARTY:
                std::cout << "Source: Third Party\n";
                break;
            case GL_DEBUG_SOURCE_APPLICATION:
                std::cout << "Source: Application\n";
                break;
            case GL_DEBUG_SOURCE_OTHER:
                std::cout << "Source: Other\n";
                break;
        }
        switch (type) {
            case GL_DEBUG_TYPE_ERROR:
                std::cout << "Type: Error\n";
                break;
            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
                std::cout << "Type: Deprecated Behaviour\n";
                break;
            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
                std::cout << "Type: Undefined Behaviour\n";
                break;
            case GL_DEBUG_TYPE_PORTABILITY:
                std::cout << "Type: Portability\n";
                break;
            case GL_DEBUG_TYPE_PERFORMANCE:
                std::cout << "Type: Performance\n";
                break;
            case GL_DEBUG_TYPE_MARKER:
                std::cout << "Type: Marker\n";
                break;
            case GL_DEBUG_TYPE_PUSH_GROUP:
                std::cout << "Type: Push Group\n";
                break;
            case GL_DEBUG_TYPE_POP_GROUP:
                std::cout << "Type: Pop Group\n";
                break;
            case GL_DEBUG_TYPE_OTHER:
                std::cout << "Type: Other\n";
                break;
        }
        switch (severity) {
            case GL_DEBUG_SEVERITY_HIGH:
                std::cout << "Severity: high\n";
                break;
            case GL_DEBUG_SEVERITY_MEDIUM:
                std::cout << "Severity: medium\n";
                break;
            case GL_DEBUG_SEVERITY_LOW:
                std::cout << "Severity: low\n";
                break;
            case GL_DEBUG_SEVERITY_NOTIFICATION:
                std::cout << "Severity: notification\n";
                break;
        }
        std::cout.flush();
    }

    void errorCallback(int error_code, const char *description) {
        std::cerr << "GLFW ERROR: Error Code:" << error_code << "\n";
        std::cerr << "            Description:" << description << std::endl;
    }
}