#ifndef GAME_SHADER_H
#define GAME_SHADER_H

#include <GL/glew.h>
#include <glm/mat4x4.hpp>

class Shader
{
public:
    Shader(const char* vertShaderPath, const char* fragShaderPath);
    ~Shader();

    bool IsGood();

    void Activate() const{glUseProgram(program);};
    static void Deactivate(){glUseProgram(0);};

    void applyMVP(glm::mat4 model) const;
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
