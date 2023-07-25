#pragma once

#ifdef _WIN32
    #include <windows.h>
    #include <GL/gl.h>
    #include "_glext.h"
#endif

#ifdef unix
    #include <GL/gl.h>
    #include <GL/glext.h>
    #include <GL/glx.h>
#endif

// functions

namespace video
{
    class GLExt
    {
    public:
        inline static PFNGLCREATESHADERPROC CreateShader = nullptr;
        inline static PFNGLCREATEPROGRAMPROC CreateProgram = nullptr;
        inline static PFNGLATTACHSHADERPROC AttachShader = nullptr;
        inline static PFNGLSHADERSOURCEPROC ShaderSource = nullptr;
        inline static PFNGLCOMPILESHADERPROC CompileShader = nullptr;
        inline static PFNGLGETSHADERIVPROC GetShaderiv = nullptr;
        inline static PFNGLLINKPROGRAMPROC LinkProgram = nullptr;
        inline static PFNGLGETPROGRAMIVPROC GetProgramiv = nullptr;
        inline static PFNGLDELETESHADERPROC DeleteShader = nullptr;
        inline static PFNGLDELETEPROGRAMPROC DeleteProgram = nullptr;
        inline static PFNGLUSEPROGRAMPROC UseProgram = nullptr;
        inline static PFNGLGETSHADERINFOLOGPROC GetShaderInfoLog = nullptr;
        inline static PFNGLGETPROGRAMINFOLOGPROC GetProgramInfoLog = nullptr;
        inline static PFNGLUNIFORM1IPROC Uniform1i = nullptr;
        inline static PFNGLUNIFORM1FPROC Uniform1f = nullptr;
        inline static PFNGLUNIFORM3FPROC Uniform3f = nullptr;
        inline static PFNGLUNIFORM4FPROC Uniform4f = nullptr;
        inline static PFNGLUNIFORMMATRIX4FVPROC UniformMat4 = nullptr;
        inline static PFNGLGETUNIFORMLOCATIONPROC GetUniform = nullptr;
        inline static PFNGLISPROGRAMPROC IsProgram = nullptr;
        inline static PFNGLISSHADERPROC IsShader = nullptr;
        inline static PFNGLGENFRAMEBUFFERSPROC GenFramebuffers = nullptr;
        inline static PFNGLBINDFRAMEBUFFERPROC BindFramebuffer = nullptr;
        inline static PFNGLFRAMEBUFFERTEXTUREPROC FramebufferTexture = nullptr;
        inline static PFNGLENABLEVERTEXATTRIBARRAYPROC EnableVertexAttribArray = nullptr;
        inline static PFNGLVERTEXATTRIBPOINTERPROC VertexAttribPointer = nullptr;
        inline static PFNGLDISABLEVERTEXATTRIBARRAYPROC DisableVertexAttribArray = nullptr;
        inline static PFNGLACTIVETEXTUREPROC ActiveTexture = nullptr;
        inline static PFNGLGENBUFFERSPROC GenBuffers = nullptr;
        inline static PFNGLGENVERTEXARRAYSPROC GenVertexArrays = nullptr;
        inline static PFNGLBINDBUFFERPROC BindBuffer = nullptr;
        inline static PFNGLBUFFERDATAPROC BufferData = nullptr;
        inline static PFNGLBINDVERTEXARRAYPROC BindVertexArray = nullptr;

        static void CreateOrho(float* matrix, float left, float right, float bottom, float top, float nearVal, float farVal);
        static void MulMatrix4(float* matrix, float* matrix2);
        static void CreateTransform(float* matrix, double x, double y, double scaleX = 1, double scaleY = 1, double angle = 0, double originX = 0, double originY = 0);

        static bool LoadGLFunctions();
        static bool IsInit();

    private:
        static void* LoadGLFunction(const char* name);

        inline static bool init = false;
    };
}