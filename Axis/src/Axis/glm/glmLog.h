#pragma once

#include <glm/mat4x4.hpp>
#include <glm/mat3x3.hpp>

#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

inline std::ostream& operator<<(std::ostream& os, const glm::mat4& mat)
{
    for (int i = 0; i < 4; i++)
    {
        os << "\n[\t";
        for (int j = 0; j < 4; j++)
        {
            os << mat[j][i] << "\t";
        }
        os << "]";
    }
    return os;
}

inline std::ostream& operator<<(std::ostream& os, const glm::mat3& mat)
{
    for (int i = 0; i < 3; i++)
    {
        os << "\n[\t";
        for (int j = 0; j < 3; j++)
        {
            os << mat[j][i] << "\t";
        }
        os << "]";
    }
    return os;
}

inline std::ostream& operator<<(std::ostream& os, const glm::vec4& vec)
{
    return os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ")";
}

inline std::ostream& operator<<(std::ostream& os, const glm::vec3& vec)
{
    return os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
}

inline std::ostream& operator<<(std::ostream& os, const glm::vec2& vec)
{
    return os << "(" << vec.x << ", " << vec.y << ")";
}

