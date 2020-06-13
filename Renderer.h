#pragma once
#ifndef GAME_RENDERER_H
#define GAME_RENDERER_H

#include <SDL2/SDL.h>
#include <utility>
#include "Shader.h"
#include "Face.h"

class Renderer{
    public:
        Renderer();
        ~Renderer();

        const std::pair<Sint32, Sint32> &getResolution() const;
private:
        bool init();

        bool setup();
        void loop();
        void destruct();

        void Clear();
        void Render();
        void Present();

        void nextFrame(){
            Clear();
            Render();
            Present();
        };

        Face<float>* triangle_vertices;

        SDL_Window* win;
        SDL_GLContext GL_Context;

        Shader* shader;


        /**
         * Stores the current size of the window
         */
        std::pair<Sint32, Sint32> resolution{1000, 1000};


};
#endif //GAME_RENDERER_H
