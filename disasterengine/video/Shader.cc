#include "Shader.h"
#include "GLExt.h"
#include "Render.h"
#include "../res/Resources.h"
#include <spdlog/spdlog.h>

namespace video
{
    Shader::~Shader()
    {
        if (!GLExt::IsInit())
            return;

        if (GLExt::IsShader(frag))
            GLExt::DeleteShader(frag);

        if (GLExt::IsShader(vert))
            GLExt::DeleteShader(vert);

        if (GLExt::IsProgram(source))
            GLExt::DeleteProgram(source);
    }

    bool Shader::LoadFragment(const std::string& fragment)
    {
        if (!GLExt::IsInit())
            return false;

        frag = GLExt::CreateShader(GL_FRAGMENT_SHADER);
        const char* const arr[] = { fragment.c_str() };
        GLExt::ShaderSource(frag, 1, arr, NULL);
        return true;
    }

    bool Shader::LoadFragment(const std::string& name, const std::string& groupName)
    {
        if (!GLExt::IsInit())
            return false;
        
        auto res = Resources::Get<ArrayResource>(name, groupName);
        if (!res)
        {
            spdlog::error("Failed to load fragment shader \"{}\" (group {}): Key doesn't exist.", name, groupName);
            return false;
        }

        frag = GLExt::CreateShader(GL_FRAGMENT_SHADER);
        const char* const arr[] = { (const char*)res->Data()};
        const int arr2[] = { (int)res->Size() };
        GLExt::ShaderSource(frag, 1, arr, arr2);
        return true;
    }

    bool Shader::LoadVertex(const std::string& vertex)
    {
        if (!GLExt::IsInit())
            return false;

        vert = GLExt::CreateShader(GL_VERTEX_SHADER);
        const char* const arr[] = { vertex.c_str() };
        GLExt::ShaderSource(vert, 1, arr, NULL);
        return true;
    }

    bool Shader::LoadVertex(const std::string& name, const std::string& groupName)
    {
        if (!GLExt::IsInit())
            return false;

        auto res = Resources::Get<ArrayResource>(name, groupName);
        if (!res)
        {
            spdlog::error("Failed to load fragment shader \"{}\" (group {}): Key doesn't exist.", name, groupName);
            return false;
        }

        vert = GLExt::CreateShader(GL_VERTEX_SHADER);
        const char* const arr[] = { (const char*)res->Data() };
        const int arr2[] = { (int)res->Size() };
        GLExt::ShaderSource(vert, 1, arr, arr2);
        return true;
    }

    Uniform Shader::GetUniform(const char* name)
    {
        return GLExt::GetUniform(source, name);
    }

    void Shader::PassUniformi(Uniform uniform, int value)
    {
        GLExt::Uniform1i(uniform, value);
    }

    void Shader::PassUniformf(Uniform uniform, float value)
    {
        GLExt::Uniform1f(uniform, value);
    }

    void Shader::PassUniformMat4(Uniform uniform, float* matrix)
    {
        GLExt::UniformMat4(uniform, 1, false, matrix);
    }

    void Shader::PassUniformRGB(Uniform uniform, Color color)
    {
        GLExt::Uniform3f(uniform, color.r, color.g, color.b);
    }

    void Shader::PassUniformRGBA(Uniform uniform, Color color)
    {
        GLExt::Uniform4f(uniform, color.r, color.g, color.b, color.a);
    }

    void Shader::PassProjection(float* modelMatrix)
    {
        GLint model = GLExt::GetUniform(this->source, "model");
        GLint ortho = GLExt::GetUniform(this->source, "ortho");

        GLfloat orthoMatrix[16];
        GLExt::CreateOrho(orthoMatrix, 0.0f, 480, 270, 0, -1.0f, 1.0f);

        GLExt::UniformMat4(ortho, 1, false, orthoMatrix);
        GLExt::UniformMat4(model, 1, false, modelMatrix);
    }

    bool Shader::Compile()
    {
        if (!GLExt::IsInit())
            return false;

        int success;
        source = GLExt::CreateProgram();

        GLExt::CompileShader(vert);

        GLExt::GetShaderiv(vert, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            char infoLog[512];
            memset(infoLog, 0, 512);

            GLExt::GetShaderInfoLog(vert, 512, NULL, infoLog);
            spdlog::error("Vertex shader ({}) compilation failed:\n{}", source, infoLog);
            return false;
        }

        GLExt::CompileShader(frag);

        GLExt::GetShaderiv(frag, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            char infoLog[512];
            memset(infoLog, 0, 512);

            GLExt::GetShaderInfoLog(frag, 512, NULL, infoLog);
            spdlog::error("Fragment shader ({}) compilation failed:\n{}", source, infoLog);
            return false;
        }

        GLExt::AttachShader(source, vert);
        GLExt::AttachShader(source, frag);
        GLExt::LinkProgram(source);

        GLExt::GetProgramiv(source, GL_LINK_STATUS, &success);
        if (!success)
        {
            char infoLog[512];
            memset(infoLog, 0, 512);

            GLExt::GetProgramInfoLog(frag, 512, NULL, infoLog);
            spdlog::error("Shader ({}) link failed:\n{}", source, infoLog);
            return false;
        }

        spdlog::info("Shader ({}) link success!", source);
        compiled = true;

        return true;
    }
}