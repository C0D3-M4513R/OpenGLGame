#include "VertexArray.h"
VertexArray::VertexArray(const float* vertPositions, unsigned int vertPositionsCount ,GLenum type):type(type),indecies(vertPositionsCount/3)
{
    // Generate and bind our VAO.
    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);

    //Create VBO
    vbo.resize(1);
    // Generate a buffer and bind it for use.
    glGenBuffers(1, &vbo[0]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    // Allocate buffer of a certain size and copy vertex data into it.
    // Arguments are TARGET, SIZE, DATA, USAGE
    glBufferData(GL_ARRAY_BUFFER, vertPositionsCount * sizeof(float), vertPositions, type);
    // Vertex attribute 0 (position data) has 3 components per vertex (x, y, z) and each is a float.
    // We don't want this data normalized (GL_FALSE), and the data is tightly packed (0).
    // There is no byte offset required (NULL).(if we have interleaved data)
    //
    // Option 1 (packed data)
    //[V1Pos][V2Pos][V3Pos][V1Color][V2Color][V3Color]
    // Option 2 (interleaved data)
    //[V1Pos][V1Color][V2Pos][V2Color][V3Pos][V3Color]
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    // Enable vertex attribute 0 (vertex position data).
    glEnableVertexAttribArray(0);
    delete[] vertPositions;
    glBindVertexArray(0);
}

void VertexArray::addComponent(unsigned int attrib, const float *positions, unsigned int positionCount) {
    glBindVertexArray(mVAO);

    bool resize = (vbo.size()<attrib+1);
    if (resize)vbo.resize(attrib+1);
    //Tell GLSL that we have data

    //create new buffer to hold our data
    if(!resize)glDeleteBuffers(1,&vbo[attrib]);
    glGenBuffers(1,&vbo[attrib]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[attrib]);
    //copy new data to array
    glBufferData(GL_ARRAY_BUFFER, positionCount*sizeof(float), positions,type);
    //For explanation look up!
    glVertexAttribPointer(attrib, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(attrib);

    delete[] positions;
}

void VertexArray::updateData(const float* data,int size,int offset){
    glBindVertexArray(mVAO);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferSubData(GL_ARRAY_BUFFER,offset,size,data);
    delete[] data;
}
void VertexArray::Draw(GLenum mode)
{
    glBindVertexArray(mVAO);
    glDrawArrays(mode, 0, indecies);
}

VertexArray::~VertexArray()
{

    for(GLuint& vboo : vbo){
        glDeleteBuffers(1,&vboo);
    }
    glDeleteVertexArrays(1, &mVAO);

}