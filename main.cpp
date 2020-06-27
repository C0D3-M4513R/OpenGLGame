#include <iostream>
#include <fstream>
#include <sstream>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

//debugger callback
void GLAPIENTRY glDebugOutput(GLenum source,
                              GLenum type,
                              unsigned int id,
                              GLenum severity,
                              GLsizei length,
                              const char *message,
                              const void *userParam)
{
    // ignore non-significant error/warning codes
//            if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return;
    SDL_LogPriority prioPrior = SDL_LogGetPriority(SDL_LOG_CATEGORY_APPLICATION);
    switch (severity){
        case GL_DEBUG_SEVERITY_HIGH:         SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION,SDL_LOG_PRIORITY_CRITICAL); break;
        case GL_DEBUG_SEVERITY_MEDIUM:       SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION,SDL_LOG_PRIORITY_ERROR); break;
        case GL_DEBUG_SEVERITY_LOW:          SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION,SDL_LOG_PRIORITY_WARN); break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION,SDL_LOG_PRIORITY_INFO); break;
    }

    SDL_Log("---------------");
    SDL_Log("Debug message (%u): %s",id,message);

    switch (source){
        case GL_DEBUG_SOURCE_API:             SDL_Log("Source: API"); break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   SDL_Log("Source: Window System"); break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: SDL_Log("Source: Shader Compiler"); break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     SDL_Log("Source: Third Party"); break;
        case GL_DEBUG_SOURCE_APPLICATION:     SDL_Log("Source: Application"); break;
        case GL_DEBUG_SOURCE_OTHER:           SDL_Log("Source: Other"); break;
    }
    switch (type){
        case GL_DEBUG_TYPE_ERROR:               SDL_Log("Type: Error"); break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: SDL_Log("Type: Deprecated Behaviour"); break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  SDL_Log("Type: Undefined Behaviour"); break;
        case GL_DEBUG_TYPE_PORTABILITY:         SDL_Log("Type: Portability"); break;
        case GL_DEBUG_TYPE_PERFORMANCE:         SDL_Log("Type: Performance"); break;
        case GL_DEBUG_TYPE_MARKER:              SDL_Log("Type: Marker"); break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          SDL_Log("Type: Push Group"); break;
        case GL_DEBUG_TYPE_POP_GROUP:           SDL_Log("Type: Pop Group"); break;
        case GL_DEBUG_TYPE_OTHER:               SDL_Log("Type: Other"); break;
    }
    switch (severity){
        case GL_DEBUG_SEVERITY_HIGH:         SDL_Log("Severity: high"); break;
        case GL_DEBUG_SEVERITY_MEDIUM:       SDL_Log("Severity: medium"); break;
        case GL_DEBUG_SEVERITY_LOW:          SDL_Log("Severity: low"); break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: SDL_Log("Severity: notification"); break;
    }
    SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION,prioPrior);
}

GLuint loadShader(const char* filePath, GLuint shaderType){
    //create/compile Shader
    GLuint shader = glCreateShader(shaderType);
    // Open the file, but freak out if not valid.
    std::ifstream file(filePath);

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string fileContentsStr = buffer.str();
    const char* fileContents = fileContentsStr.c_str();

    glShaderSource(shader, 1, &fileContents, nullptr);
    glCompileShader(shader);

    //Error handling!
    GLint compileSucceeded = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileSucceeded);
    if(compileSucceeded == GL_FALSE){
        GLint errorLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &errorLength);
        GLchar* errorLog = new GLchar[errorLength];
        glGetShaderInfoLog(shader, errorLength, &errorLength, errorLog);
        SDL_LogCritical(SDL_LOG_CATEGORY_SYSTEM,"Error compiling shader: %s",errorLog);
        exit(-1);
    }
    SDL_LogVerbose(SDL_LOG_CATEGORY_SYSTEM,"Shader compiled!");
    return shader;
}

GLuint createProgramm(){
    GLuint vertShader = loadShader("resources/Simple.vert",GL_VERTEX_SHADER);
    GLuint fragShader = loadShader("resources/Simple.frag",GL_FRAGMENT_SHADER);

    GLuint program=glCreateProgram();
    glAttachShader(program,vertShader);
    glAttachShader(program,fragShader);
    // Link the program.
    glLinkProgram(program);
    //Error handling
    GLint linkSucceeded = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &linkSucceeded);
    if(linkSucceeded == GL_FALSE){
        GLint errorLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &errorLength);
        GLchar* errorLog = new GLchar[errorLength];
        glGetProgramInfoLog(program, errorLength, &errorLength, errorLog);
        SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION,"Error linking shader program: %s",errorLog);
        exit(-1);
    }
    SDL_LogInfo(SDL_LOG_CATEGORY_SYSTEM,"Shader Programm Link completed!");
    // Detach shaders after a successful link.
    glDetachShader(program, vertShader);
    glDetachShader(program, fragShader);

    return program;
}


