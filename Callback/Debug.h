#ifndef GAME_DEBUG_H
#define GAME_DEBUG_H


#include <GL/glew.h>

namespace Callback {
    void glDebugOutput(
            [[maybe_unused]] GLenum source,
            [[maybe_unused]] GLenum type,
            [[maybe_unused]] unsigned int id,
            [[maybe_unused]] GLenum severity,
            [[maybe_unused]] GLsizei length,
            [[maybe_unused]] const char *message,
            [[maybe_unused]] const void *userParam
    );
    void errorCallback(int error_code, const char *description);
};


#endif //GAME_DEBUG_H
