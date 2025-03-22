#pragma once

#include "stdafx.h"

struct UniformBufferObject
{
    glm::mat4 Model;
    glm::mat4 View;
    glm::mat4 Projection;
};
