#ifndef GAME_RENDERER_H
#define GAME_RENDERER_H

#include <SDL2/SDL.h>
#include <utility>
#include "Shader.h"

template <typename T>
class Face;

namespace Renderer{
    bool run();
    const Shader*const getShader();
};
#endif //GAME_RENDERER_H
