#pragma once

#include "stdafx.h"

namespace zvk
{

struct Vertex
{
    glm::vec2 Position;
    glm::vec3 Color;

    static vk::VertexInputBindingDescription BindingDescription()
    {
        return vk::VertexInputBindingDescription{0, sizeof(Vertex), vk::VertexInputRate::eVertex};
    }

    static std::array<vk::VertexInputAttributeDescription, 2> AttributeDescriptions()
    {
        vk::VertexInputAttributeDescription descPosition{0, 0, vk::Format::eR32G32Sfloat,
                                                         offsetof(Vertex, Position)};
        vk::VertexInputAttributeDescription descColor{1, 0, vk::Format::eR32G32B32Sfloat,
                                                      offsetof(Vertex, Color)};

        return {descPosition, descColor};
    }
};

} // namespace zvk
