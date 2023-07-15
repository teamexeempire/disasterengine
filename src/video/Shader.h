#pragma once
#include <string>

#ifdef _WIN32
    #include <windows.h>
#endif

#include <GL/gl.h>
#include "Color.h"

namespace video
{
    using Uniform = int;
    class Shader
    {
    public:
        ~Shader();

        bool LoadFragment(const std::string& fragment);
        bool LoadFragment(const std::string& name, const std::string& groupName);
        bool LoadVertex(const std::string& vertex);
        bool LoadVertex(const std::string& name, const std::string& groupName);

        Uniform GetUniform(const char* name);

        void PassProjection(float* modelMatrix);
        void PassUniformi(Uniform uniform, int value);
        void PassUniformf(Uniform uniform, float value);
        void PassUniformMat4(Uniform uniform, float* matrix);
        void PassUniformRGB(Uniform uniform, Color color);
        void PassUniformRGBA(Uniform uniform, Color color);

        bool Compile();

    private:
        friend class Render;

        GLuint source;
        GLuint frag, vert;
        bool compiled = false;
    };
}