#ifndef GAME_RENDERER_H
#define GAME_RENDERER_H

#include <utility>
#include "Shader.h"
#include "Camera.h"

constexpr char const * const newline = "\n";

namespace Renderer{
    bool run();
    [[nodiscard]] const Shader& getShader();
    [[nodiscard]] const Camera& getCamera();
    const std::pair<unsigned int,unsigned int>& getResolution();
    unsigned int getResolutionX();
    unsigned int getResolutionY();

};
#endif //GAME_RENDERER_H
