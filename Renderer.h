#ifndef GAME_RENDERER_H
#define GAME_RENDERER_H

#include <SDL2/SDL.h>
#include <utility>
#include "Shader.h"

namespace Renderer{
    bool run();
    [[nodiscard]] const Shader*const getShader();
};
#endif //GAME_RENDERER_H