void createVABBO(GLuint& vao, GLuint* vbo){
    //Those arrays should be deleted by Face!
    const unsigned int n = 4*3;
    auto *vertices = new float[n];
    vertices[0]=-0.5f;
    vertices[1]=-0.5f;
    vertices[2]=0.0f;

    vertices[3]=0.5f;
    vertices[4]=-0.5f;
    vertices[5]=0.0f;

    vertices[6]=-0.5f;
    vertices[7]=0.5f;
    vertices[8]=0.0f;

    vertices[9]=0.5f;
    vertices[10]=0.5f;
    vertices[11]=0.0f;
    auto *color = new float[n];
    color[0]=0.f;
    color[1]=1.f;
    color[2]=0.0f;

    color[3]=0.f;
    color[4]=0.f;
    color[5]=1.0f;

    color[6]=1.f;
    color[7]=0.f;
    color[8]=0.0f;

    color[9]=1.f;
    color[10]=1.f;
    color[11]=1.0f;


    //create vaos
    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);
    //create vbos
    glGenBuffers(2,vbo);
    //put vert data
    glBindBuffer(GL_ARRAY_BUFFER,vbo[0]);
    glBufferData(GL_ARRAY_BUFFER,n * sizeof(float),vertices,GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0, nullptr);
    glEnableVertexAttribArray(0);
    //put color data
    glBindBuffer(GL_ARRAY_BUFFER,vbo[1]);
    glBufferData(GL_ARRAY_BUFFER,n * sizeof(float),color,GL_STATIC_DRAW);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0, nullptr);
    glEnableVertexAttribArray(1);
    //unbind buffer
    glBindBuffer(GL_ARRAY_BUFFER,0);
    delete[] vertices;
    delete[] color;
}

int main(int argc, char *argv[]) {
    if (SDL_InitSubSystem(SDL_INIT_VIDEO) != 0) {
        SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_CRITICAL);
        SDL_Log("error initializing SDL: %s\n", SDL_GetError());
        return false;
    }
    SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_DEBUG);
    SDL_Log("Hello World!");

    //set default logging level to verbose
    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);


    // Tell SDL we want to use OpenGL 3.3
    // These attributes must be set before creating the window.
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    // Require hardware acceleration.
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);

    // We request from OpenGL at least 8-bits per channel for the color buffer and depth buffer
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,8);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_NO_ERROR,0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS,SDL_GL_CONTEXT_DEBUG_FLAG);


    SDL_Window* win = SDL_CreateWindow("GAME", // creates a window
                           SDL_WINDOWPOS_CENTERED,
                           SDL_WINDOWPOS_CENTERED,
                           1000, 1000, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
    if (win == nullptr) return false;

    // Create OpenGL context.
    SDL_GLContext GL_Context = SDL_GL_CreateContext(win);
    if (GL_Context == nullptr) return false;

    // Initialize GLEW.
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) return false;

    //use debugger callback
    int flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(glDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }

    //make my shader program
    GLuint program=createProgramm();

    GLuint vao,vbo[2];
    createVABBO(vao,vbo);

        const glm::mat4 v = glm::lookAt(glm::vec3(0,0,0),glm::vec3(0,0,0.5),glm::vec3(0,1,0));
    glm::mat4 m = glm::translate(glm::identity<glm::mat4>(),glm::vec3(0.5f,0.5f,0.f));


    glm::mat4 mvp;
    GLuint model;

    //prevent the os thinking, that we are doing nothing (by querying events)
    while(true){
        SDL_Event event;
        // Events mangement
        while (SDL_PollEvent(&event)) {
            if(event.type==SDL_QUIT) {
                glDeleteBuffers(2,vbo);
                glDeleteVertexArrays(1,&vao);
                glDeleteProgram(program);
                SDL_GL_DeleteContext(GL_Context);
                SDL_DestroyWindow(win);
                SDL_QuitSubSystem(SDL_INIT_VIDEO);
                return 1;
            }
        }

        //bind model matrix
        mvp = glm::translate(glm::identity<glm::mat4>(),glm::vec3(.5,.5,0));
        glUseProgram(program);
        model = glGetUniformLocation(program,"model");
        glUniformMatrix4fv(model,1,GL_FALSE,&mvp[0][0]);

        //draw
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //1st triangle
        glDrawArrays(GL_TRIANGLES, 0, 4);

        mvp = glm::translate(glm::identity<glm::mat4>(),glm::vec3(0,0,0));
        glUseProgram(program);
        model = glGetUniformLocation(program,"model");
        glUniformMatrix4fv(model,1,GL_FALSE,&mvp[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 4);

        SDL_GL_SwapWindow(win);

    }

}