#include "GLExt.h"
#include <spdlog/spdlog.h>
#include <SDL2/SDL.h>

#define LOAD_OR_FAIL(slot, name) \
slot = (decltype(slot))(SDL_GL_GetProcAddress(name));\
if(!slot) \
{\
    spdlog::error(" " name " fail!");\
    return false;\
}\
spdlog::info("  " name)

namespace video
{
    bool GLExt::LoadGLFunctions()
    {
        LOAD_OR_FAIL(GLExt::CreateShader, "glCreateShader");
        LOAD_OR_FAIL(GLExt::CreateProgram, "glCreateProgram");
        LOAD_OR_FAIL(GLExt::AttachShader, "glAttachShader");
        LOAD_OR_FAIL(GLExt::ShaderSource, "glShaderSource");
        LOAD_OR_FAIL(GLExt::CompileShader, "glCompileShader");
        LOAD_OR_FAIL(GLExt::GetShaderiv, "glGetShaderiv");
        LOAD_OR_FAIL(GLExt::LinkProgram, "glLinkProgram");
        LOAD_OR_FAIL(GLExt::GetProgramiv, "glGetProgramiv");
        LOAD_OR_FAIL(GLExt::DeleteShader, "glDeleteShader");
        LOAD_OR_FAIL(GLExt::DeleteProgram, "glDeleteProgram");
        LOAD_OR_FAIL(GLExt::UseProgram, "glUseProgram");
        LOAD_OR_FAIL(GLExt::GetShaderInfoLog, "glGetShaderInfoLog");
        LOAD_OR_FAIL(GLExt::GetProgramInfoLog, "glGetProgramInfoLog");
        LOAD_OR_FAIL(GLExt::Uniform1i, "glUniform1i");
        LOAD_OR_FAIL(GLExt::Uniform1f, "glUniform1f");
        LOAD_OR_FAIL(GLExt::Uniform3f, "glUniform3f");
        LOAD_OR_FAIL(GLExt::Uniform4f, "glUniform4f");
        LOAD_OR_FAIL(GLExt::UniformMat4, "glUniformMatrix4fv");
        LOAD_OR_FAIL(GLExt::GetUniform, "glGetUniformLocation");
        LOAD_OR_FAIL(GLExt::IsProgram, "glIsProgram");
        LOAD_OR_FAIL(GLExt::IsShader, "glIsShader");
        LOAD_OR_FAIL(GLExt::GenFramebuffers, "glGenFramebuffers");
        LOAD_OR_FAIL(GLExt::BindFramebuffer, "glBindFramebuffer");
        LOAD_OR_FAIL(GLExt::FramebufferTexture, "glFramebufferTexture");
        LOAD_OR_FAIL(GLExt::EnableVertexAttribArray, "glEnableVertexAttribArray");
        LOAD_OR_FAIL(GLExt::VertexAttribPointer, "glVertexAttribPointer");
        LOAD_OR_FAIL(GLExt::DisableVertexAttribArray, "glDisableVertexAttribArray");
        LOAD_OR_FAIL(GLExt::ActiveTexture, "glActiveTexture");
        LOAD_OR_FAIL(GLExt::GenBuffers, "glGenBuffers");
        LOAD_OR_FAIL(GLExt::GenVertexArrays, "glGenVertexArrays");
        LOAD_OR_FAIL(GLExt::BindBuffer, "glBindBuffer");
        LOAD_OR_FAIL(GLExt::BufferData, "glBufferData");
        LOAD_OR_FAIL(GLExt::BindVertexArray, "glBindVertexArray");

        init = true;
        return true;
    }

    bool GLExt::IsInit()
    {
        return init;
    }

    void GLExt::CreateOrho(float* matrix, float left, float right, float bottom, float top, float nearVal, float farVal)
    {
        matrix[0] = 2.0f / (right - left);
        matrix[1] = 0.0f;
        matrix[2] = 0.0f;
        matrix[3] = 0.0f;

        matrix[4] = 0.0f;
        matrix[5] = 2.0f / (top - bottom);
        matrix[6] = 0.0f;
        matrix[7] = 0.0f;

        matrix[8] = 0.0f;
        matrix[9] = 0.0f;
        matrix[10] = -2.0f / (farVal - nearVal);
        matrix[11] = 0.0f;

        matrix[12] = -(right + left) / (right - left);
        matrix[13] = -(top + bottom) / (top - bottom);
        matrix[14] = -(farVal + nearVal) / (farVal - nearVal);
        matrix[15] = 1.0f;
    }

    void GLExt::MulMatrix4(float* matrix, float* matrix2)
    {
        float temp[16];

        for (int col = 0; col < 4; ++col)
        {
            for (int row = 0; row < 4; ++row)
            {
                float sum = 0.0f;

                for (int k = 0; k < 4; ++k)
                {
                    sum += matrix[k * 4 + row] * matrix2[col * 4 + k];
                }

                temp[col * 4 + row] = sum;
            }
        }

        memcpy(matrix, temp, 16);
    }

    void GLExt::CreateTransform(float* matrix, double x, double y, double scaleX, double scaleY, double angle, double originX, double originY)
    {
        memset(matrix, 0, sizeof(float) * 16);
        matrix[0] = 1.0f;
        matrix[5] = 1.0f;
        matrix[10] = 1.0f;
        matrix[15] = 1.0f;

        // Apply translation
        matrix[12] = x;
        matrix[13] = y;

        // Apply scaling
        matrix[0] *= scaleX;
        matrix[5] *= scaleY;

        // Apply rotation
        float cosAngle = std::cos(angle);
        float sinAngle = std::sin(angle);

        float a = matrix[0] * cosAngle - matrix[1] * sinAngle;
        float b = matrix[0] * sinAngle + matrix[1] * cosAngle;
        float c = matrix[4] * cosAngle - matrix[5] * sinAngle;
        float d = matrix[4] * sinAngle + matrix[5] * cosAngle;

        matrix[0] = a;
        matrix[1] = b;
        matrix[4] = c;
        matrix[5] = d;

        matrix[12] += a * -originX + c * -originY;
        matrix[13] += b * -originX + d * -originY;
    }
}