#ifndef GAME_RENDERER_H
#define GAME_RENDERER_H

#include <SDL2/SDL.h>
#include <utility>
#include "Shader.h"
#include "Camera.h"

namespace Renderer{
    bool run();
    [[nodiscard]] const Shader*const getShader();
    [[nodiscard]] const Camera*const getCamera();
};
#endif //GAME_RENDERER_H
