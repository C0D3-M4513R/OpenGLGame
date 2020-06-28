#include <fstream>
#include <iostream>
#include <sstream>
#include "Shader.h"
#include "Renderer.h"
#include <glm/gtc/type_ptr.hpp>


GLuint Shader::LoadAndCompileShaderFromFile(const char* filePath, GLuint shaderType)
{
    // Open the file, but freak out if not valid.
    std::ifstream file(filePath);
    if(!file.good())
    {
        std::cout << "Couldn't open shader file for loading: " << filePath << std::endl;
        return GL_NONE;
    }

    // Read the file contents into a char buffer.
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string fileContentsStr = buffer.str();
    const char* fileContents = fileContentsStr.c_str();

    // Create shader, load file contents into it, and compile it.
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &fileContents, nullptr);
    glCompileShader(shader);
    return shader;
}

bool Shader::IsShaderCompiled(GLuint shader)
{
    // Ask GL whether compile succeeded for this shader.
    GLint compileSucceeded = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileSucceeded);

    // If not, we'll output the error log and fail.
    if(compileSucceeded == GL_FALSE)
    {
        GLint errorLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &errorLength);

        GLchar* errorLog = new GLchar[errorLength];
        glGetShaderInfoLog(shader, errorLength, &errorLength, errorLog);

        std::cout << "Error compiling shader: " << errorLog << std::endl;
        delete[] errorLog;
        return false;
    }

    // GL reports the compilation was successful!
    return true;
}

Shader::Shader(const char* vertShaderPath, const char* fragShaderPath)
{
    // Compile default shader program.
    GLuint vertexShader = LoadAndCompileShaderFromFile(vertShaderPath, GL_VERTEX_SHADER);
    GLuint fragmentShader = LoadAndCompileShaderFromFile(fragShaderPath, GL_FRAGMENT_SHADER);
    if(!IsShaderCompiled(vertexShader) || !IsShaderCompiled(fragmentShader))
    {
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        error = true;
        return;
    }

    // Assemble shader program.
    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    // Link the program.
    glLinkProgram(program);

    // If linking failed, clean up and return error.
    if(!IsProgramLinked(program))
    {
        glDeleteProgram(program);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        error = true;
        return;
    }

    // Detach shaders after a successful link.
    glDetachShader(program, vertexShader);
    glDetachShader(program, fragmentShader);
}

bool Shader::IsProgramLinked(GLuint program)
{
    // Ask GL whether link succeeded for this program.
    GLint linkSucceeded = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &linkSucceeded);

    // If not, we'll output the error log and fail.
    if(linkSucceeded == GL_FALSE)
    {
        GLint errorLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &errorLength);

        GLchar* errorLog = new GLchar[errorLength];
        glGetProgramInfoLog(program, errorLength, &errorLength, errorLog);

        std::cout << "Error linking shader program: " << errorLog << std::endl;
        delete[] errorLog;
        return false;
    }

    // GL reports the linking was successful!
    return true;
}

Shader::~Shader()
{
    if(program != GL_NONE)
    {
        glDeleteProgram(program);
    }
}

bool Shader::IsGood()
{
    return !error;
}

void Shader::applyMVP(glm::mat4 model) const {
    //TODO::Projection Matrix
//    glm::mat4 mvp = Renderer::getCamera()->view() * model;
    glm::mat4 mvp = model;
    const float* mvpPointer = glm::value_ptr(mvp);

#ifdef DEBUG
    for(unsigned int i = 0; i < 16; i++){
        SDL_LogVerbose(SDL_LOG_CATEGORY_SYSTEM,"MVP Matrix[%u]: %f",i,mvpPointer[i]);
    }
#endif


    glUseProgram(program);
    GLuint MatrixID = glGetUniformLocation(program, "mvp");
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, mvpPointer);

    glUseProgram(0);
}
