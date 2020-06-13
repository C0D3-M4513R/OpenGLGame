//
// Created by Timon Kayser on 12.06.2020.
//
#pragma once
#ifndef GAME_SHADER_H
#define GAME_SHADER_H

#include <GL/glew.h>

class Shader
{
public:
    Shader(const char* vertShaderPath, const char* fragShaderPath);
    ~Shader();

    bool IsGood();
    void Activate();

private:
    // Handle to the compiled and linked GL shader program.
    GLuint program = GL_NONE;

    // Did we encounter an error during compile/linking?
    bool error = false;

    GLuint LoadAndCompileShaderFromFile(const char* filePath, GLuint shaderType);
    bool IsShaderCompiled(GLuint shader);
    bool IsProgramLinked(GLuint program);
};
#endif //GAME_SHADER_H
