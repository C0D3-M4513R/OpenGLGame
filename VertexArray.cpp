//
// Created by Timon Kayser on 12.06.2020.
//

#include "VertexArray.h"

VertexArray::VertexArray(const float* vertPositions, unsigned int vertPositionsCount, bool colorData ,bool dynamic)
{
    //Create VBO

    // Each vertex position is 3 elements, so divide by 3 to get vertex count.
    //Note this assumes a 3D space
    vertexCount = vertPositionsCount/3;

    // Generate a buffer and bind it for use.
    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);

    // Allocate buffer of a certain size and copy vertex data into it.
    // Arguments are TARGET, SIZE, DATA, USAGE
    glBufferData(GL_ARRAY_BUFFER, vertPositionsCount * sizeof(float) * (colorData?2:1), vertPositions, dynamic?GL_DYNAMIC_DRAW:GL_STATIC_DRAW);


    // Generate and bind our VAO.
    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);

    // Enable vertex attribute 0 (vertex position data).
    glEnableVertexAttribArray(0);
    if(colorData)glEnableVertexAttribArray(1);

    // Make sure VBO is bound, so the VAO knows what buffer to use in conjunction with the VAO.
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);

    // Vertex attribute 0 (position data) has 3 components per vertex (x, y, z) and each is a float.
    // We don't want this data normalized (GL_FALSE), and the data is tightly packed (0).
    // There is no byte offset required (NULL).(if we have interleaved data)

    // Option 1 (packed data)
    //[V1Pos][V2Pos][V3Pos][V1Color][V2Color][V3Color]
    // Option 2 (interleaved data)
    //[V1Pos][V1Color][V2Pos][V2Color][V3Pos][V3Color]
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    if(colorData)glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0,(void*)(vertPositionsCount*sizeof(float)));
}

void VertexArray::updateData(const float* data,int size,int offset){
    glBufferSubData(GL_ARRAY_BUFFER,offset,size,data);
}

unsigned int VertexArray::getVertexCount() const {
    return vertexCount;
}

void VertexArray::Draw(GLenum mode)
{
    glBindVertexArray(mVAO);
    glDrawArrays(mode, 0, vertexCount);
}

VertexArray::~VertexArray()
{
    if(mVBO != GL_NONE)
    {
        glDeleteBuffers(1, &mVBO);
    }
    if(mVAO != GL_NONE)
    {
        glDeleteVertexArrays(1, &mVAO);
    }
}