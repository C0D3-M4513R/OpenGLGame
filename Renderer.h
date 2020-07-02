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
    unsigned int getResolutionX();
    unsigned int getResolutionY();
    void setResolutionX(unsigned int x);
    void setResolutionY(unsigned int y);

};
#endif //GAME_RENDERER_H
