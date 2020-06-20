#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
glm::mat4 Camera::view() const {
//    glm::vec3 zaxis = glm::normalize(position - lookAt);
//    glm::vec3 xaxis = glm::normalize(cross(up, zaxis));
//    glm::vec3 yaxis = cross(zaxis, xaxis);
//
//    // Create a 4x4 view matrix from the right, up, forward and eye position vectors
//    glm::mat4 viewMatrix = {
//            glm::vec4(      xaxis.x,            yaxis.x,            zaxis.x,       0 ),
//            glm::vec4(      xaxis.y,            yaxis.y,            zaxis.y,       0 ),
//            glm::vec4(      xaxis.z,            yaxis.z,            zaxis.z,       0 ),
//            glm::vec4(-dot( xaxis, position ), -dot( yaxis, position ), -dot( zaxis, position ),  1 )
//    };

    return glm::lookAt(lookAt,position,up);
}
